# Leaflitter

A disassembly of Paper Mario: Sticker Star (Europe)

## Getting Started

### Extracting code.bin

You need Paper Mario: Sticker Star (Europe) and either need a [homebrewed 3DS with godmode9](https://3ds.hacks.guide) or [ctrtool](https://github.com/3DSGuy/Project_CTR/releases/tag/ctrtool-v1.2.0).

If you use ctrtool, open a command line in the same folder as ctrtool and run this command:

    ./ctrtool --exefsdir exefs <path to Sticker Star.3ds>

Then open the new `exefs` folder that was created and move the file `code.bin` into the root of the Leaflitter folder.

### Installing n3dsdisasm

Download [n3dsdisasm.exe](https://github.com/gamestabled/n3dsdisasm/releases) if you're on Windows or `n3dsdisasm` if you're on Linux and place it in a new folder. Copy the path to your directory and [add it to your PATH](https://windowsloop.com/how-to-add-to-windows-path/).

### Generating the disassembly

Open the Terminal or Git Bash and run `./disassemble.sh`. This will generate the disassembly into `asm/disasm.s`.

### Extending the config file

TODO
