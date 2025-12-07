#/usr/bin/env python3

from dataclasses import dataclass
from enum import Enum
from sys import argv
import re

EXTERN_C_DECLARATION = "extern \"C\""

@dataclass
class Symbol:
    # the symbol's raw offset, only set if necessary, e.g. 0x1027cc
    raw_offset: int | None
    # e.g. isFloat(Variable*)
    signature: str
    # Whether the symbol name should be mangled
    no_mangle: bool
    namespaces: list[str]
    # Name of symbol
    name: str
    # List of data types, e.g. 'Variable*'
    args: list[str]
    
    def from_symbol_sig(symbol_sig: str):
        # Handle and remove offset prefix
        if ':' in symbol_sig and symbol_sig[symbol_sig.index(':') + 1] != ':':
            raw_offset_str = symbol_sig[:symbol_sig.index(':')]
            if raw_offset_str.startswith('0x'):
                raw_offset = int(raw_offset_str[2:], 16)
            else:
                raw_offset = int(raw_offset_str)
            
            symbol_sig = symbol_sig[symbol_sig.index(':') + 1:]
        else:
            raw_offset = None
        
        # Don't parse signature if extern "C" prefix is passed
        if symbol_sig.startswith(EXTERN_C_DECLARATION):
            symbol_sig = symbol_sig[len(EXTERN_C_DECLARATION):].strip()
            return Symbol(raw_offset, symbol_sig, True, [], symbol_sig, [])
        
        # Parse signature
        *namespaces, symbol_sig = symbol_sig.strip().split('::')
        
        if '(' in symbol_sig:
            name = symbol_sig[:symbol_sig.index('(')]
            args = symbol_sig[symbol_sig.index('(') + 1:-1].split(',')
        else:
            name = symbol_sig
            args = []
        
        return Symbol(raw_offset, symbol_sig, False, namespaces, name, args)
    
    def mangle(self) -> str:
        if self.no_mangle:
            return self.signature
        
        namespaces_mangled = ''.join(f"N{len(namespace)}{namespace}" for namespace in self.namespaces)
        sig_mangled = ''.join(mangle_typename_segment(name) for name in self.args)
        ident_end = 'E' if self.namespaces else ''
        return f"_Z{namespaces_mangled}{len(self.name)}{self.name}{ident_end}{sig_mangled}"

class ArchType(Enum):
    ARM = 0
    Thumb = 1

@dataclass
class Ctx:
    disasm: list[str]
    disasm_functions: dict[str, int]
    disasm_types: dict[str, ArchType]
    
    symbols: dict[str, Symbol]
    symbol_offsets: dict[int, Symbol]


ARM_FUNC_START_DIRECTIVE = '	arm_func_start '
THUMB_FUNC_START_DIRECTIVE = '	thumb_func_start '

def index_disasm(disasm: list[str]) -> (dict[str, int], dict[str, ArchType]):
    indices = {}
    arch_types = {}
    
    for i, line in enumerate(disasm):
        if line.startswith(ARM_FUNC_START_DIRECTIVE):
            indices[line[len(ARM_FUNC_START_DIRECTIVE):].rstrip()] = i
            arch_types[line[len(ARM_FUNC_START_DIRECTIVE):].rstrip()] = ArchType.ARM
        elif line.startswith(THUMB_FUNC_START_DIRECTIVE):
            indices[line[len(THUMB_FUNC_START_DIRECTIVE):].rstrip()] = i
            arch_types[line[len(THUMB_FUNC_START_DIRECTIVE):].rstrip()] = ArchType.Thumb
    
    return indices, arch_types

def get_referenced_symbol(symbols: dict[str, Symbol], line: str) -> str | None:
    [symbol] = re.findall("^\\t(?:bl|b|blx) (#?[\\w\\(\\)\\*,\\s]+)$", line.rstrip()) or [None]
    return symbol

def mangle_typename_segment(name: str) -> str:
    name = name.strip()
    
    prefix = ""
    
    match name:
        case "int":
            name = 'i'
        case "bool":
            name = 'b'
        case name if name.endswith('*'):
            name = name[:-1].rstrip()
            prefix = 'P' + str(len(name))
        case _:
            prefix = str(len(name))
    
    return prefix + name

def get_function_asm(ctx: Ctx, i: int, symbol: Symbol) -> str:
    result = ""
    
    prefix = 'i' if i is None else f"j{i:02}"
    result += f"\tAREA |{prefix}.{symbol.mangle()}|, CODE, READONLY\n"
    result += f"\tGLOBAL {symbol.mangle()}\n"
    
    match ctx.disasm_types[symbol.name]:
        case ArchType.ARM:
            result += f"\tARM\n\n"
        case ArchType.Thumb:
            result += f"\tTHUMB\n\n"
    
    for i, line in enumerate(ctx.disasm[ctx.disasm_functions[symbol.name] + 1:]):
        if line.startswith(ARM_FUNC_START_DIRECTIVE) or line.startswith(THUMB_FUNC_START_DIRECTIVE):
            break
        
        # Check for label and convert them into armasm syntax
        if ':' in line:
            colon_idx = line.index(':')
            line = line[:colon_idx].replace(symbol.name, symbol.mangle()) + line[colon_idx + 1:]
        
        # Remove @ and everything after it
        if '@' in line:
            at_sign_idx = line.index('@')
            line = line[:at_sign_idx] + '\n'
        
        # Replace unmangled input symbols with mangled ones
        if external_symbol_name := get_referenced_symbol(ctx.symbols, line):
            external_symbol = ctx.symbols.get(external_symbol_name, None)
            
            if external_symbol_name.startswith('#'):
                # Special case: armasm does not accept 'blx #...', the # has to be removed
                external_symbol_name = external_symbol_name[1:]
                try:
                    if external_symbol_name.startswith('0x'):
                        as_int = int(external_symbol_name[2:], 16)
                    else:
                        as_int = int(external_symbol_name)
                    
                    # If it did not error, that means it is an int
                    external_symbol = ctx.symbol_offsets.get(as_int, None)
                    if external_symbol is not None:
                        line = line.replace('#' + external_symbol_name, external_symbol.mangle())
                    else:
                        line = line.replace('#' + external_symbol_name, external_symbol_name)
                except ValueError:
                    pass
            elif external_symbol:
                line = line.replace(external_symbol_name, external_symbol.mangle())
            elif '(' in external_symbol_name or ')' in external_symbol_name or '*' in external_symbol_name:
                line = line.replace(external_symbol_name, f"|{external_symbol_name}|")
        
        # Convert direct byte insert directives into correct syntax
        line = line.replace('.byte', 'dcb')
        line = line.replace('.4byte', 'dcq')
        
        # Convert align directive
        line = line.replace('.align', 'align')
        
        result += line
    
    return result

def main():
    """
    Takes in a list of symbol/function names that are part of a given translation unit
    and splits them out from asm/disasm.s into its own armasm-comptatible .s file
    so it can be assembled by armasm.
    """
    
    if len(argv) <= 1:
        print("Usage: python gen_armasm_target.py <output.s> <list of symbols.txt>")
        return
    
    output_file_path = argv[1]
    symbols_path = argv[2]
    
    with open(symbols_path, 'r') as f:
        symbol_sigs = f.readlines()
    
    symbols = [Symbol.from_symbol_sig(sig) for sig in symbol_sigs if sig.strip()]
    
    symbol_offsets = {symbol.raw_offset: symbol for symbol in symbols if symbol.raw_offset is not None}
    symbols = {symbol.name: symbol for symbol in symbols}
    
    # Global disasm.s
    with open('asm/disasm.s', 'r') as f:
        disasm = f.readlines()
    
    disasm_functions, disasm_types = index_disasm(disasm)
    
    result = ""
    
    # Find all defined symbols
    defined_symbols = set()
    
    for name in symbols.keys():
        if name not in disasm_functions:
            print(f"WARNING: {name!r} not in disasm.s")
            continue
        
        for line in disasm[disasm_functions[name] + 1:]:
            if line.startswith(ARM_FUNC_START_DIRECTIVE) or line.startswith(THUMB_FUNC_START_DIRECTIVE):
                break
            
            if ':' in line:
                defined_symbols.add(line[:line.index(':')].strip())
    
    # Extern symbol declarations
    extern_symbols = set()
    
    for name in symbols.keys():
        if name not in disasm_functions:
            continue
        
        for line in disasm[disasm_functions[name] + 1:]:
            if line.startswith(ARM_FUNC_START_DIRECTIVE) or line.startswith(THUMB_FUNC_START_DIRECTIVE):
                break
            
            extern_symbol = get_referenced_symbol(symbols, line.rstrip())
            is_really_extern = (
                extern_symbol is not None
                and not extern_symbol.startswith('#')
                and symbols.get(extern_symbol, None) is None
                and (extern_symbol.startswith('_Z') or extern_symbol not in defined_symbols)
                and extern_symbol not in ('pc', 'lr', 'r0')
            )
            if is_really_extern:
                extern_symbols.add(extern_symbol)
    
    for extern_symbol in extern_symbols:
        result += f"\tEXTERN {extern_symbol}\n"
    
    result += "\n"
    
    # Handle sorting of symbols
    sorted_syms = [symbol for name, symbol in symbols.items() if name in disasm_functions]
    sorted_syms.sort(key=lambda sym: sym.mangle())
    
    # Main symbol content
    ctx = Ctx(disasm, disasm_functions, disasm_types, symbols, symbol_offsets)
    
    is_sorted = True
    i = 0
    for name, symbol in symbols.items():
        if name not in disasm_functions:
            continue
        
        if is_sorted:
            is_sorted = symbol == sorted_syms[i]
        
        index = None if is_sorted else i
        
        result += get_function_asm(ctx, index, symbol)
        i += 1
    
    result += "\tEND\n\n"
    
    with open(output_file_path, 'w') as f:
        f.write(result)

if __name__ == '__main__':
    main()

