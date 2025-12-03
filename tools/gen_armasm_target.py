#/usr/bin/env python3

from dataclasses import dataclass
from sys import argv
import re

@dataclass
class Symbol:
    # e.g. isFloat(Variable*)
    signature: str
    # Name of symbol
    name: str
    # List of data types, e.g. 'Variable*'
    args: list[str]
    
    def from_symbol_sig(symbol_sig: str):
        symbol_sig = symbol_sig.strip()
        
        if '(' in symbol_sig:
            name = symbol_sig[:symbol_sig.index('(')]
            args = symbol_sig[symbol_sig.index('(') + 1:-1].split(',')
        else:
            name = symbol_sig
            args = []
        
        return Symbol(symbol_sig, name, args)
    
    def mangle(self) -> str:
        sig_mangled = ''.join(mangle_typename_segment(name) for name in self.args)
        return f"_Z{len(self.name)}{self.name}{sig_mangled}"


ARM_FUNC_START_DIRECTIVE = '	arm_func_start '
ARM_FUNC_START_DIRECTIVE_LEN = len(ARM_FUNC_START_DIRECTIVE)

def index_disasm(disasm: list[str]) -> dict[str, int]:
    result = {}
    
    for i, line in enumerate(disasm):
        if line.startswith(ARM_FUNC_START_DIRECTIVE):
            result[line[ARM_FUNC_START_DIRECTIVE_LEN:].rstrip()] = i
    
    return result

def get_referenced_symbol(symbols: dict[str, Symbol], line: str) -> str | None:
    [symbol] = re.findall("^\\t(?:bl|b) (\\w+)$", line.rstrip()) or [None]
    return symbol

def mangle_typename_segment(name: str) -> str:
    name = name.strip()
    if name.endswith('*'):
        prefix = 'P'
        name = name[:-1].rstrip()
    else:
        prefix = ''
    
    return prefix + f"{len(name)}{name}"

def get_function_asm(disasm: list[str], disasm_functions: dict[str, int], symbols: dict[str, Symbol], symbol: Symbol) -> str:
    result = ""
    
    result += f"\tAREA |i.{symbol.mangle()}|, CODE, READONLY\n"
    result += f"\tGLOBAL |{symbol.signature}|\n\n"
    
    for i, line in enumerate(disasm[disasm_functions[symbol.name] + 1:]):
        if line.startswith(ARM_FUNC_START_DIRECTIVE):
            break
        
        # Check for label and convert them into armasm syntax
        if ':' in line:
            colon_idx = line.index(':')
            result += line[:colon_idx].replace(symbol.name, f"|{symbol.signature}|") + '\n'
            continue
        
        # Remove @ and everything after it
        if '@' in line:
            at_sign_idx = line.index('@')
            line = line[:at_sign_idx] + '\n'
        
        if external_symbol_name := get_referenced_symbol(symbols, line):
            external_symbol = symbols.get(external_symbol_name, None)
            
            if external_symbol:
                line = line.replace(external_symbol_name, f"|{external_symbol.signature}|")
        
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
            if symbols.get(extern_symbol, None) is None and (extern_symbol is None or not extern_symbol.startswith('_')):
                extern_symbols.add(extern_symbol)
    
    for extern_symbol in extern_symbols:
        result += f"\tEXTERN {extern_symbol}\n"
    
    result += "\n"
    
    # Main symbol content
    for name, symbol in symbols.items():
        if name not in disasm_functions:
            continue
        
        result += get_function_asm(disasm, disasm_functions, symbols, symbol)
    
    result += "\tEND\n\n"
    
    with open(output_file_path, 'w') as f:
        f.write(result)

if __name__ == '__main__':
    main()

