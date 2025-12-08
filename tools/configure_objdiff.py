import json
import os
from pathlib import Path

from gen_armasm_target import Symbol

TARGET_PATH_ROOT = Path('build/orig')
OBJ_PATH_ROOT = Path('build/obj')

def get_unit(filepath: Path) -> dict:
    with open(filepath, 'r') as f:
        symbol_sigs = f.readlines()
    
    return {
        "name": filepath.stem,
        "target_path": str(TARGET_PATH_ROOT / filepath.stem) + '.o',
        "base_path": str(OBJ_PATH_ROOT / filepath.stem) + '.o',
        "metadata": {},
    }

def main():
    with open('objdiff_template.json', 'r') as f:
        template = f.read()
    
    cfg = json.loads(template)
    units = []
    
    for dirname, dirs, files in os.walk("split"):
        dirpath = Path(dirname)
        
        for file in sorted(files):
            filepath = dirpath / file
            units.append(get_unit(filepath))
    
    cfg['units'] = units
    
    with open('objdiff.json', 'w') as f:
        f.write(json.dumps(cfg, indent=4) + '\n')

if __name__ == '__main__':
    main()
