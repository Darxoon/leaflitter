#!/bin/bash

shopt -s globstar

# TODO use an actual build system
for src_file in src/**/*.cpp; do
    out_file="build/${src_file:4:-4}.o"
    mkdir -p "$(dirname "$out_file")"
    
    wibo $ARMCC_504_BIN/armcc.exe --cpp --arm --split_sections --debug --no_debug_macros --gnu --debug_info=line_inlining_extensions -O3 -Otime --data_reorder --signed_chars --multibyte_chars --remove_unneeded_entities --force_new_nothrow --remarks --no_rtti -I include -c $src_file -o $out_file
done

