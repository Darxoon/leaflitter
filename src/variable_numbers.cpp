#include "variable_numbers.h"

Variable* getReferenceType(Variable* variable);
void logScriptError(int dst, const char* msg, ...);
bool isSavedVar(Variable* variable);
int func_0029e33c(Variable* variable);
bool func_002a9a74(Variable* variable);
bool isFunc(Variable* variable);
bool isAllocVar(Variable* variable);
UNK_TYPE func_0029e9b4(Variable* variable);

// for some reason I need to put them here for getUnk12 specifically
const char ERROR_MSG_1[] = "変数が未初期化で使用されています。\n";
const char ERROR_MSG_2[] = "変数の型変換できません！\n";
const char ERROR_MSG_3[] = "参照型変数に値が入っていません。\n";

int getTable(Variable* var) {
    if (var->status == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, ERROR_MSG_1, -1);
        /* Cannot convert variable type! */
        logScriptError(0, ERROR_MSG_2, -1);
        return var->userData.asInt;
    }

    // make sure var is not a reference
    if (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, ERROR_MSG_3, -1);
        }

        return getTable(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isTable(var)) {
        return var->userData.asInt;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, ERROR_MSG_2, -1);
    
    return var->userData.asInt;
}

bool __attribute__((noinline)) isTable(Variable* var) {
    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isTable(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_TABLE;
}

bool __attribute__((noinline)) isFunc2(Variable* var) {
    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isFunc2(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_FUNC2;
}

// not matching: unimplemented
void setBool(Variable* var, bool value) {
    
}

bool getBool(Variable* var) {
    if (var->status == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asBool;
    }

    // make sure var is not a reference
    if (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return getBool(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isBool(var)) {
        return var->userData.asBool;
    }
    
    if (isInt(var)) {
        return var->userData.asInt;
    }
    
    if (isFloat(var)) {
        return var->userData.asFloat;
    }
    
    if (isHex(var)) {
        return var->userData.asInt;
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asBool;
}

bool __attribute__((noinline)) isBool(Variable* var) {
    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isBool(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_BOOL;
}

// not matching: unimplemented
void setPtr(Variable* var, UNK_TYPE value) {
    
}

UNK_TYPE getPtr(Variable* var) {
    if (var->status == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }

    // make sure var is not a reference
    if (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return getPtr(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isAllocVar(var)) {
        return func_0029e9b4(var);
    }

    switch (var->status) {
        case VARIABLE_PTR:
            return var->userData.asInt;
        case VARIABLE_REF:
            if (getReferenceType(var) != nullptr) {
                if (isPtr(getReferenceType(var))) {
                    return var->userData.asInt;
                }
            }
    }
    
    if (isFunc(var) || isBool(var) || isTable(var) || isInt(var) || isHex(var)) {
        return var->userData.asInt;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asInt;
}

bool __attribute__((noinline)) isPtr(Variable* var) {
    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isPtr(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_PTR;
}

unsigned int getHex(Variable* var) {
    if (var->status == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asUint;
    }

    // make sure var is not a reference
    if (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return getHex(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }
    
    if (isHex(var)) {
        return var->userData.asUint;
    }
    
    if (isFloat(var)) {
        return var->userData.asFloat;
    }
    
    if (isInt(var)) {
        return var->userData.asInt;
    }
    
    if (isBool(var)) {
        return var->userData.asByte ? 1 : 0;
    }

    if (!func_002a9a74(var) && !isFunc(var) && !isFunc2(var) && !isTable(var)) {
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }
    
    return var->userData.asUint;
}

bool __attribute__((noinline)) isHex(Variable* var) {
    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isHex(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_HEX;
}

// not matching: compiler version
float getFloat(Variable* var) {
    if (var->status == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asFloat;
    }

    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return getFloat(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }
    
    if (isFloat(var)) {
        return var->userData.asFloat;
    }
    
    if (isInt(var)) {
        return var->userData.asInt;
    }

    if (isBool(var)) {
        return var->userData.asByte ? 1.0f : 0.0f;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asFloat;
}

bool __attribute__((noinline)) isFloat(Variable* var) {
    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isFloat(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_FLOAT;
}

int getInt(Variable* var) {
    if (var->status == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }

    // make sure var is not a reference
    if (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return getInt(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }

    if (isInt(var)) {
        return var->userData.asInt;
    }
    
    if (isFloat(var)) {
        return (int)var->userData.asFloat;
    }
    
    if (isHex(var)) {
        return var->userData.asInt;
    }

    switch (var->status) {
        case VARIABLE_PTR:
            return var->userData.asInt;
        case VARIABLE_REF:
            if (getReferenceType(var) != nullptr) {
                if (isPtr(getReferenceType(var))) {
                    return var->userData.asInt;
                }
            }
    }

    if (isBool(var)) {
        int result = var->userData.asUByte;
        if (result != 0)
            result = 1;
        return result;
    }
    
    if (!func_002a9a74(var) && !isFunc(var) && !isFunc2(var) && !isTable(var)) {
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }
    
    return var->userData.asInt;
}

bool __attribute__((noinline)) isInt(Variable* var) {
    // make sure var is not a reference
    if (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isInt(getReferenceType(var));
    }

    return var->status == VARIABLE_INT;
}

