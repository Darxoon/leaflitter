#include "variable_numbers.h"

Variable* getReferenceType(Variable* variable);
void logScriptError(int dst, const char* msg, ...);
bool isSavedVar(Variable* variable);
int func_0029e33c(Variable* variable);
bool func_002a9a74(Variable* variable);
bool isFunc(Variable* variable);
bool isAllocVar(Variable* variable);
UNK_TYPE func_0029e9b4(Variable* variable);

int KSM::tableValue(Variable* var) {
    if (var->dataType == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }

    // make sure var is not a reference
    if (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return tableValue(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isTableValue(var)) {
        return var->userData.asInt;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asInt;
}

bool __attribute__((noinline)) KSM::isTableValue(Variable* var) {
    // make sure var is not a reference
    while (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isTableValue(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->dataType == VARIABLE_TABLE;
}

bool __attribute__((noinline)) KSM::isUserFunction(Variable* var) {
    // make sure var is not a reference
    while (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isUserFunction(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->dataType == VARIABLE_FUNC2;
}

// not matching: unimplemented
void KSM::setBooleanValu(Variable* var, bool value) {
    
}

bool KSM::getBooleanValue(Variable* var) {
    if (var->dataType == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asBool;
    }

    // make sure var is not a reference
    if (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return getBooleanValue(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (hasBooleanValue(var)) {
        return var->userData.asBool;
    }
    
    if (hasIntVal(var)) {
        return var->userData.asInt;
    }
    
    if (hasFloat(var)) {
        return var->userData.asFloat;
    }
    
    if (isHexV(var)) {
        return var->userData.asInt;
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asBool;
}

bool __attribute__((noinline)) KSM::hasBooleanValue(Variable* var) {
    // make sure var is not a reference
    while (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return hasBooleanValue(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->dataType == VARIABLE_BOOL;
}

// not matching: unimplemented
void KSM::stPtr(Variable* var, UNK_TYPE value) {
    
}

UNK_TYPE KSM::getPtr(Variable* var) {
    if (var->dataType == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }

    // make sure var is not a reference
    if (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return getPtr(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isAllocVar(var)) {
        return func_0029e9b4(var);
    }

    switch (var->dataType) {
        case VARIABLE_PTR:
            return var->userData.asInt;
        case VARIABLE_REF:
            if (getReferenceType(var) != nullptr) {
                if (hasPtr(getReferenceType(var))) {
                    return var->userData.asInt;
                }
            }
    }
    
    if (isFunc(var) || hasBooleanValue(var) || isTableValue(var) || hasIntVal(var) || isHexV(var)) {
        return var->userData.asInt;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asInt;
}

bool __attribute__((noinline)) KSM::hasPtr(Variable* var) {
    // make sure var is not a reference
    while (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return hasPtr(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->dataType == VARIABLE_PTR;
}

unsigned int KSM::hexVal(Variable* var) {
    if (var->dataType == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asUint;
    }

    // make sure var is not a reference
    if (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return hexVal(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }
    
    if (isHexV(var)) {
        return var->userData.asUint;
    }
    
    if (hasFloat(var)) {
        return var->userData.asFloat;
    }
    
    if (hasIntVal(var)) {
        return var->userData.asInt;
    }
    
    if (hasBooleanValue(var)) {
        return var->userData.asByte ? 1 : 0;
    }

    if (!func_002a9a74(var) && !isFunc(var) && !isUserFunction(var) && !isTableValue(var)) {
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }
    
    return var->userData.asUint;
}

bool __attribute__((noinline)) KSM::isHexV(Variable* var) {
    // make sure var is not a reference
    while (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isHexV(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->dataType == VARIABLE_HEX;
}

// not matching: compiler version
float KSM::floatVal(Variable* var) {
    if (var->dataType == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asFloat;
    }

    // make sure var is not a reference
    while (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return floatVal(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }
    
    if (hasFloat(var)) {
        return var->userData.asFloat;
    }
    
    if (hasIntVal(var)) {
        return var->userData.asInt;
    }

    if (hasBooleanValue(var)) {
        return var->userData.asByte ? 1.0f : 0.0f;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asFloat;
}

bool __attribute__((noinline)) KSM::hasFloat(Variable* var) {
    // make sure var is not a reference
    while (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return hasFloat(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->dataType == VARIABLE_FLOAT;
}

int KSM::getIntVal(Variable* var) {
    if (var->dataType == VARIABLE_NOINIT) {
        /* A variable is used without being initialized. */
        logScriptError(0, "変数が未初期化で使用されています。\n", -1);
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }

    // make sure var is not a reference
    if (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            /* The reference variable does not have a value. */
            logScriptError(0, "参照型変数に値が入っていません。\n", -1);
        }

        return getIntVal(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }

    if (hasIntVal(var)) {
        return var->userData.asInt;
    }
    
    if (hasFloat(var)) {
        return (int)var->userData.asFloat;
    }
    
    if (isHexV(var)) {
        return var->userData.asInt;
    }

    switch (var->dataType) {
        case VARIABLE_PTR:
            return var->userData.asInt;
        case VARIABLE_REF:
            if (getReferenceType(var) != nullptr) {
                if (hasPtr(getReferenceType(var))) {
                    return var->userData.asInt;
                }
            }
    }

    if (hasBooleanValue(var)) {
        int result = var->userData.asUByte;
        if (result != 0)
            result = 1;
        return result;
    }
    
    if (!func_002a9a74(var) && !isFunc(var) && !isUserFunction(var) && !isTableValue(var)) {
        /* Cannot convert variable type! */
        logScriptError(0, "変数の型変換できません！\n", -1);
        return var->userData.asInt;
    }
    
    return var->userData.asInt;
}

bool __attribute__((noinline)) KSM::hasIntVal(Variable* var) {
    // make sure var is not a reference
    if (var->dataType == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return hasIntVal(getReferenceType(var));
    }

    return var->dataType == VARIABLE_INT;
}

