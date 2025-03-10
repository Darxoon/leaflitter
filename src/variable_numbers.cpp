#include "variable_numbers.h"

Variable* getReferenceType(Variable* variable);
void logScriptError(int dst, const char* msg, ...);
bool isSavedVar(Variable* variable);
int func_0029e33c(Variable* variable);
bool func_002a9a74(Variable* variable);
bool isFunc(Variable* variable);
bool isAllocVar(Variable* variable);
UNK_TYPE func_0029e9b4(Variable* variable);

#define VARIABLE_UNK_6 6
#define VARIABLE_UNK_9 9
#define VARIABLE_UNK_12 12

// for some reason I need to put them here for getUnk12 specifically
const char ERROR_MSG_1[] = "変数が未初期化で使用されています。\n";
const char ERROR_MSG_2[] = "変数の型変換できません！\n";
const char ERROR_MSG_3[] = "参照型変数に値が入っていません。\n";

int getUnk12(Variable* var) {
    if (var->status == VARIABLE_UNINITIALIZED) {
        /* A variable is used without being initialized. */
        logScriptError(0, ERROR_MSG_1, -1);
        /* Cannot convert variable type! */
        logScriptError(0, ERROR_MSG_2, -1);
        return var->userData.asInt;
    }

    // make sure var is not a UserVar
    if (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, ERROR_MSG_3, -1);
        }

        return getUnk12(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isUnk12(var)) {
        return var->userData.asInt;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, ERROR_MSG_2, -1);
    
    return var->userData.asInt;
}

bool isUnk12(Variable* var) {
    // make sure var is not a UserVar
    while (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isUnk12(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_UNK_12;
}

bool isUnk9(Variable* var) {
    // make sure var is not a UserVar
    while (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isUnk9(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_UNK_9;
}

// not matching: unimplemented
void setBool(Variable* var, bool value) {
    
}

bool getBool(Variable* var) {
    if (var->status == VARIABLE_UNINITIALIZED) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asBool;
    }

    // make sure var is not a UserVar
    if (var->status == VARIABLE_USER_VAR) {
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
    
    if (isUint(var)) {
        return var->userData.asInt;
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asBool;
}

bool isBool(Variable* var) {
    // make sure var is not a UserVar
    while (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isBool(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_BOOL;
}

// not matching: unimplemented
void setUnk6(Variable* var, UNK_TYPE value) {
    
}

UNK_TYPE getUnk6(Variable* var) {
    if (var->status == VARIABLE_UNINITIALIZED) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }

    // make sure var is not a UserVar
    if (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return getUnk6(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isAllocVar(var)) {
        return func_0029e9b4(var);
    }

    switch (var->status) {
        case VARIABLE_UNK_6:
            return var->userData.asInt;
        case VARIABLE_USER_VAR:
            if (getReferenceType(var) != nullptr) {
                if (isUnk6(getReferenceType(var))) {
                    return var->userData.asInt;
                }
            }
    }
    
    if (isFunc(var) || isBool(var) || isUnk12(var) || isInt(var) || isUint(var)) {
        return var->userData.asInt;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asInt;
}

bool isUnk6(Variable* var) {
    // make sure var is not a UserVar
    while (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isUnk6(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_UNK_6;
}

unsigned int getUint(Variable* var) {
    if (var->status == VARIABLE_UNINITIALIZED) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asUint;
    }

    // make sure var is not a UserVar
    if (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return getUint(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }
    
    if (isUint(var)) {
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

    if (!func_002a9a74(var) && !isFunc(var) && !isUnk9(var) && !isUnk12(var)) {
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }
    
    return var->userData.asUint;
}

bool isUint(Variable* var) {
    // make sure var is not a UserVar
    while (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isUint(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_UINT;
}

// not matching: compiler version
float getFloat(Variable* var) {
    if (var->status == VARIABLE_UNINITIALIZED) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asFloat;
    }

    // make sure var is not a UserVar
    while (var->status == VARIABLE_USER_VAR) {
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

bool isFloat(Variable* var) {
    // make sure var is not a UserVar
    while (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isFloat(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_FLOAT;
}

int getInt(Variable* var) {
    if (var->status == VARIABLE_UNINITIALIZED) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }

    // make sure var is not a UserVar
    if (var->status == VARIABLE_USER_VAR) {
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
    
    if (isUint(var)) {
        return var->userData.asInt;
    }

    switch (var->status) {
        case VARIABLE_UNK_6:
            return var->userData.asInt;
        case VARIABLE_USER_VAR:
            if (getReferenceType(var) != nullptr) {
                if (isUnk6(getReferenceType(var))) {
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
    
    if (!func_002a9a74(var) && !isFunc(var) && !isUnk9(var) && !isUnk12(var)) {
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }
    
    return var->userData.asInt;
}

bool isInt(Variable* var) {
    // make sure var is not a UserVar
    if (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isInt(getReferenceType(var));
    }

    return var->status == VARIABLE_INT;
}

