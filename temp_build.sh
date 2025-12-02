#!/bin/bash

shopt -s globstar

# Print compiler version
wibo $ARMCC_4_1_BIN/armcc.exe --help | head -n 1
wibo $ARMCC_4_1_BIN/armlink.exe --help | head -n 1
echo

# TODO use an actual build system
files=$(echo src/**/*.cpp)

for src_file in $files; do
    out_file="build/${src_file:4:-4}.o"
    echo "Compiling $src_file (output: $out_file)"
    mkdir -p "$(dirname "$out_file")"
    
    wibo $ARMCC_4_1_BIN/armcc.exe --cpp --arm --split_sections --debug --no_debug_macros --gnu \
        --debug_info=line_inlining_extensions -O3 -Otime --data_reorder --signed_chars \
        --multibyte_chars --remove_unneeded_entities --force_new_nothrow --remarks --no_rtti \
        -I include -c $src_file -o $out_file
done

echo Linking files build/**/*.o ...
wibo $ARMCC_4_1_BIN/armlink.exe --output build/out --unresolved=_Z4stubv build/**/*.o
