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
        float asFloat;
        char asByte;
        unsigned char asUByte;
    } userData;
};

// Variable statuses
#define VARIABLE_FLOAT 0
#define VARIABLE_INT 1
#define VARIABLE_UINT 2 /* unsure */
#define VARIABLE_STRING 3
#define VARIABLE_ALLOC 4
#define VARIABLE_USER_VAR 5

#define VARIABLE_BOOL 7 /* unsure */
#define VARIABLE_FUNC 8

#define VARIABLE_QUEUED_FREE 13
#define VARIABLE_UNINITIALIZED 14

#define VARIABLE_SAVED_VAR 20 /* unsure */
