#/usr/bin/env python3

from dataclasses import dataclass
from sys import argv
import re

@dataclass
class Symbol:
    # e.g. isFloat(Variable*)
    signature: str
    namespaces: list[str]
    # Name of symbol
    name: str
    # List of data types, e.g. 'Variable*'
    args: list[str]
    
    def from_symbol_sig(symbol_sig: str):
        *namespaces, symbol_sig = symbol_sig.strip().split('::')
        
        if '(' in symbol_sig:
            name = symbol_sig[:symbol_sig.index('(')]
            args = symbol_sig[symbol_sig.index('(') + 1:-1].split(',')
        else:
            name = symbol_sig
            args = []
        
        return Symbol(symbol_sig, namespaces, name, args)
    
    def mangle(self) -> str:
        namespaces_mangled = ''.join(f"N{len(namespace)}{namespace}" for namespace in self.namespaces)
        sig_mangled = ''.join(mangle_typename_segment(name) for name in self.args)
        ident_end = 'E' if self.namespaces else ''
        return f"_Z{namespaces_mangled}{len(self.name)}{self.name}{ident_end}{sig_mangled}"


ARM_FUNC_START_DIRECTIVE = '	arm_func_start '
ARM_FUNC_START_DIRECTIVE_LEN = len(ARM_FUNC_START_DIRECTIVE)

def index_disasm(disasm: list[str]) -> dict[str, int]:
    result = {}
    
    for i, line in enumerate(disasm):
        if line.startswith(ARM_FUNC_START_DIRECTIVE):
            result[line[ARM_FUNC_START_DIRECTIVE_LEN:].rstrip()] = i
    
    return result

def get_referenced_symbol(symbols: dict[str, Symbol], line: str) -> str | None:
    [symbol] = re.findall("^\\t(?:bl|b) ([\\w\\(\\)\\*,\\s]+)$", line.rstrip()) or [None]
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

def get_function_asm(disasm: list[str], disasm_functions: dict[str, int], symbols: dict[str, Symbol], i: int, symbol: Symbol) -> str:
    result = ""
    
    prefix = 'i' if i is None else f"j{i:02}"
    result += f"\tAREA |{prefix}.{symbol.mangle()}|, CODE, READONLY\n"
    result += f"\tGLOBAL {symbol.mangle()}\n\n"
    
    for i, line in enumerate(disasm[disasm_functions[symbol.name] + 1:]):
        if line.startswith(ARM_FUNC_START_DIRECTIVE):
            break
        
        # Check for label and convert them into armasm syntax
        if ':' in line:
            colon_idx = line.index(':')
            result += line[:colon_idx].replace(symbol.name, symbol.mangle()) + '\n'
            continue
        
        # Remove @ and everything after it
        if '@' in line:
            at_sign_idx = line.index('@')
            line = line[:at_sign_idx] + '\n'
        
        if external_symbol_name := get_referenced_symbol(symbols, line):
            external_symbol = symbols.get(external_symbol_name, None)
            
            if external_symbol:
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
    
    symbols = (Symbol.from_symbol_sig(sig) for sig in symbol_sigs if sig.strip())
    symbols = {symbol.name: symbol for symbol in symbols}
    
    # Global disasm.s
    with open('asm/disasm.s', 'r') as f:
        disasm = f.readlines()
    
    disasm_functions = index_disasm(disasm)
    
    result = ""
    
    # Extern symbol definitions
    extern_symbols = set()
    
    for name in symbols.keys():
        if name not in disasm_functions:
            print(f"WARNING: {name} not in disasm.s")
            continue
        
        for line in disasm[disasm_functions[name] + 1:]:
            if line.startswith(ARM_FUNC_START_DIRECTIVE):
                break
            
            extern_symbol = get_referenced_symbol(symbols, line.rstrip())
            is_really_extern = (
                extern_symbol is not None
                and symbols.get(extern_symbol, None) is None
                and (extern_symbol is None or extern_symbol.startswith('_Z') or not extern_symbol.startswith('_'))
            )
            if is_really_extern:
                extern_symbols.add(extern_symbol)
    
    for extern_symbol in extern_symbols:
        result += f"\tEXTERN |{extern_symbol}|\n"
    
    result += "\n"
    
    # Handle sorting of symbols
    sorted_syms = [symbol for name, symbol in symbols.items() if name in disasm_functions]
    sorted_syms.sort(key=lambda sym: sym.mangle())
    
    # Main symbol content
    is_sorted = True
    i = 0
    for name, symbol in symbols.items():
        if name not in disasm_functions:
            continue
        
        if is_sorted:
            is_sorted = symbol == sorted_syms[i]
        
        index = None if is_sorted else i
        
        result += get_function_asm(disasm, disasm_functions, symbols, index, symbol)
        i += 1
    
    result += "\tEND\n\n"
    
    with open(output_file_path, 'w') as f:
        f.write(result)

if __name__ == '__main__':
    main()

