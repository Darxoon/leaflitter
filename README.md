# Leaflitter

A WIP decompilation of Paper Mario: Sticker Star (Europe)

## Getting Started

### Requirements

Disassembling and compiling  the project is not yet possible on Windows, so use WSL if you are on Windows.

Make sure you have python3, make and an appropriate compiler (see [Building](#building) for more info on the compiler).

You also need Paper Mario: Sticker Star (European Release) and either need a [homebrewed 3DS with godmode9](https://3ds.hacks.guide) or [ctrtool](https://github.com/3DSGuy/Project_CTR/releases/tag/ctrtool-v1.2.0).

### Extracting code.bin

If you use ctrtool, open a command line in the same folder as ctrtool and run this command:

    ./ctrtool --exefsdir exefs <path to Sticker Star.3ds>

Then open the new `exefs` folder that was created and move the file `code.bin` into the root of the Leaflitter folder.

TODO: instructions on using godmode9

### Installing n3dsdisasm

Go to <https://github.com/gamestabled/n3dsdisasm/releases> and download "n3dsdisasm.exe" if you're on Windows or "n3dsdisasm" if you're on Linux and place it in a new folder. Copy the path to that folder and [add it to your PATH](https://windowsloop.com/how-to-add-to-windows-path/).

(On Linux, I suggest placing n3dsdisasm into the `~/.local/bin` folder. You might have to create the folder and restart your shell if it doesn't exist yet. Many distros add it to your PATH automatically but in case not, you will have to add it yourself.)

### Generating the disassembly

Open the Terminal in this folder and type `make disassemble`. This will generate the disassembly into the `asm/disasm.s` file.

## Building

Make sure you have armcc 4.1 build 995 (compiler version may change in the future). Go into the `win-x86_64` folder which should contain a bunch of .exe files. Set the environment variable `ARMCC_4_1_BIN` to that folder.

### Windows

WIP (Use WSL and then follow the Linux instructions.)

### Linux

Download [wibo](https://github.com/decompals/wibo/releases/tag/0.6.16) (a simpler wine alternative) and make sure the program is in your PATH. (Once again, I recommend putting it into `~/.local/bin`.)

Then run `make`, which will compile and link all source files into `build/out`. This is not matching yet and also not in the same format as the code.bin file you provided yet.

## objdiff

If you want to use the great tool [objdiff](https://github.com/encounter/objdiff), make sure you have followed the above steps first. Make will generate the `objdiff.json` file automatically, meaning that you can point objdiff to this folder (leaflitter).

Note: Make sure you have at least version 3.4.5, as earlier releases are broken for armcc binaries.

Also note: By default, almost nothing will be displayed in the symbol listings. That's because you have to go into the "Diff Options" toolbar and enable "Show hidden symbols".
