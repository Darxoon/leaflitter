#!/bin/sh

echo Disassembling code.bin...

n3dsdisasm code.bin -c stickerstar.cfg > asm/disasm.s

echo Done.
