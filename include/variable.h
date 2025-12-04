#pragma once

struct Variable {
    char* name;
    int handle;
    struct {
        int status : 24; /* should really be called data type or something like this */
        int field_0xb : 8;
    };
    int field_0xc; /* might be a Variable* */
    Variable* next;

    // user data depending on status
    union {
        Variable* referenceType;
        char* asString;
        int asInt;
        unsigned int asUint;
        float asFloat;
        bool asBool;
        char asByte;
        unsigned char asUByte;
    } userData;
};

// Variable statuses
#define VARIABLE_FLOAT 0
#define VARIABLE_INT 1
#define VARIABLE_HEX 2
#define VARIABLE_STRING 3
#define VARIABLE_ALLOC 4
#define VARIABLE_REF 5
#define VARIABLE_PTR 6
#define VARIABLE_BOOL 7
#define VARIABLE_FUNC 8
#define VARIABLE_FUNC2 9
#define VARIABLE_EMPTY_STRING 10
#define VARIABLE_STRING_ME 11
#define VARIABLE_TABLE 12
#define VARIABLE_QUEUED_FREE 13
#define VARIABLE_NOINIT 14

#define VARIABLE_USER_VAR 20
