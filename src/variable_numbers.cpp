#include "variable_numbers.h"

Variable* getReferenceType(Variable* variable);
void logScriptError(int dst, char* msg, ...);
bool isSavedVar(Variable* variable);
int func_0029e33c(Variable* variable);
bool func_002a9a74(Variable* variable);
bool isFunc(Variable* variable);

// not matching: unimplemented
int getUnk12(Variable* var) {
    return 0;
}

// not matching: unimplemented
bool isUnk12(Variable* var) {
    return false;
}

// not matching: unimplemented
bool isUnk9(Variable* var) {
    return false;
}

// not matching: unimplemented
bool getBool(Variable* var) {
    return false;
}

// not matching: unimplemented
bool isBool(Variable* var) {
    return false;
}

// not matching: unimplemented
int func_002948f4(Variable* var) {
    return 0;
}

// not matching: unimplemented
bool isUnk6(Variable* var) {
    return false;
}

// not matching: unimplemented
unsigned int getUint(Variable* var) {
    return 0;
}

// not matching: unimplemented
bool isUint(Variable* var) {
    return false;
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
            return 0;
        }

        return isInt(getReferenceType(var)); // wow this has tail call optimization
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
    while (var->status == VARIABLE_USER_VAR) {
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
        case 6:
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
    while (var->status == VARIABLE_USER_VAR) {
        if (getReferenceType(var) == nullptr) {
            return 0;
        }

        return isInt(getReferenceType(var));
    }

    return var->status == VARIABLE_INT;
}

