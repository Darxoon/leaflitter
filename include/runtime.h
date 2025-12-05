#pragma once

#include "ctx.h"

#include "variable.h"

struct ScriptBinary {
    int filename;
    int field_0x4;
    // TODO: opaque struct pointer
    struct Function* defined_functions;
    struct Variable* defined_variables;
    struct Variable* defined_constants;
    struct Variable* field_0x14;
    struct Table* registered_tables;
    struct FunctionImport* defined_function_imports;
    u32 field_0x20;
    Variable* maybe_var;
    int* code_section;
    int code_section_length;
    int opposite_counter;
    ScriptBinary* next;
    struct Script* scripts[8];
    int current_script;
    int field_0x5c[4];
    char field_0x6c[3];
    char field_0x6f;
    char buf_field_0x36;
    char field_0x71;
    char field_0x72;
    char field_0x73;
};

struct FuncStackFrame {
    ScriptBinary* file;
    // TODO: opaque struct pointer
    struct Function* field_0x4;
    Variable* field_0x8[3];
    u32* code;
    short field_0x18;
    char field_0x1a;
    char field_0x1b;
    char field_0x1c;
    char field_0x1d;
    char field_0x1e;
    char field_0x1f;
    Variable* variables2[20];
    int field_0x70;
};

struct RuntimeUnk {
    int field_0x0[0x23];
};

struct Runtime {
    int flags;
    char field_0x4;
    char field_0x5;
    char field_0x6;
    char field_0x7;
    int field_0x8[0x10];
    ScriptBinary* ksm_file;
    int field_0x4c;
    ScriptBinary* ksm_file2;
    // TODO: opaque struct pointer
    struct Function* Func;
    ScriptBinary* fallback_ksm_file;
    Variable* variable1;
    Variable* variable2;
    int field_0x64[2];
    Variable* field_0x6c;
    int field_0x70;
    u32* code;
    int next_instruction;
    u32* field_0x7c;
    int field_0x80[7];
    short next_free_var;
    short field_0x9e[2];
    char arr_index;
    char field_0xa3;
    char field_0xa4;
    char field_0xa5;
    char field_0xa6;
    char field_0xa7;
    int field_0xa8[8];
    Runtime* field_0xc8;
    Runtime* next;
    Variable* variables1[16];
    int field_0x110[0x10];
    u32 field_0x150;
    int field_0x154[0x11];
    Variable* variables2[20];
    int field_0x1e8[0x163];
    FuncStackFrame FuncStack[2];
    int field_0x85c[0x13f];
    int funcStackSize;
    int field_0xd5c[0xd1];
    Variable* field_0x10a0;
    int field_0x10a4[15];
    RuntimeUnk argStack;
    int field_0x116c[3];
    int arg_stack_length;
    int field_0x117c[0x48];
    int switchNestLevel;
    int field_0x12a0[0x10];
    int doWhileNestLevel;
};
