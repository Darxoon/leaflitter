#!/usr/bin/env python3
import re
from sys import argv

STATEMENT_REGEX = re.compile(r"^\s*((?:struct\s+)?(?:\w+::)*\w+(?:\s*\*)*) ([\w\?]+)(?:\[(\d+)\])?;")
UNNAMED_FIELD_REGEX = re.compile(r"^field\d*_0x([\dabcdef]+)$")

INT_TYPES = ['char', 'short', None, 'int']

def write_undefined_field(out_lines: list[str], field_name: str | None, field_size: int | None):
    if field_name is None or field_size is None:
        return
    
    if field_size == 3:
        out_lines.append(f"    char {field_name}[3];")
    else:
        out_lines.append(f"    {INT_TYPES[field_size - 1]} {field_name};")

def remove_padding_after_bools(lines: list[str]) -> list[str]:
    result: list[str] = []
    
    following_bool: bool = False
    
    for line in lines:
        if line == "};":
            result.append(line)
            return result
        
        match = STATEMENT_REGEX.match(line)
        if match is None:
            print("line without statement:", line)
            continue
        
        type_name, field_name, array_count = match.groups()
        
        # remove leading 'struct' because this isn't C
        if type_name.startswith('struct '):
            type_name = type_name[7:]
        # more modern pointer style
        type_name = type_name.replace(' *', '*')
        
        # on unnamed undefined fields, disable following_bool if aligned by 4
        match = UNNAMED_FIELD_REGEX.match(field_name)
        if match is not None and type_name.startswith('undefined'):
            field_offset = int(match.group(1), 16)
            if field_offset % 4 == 0:
                following_bool = False
        else:
            following_bool = type_name == "bool"
        
        if not following_bool or match is None or not type_name.startswith('undefined'):
            array_suffix = f"[{array_count}]" if array_count is not None else ""
            result.append(f"    {type_name} {field_name}{array_suffix};")
    
    raise ValueError(f"Could not find end of struct {argv[1]}")

def collapse_undefined(lines: list[str]) -> list[str]:
    result: list[str] = []
    
    undefined_field: str | None = None
    undefined_field_size: int | None = None
    
    for line in lines:
        if line == "};":
            write_undefined_field(result, undefined_field, undefined_field_size)
            result.append(line)
            return result
        
        match = STATEMENT_REGEX.match(line)
        if match is None:
            print("line without statement:", line)
            continue
        
        type_name, field_name, array_count = match.groups()
        
        if not type_name.startswith('undefined') or array_count is not None:
            write_undefined_field(result, undefined_field, undefined_field_size)
            undefined_field = None
            undefined_field_size = None
            
            # rename field23423_0x... to field_0x...
            match = UNNAMED_FIELD_REGEX.match(field_name)
            if match is not None:
                field_offset = int(match.group(1), 16)
                field_name = f"field_{hex(field_offset)}"
            
            array_suffix = f"[{array_count}]" if array_count is not None else ""
            result.append(f"    {type_name} {field_name}{array_suffix};")
        else:
            field_size = 1 if type_name == 'undefined' else int(type_name[9:])
            
            match = UNNAMED_FIELD_REGEX.match(field_name)
            if match is None:
                write_undefined_field(result, undefined_field, undefined_field_size)
                undefined_field = field_name
                undefined_field_size = field_size
            else:
                field_offset = int(match.group(1), 16)
                field_name = f"field_{hex(field_offset)}"
                
                if undefined_field_size is not None and undefined_field_size < 4 and field_offset % 4 == undefined_field_size:
                    undefined_field_size += 1
                else:
                    write_undefined_field(result, undefined_field, undefined_field_size)
                    undefined_field = field_name
                    undefined_field_size = field_size
    
    raise ValueError(f"Could not find end of struct {argv[1]}")

def write_undefined_field_array(out_lines: list[str], field_name: str | None, field_type: str | None, field_count: int | None):
    if field_name is None or field_type is None or field_count is None:
        return
    
    if field_count > 15:
        array_suffix = f"[{hex(field_count)}]"
    elif field_count > 1:
        array_suffix = f"[{field_count}]"
    else:
        array_suffix = ""
    
    out_lines.append(f"    {field_type} {field_name}{array_suffix};")

def collapse_unk_into_arrays(lines: list[str]) -> list[str]:
    result: list[str] = []
    
    undefined_field: str | None = None
    undefined_field_type: str | None = None
    undefined_field_count: int | None = None
    
    for line in lines:
        if line == "};":
            write_undefined_field_array(result, undefined_field, undefined_field_type, undefined_field_count)
            result.append(line)
            return result
        
        match = STATEMENT_REGEX.match(line)
        if match is None:
            print("line without statement:", line)
            continue
        
        type_name, field_name, array_count = match.groups()
        
        match = UNNAMED_FIELD_REGEX.match(field_name)
        if match is None or array_count is not None or type_name == "char":
            write_undefined_field_array(result, undefined_field, undefined_field_type, undefined_field_count)
            undefined_field = None
            undefined_field_type = None
            undefined_field_count = None
            
            array_suffix = f"[{array_count}]" if array_count is not None else ""
            result.append(f"    {type_name} {field_name}{array_suffix};")
        else:
            if undefined_field_count is not None and undefined_field_type == type_name:
                undefined_field_count += 1
            else:
                write_undefined_field_array(result, undefined_field, undefined_field_type, undefined_field_count)
                undefined_field = field_name
                undefined_field_type = type_name
                undefined_field_count = 1
    
    raise ValueError(f"Could not find end of struct {argv[1]}")

def main():
    if len(argv) != 3:
        print("Usage: process_ghidra_header.py <type name> <ghidra header.h>")
        return
    
    with open(argv[2], "r") as f:
        input_file = f.read()
    
    lines = input_file.splitlines()
    starting_line = lines.index(f"struct {argv[1]} {{")
    print(starting_line)
    lines = lines[starting_line + 1:]
    
    lines = remove_padding_after_bools(lines)
    lines = collapse_undefined(lines)
    lines = collapse_unk_into_arrays(lines)
    
    lines.insert(0, f"struct {argv[1]} {{")
    
    with open('out.h', 'w') as f:
        f.write('\n'.join(lines))

if __name__ == "__main__":
    main()