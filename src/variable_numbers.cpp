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

int KSM::tableValue(Variable* var) {
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

        return tableValue(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isTableValue(var)) {
        return var->userData.asInt;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, ERROR_MSG_2, -1);
    
    return var->userData.asInt;
}

bool __attribute__((noinline)) KSM::isTableValue(Variable* var) {
    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isTableValue(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_TABLE;
}

bool __attribute__((noinline)) KSM::isUserFunction(Variable* var) {
    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isUserFunction(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_FUNC2;
}

// not matching: unimplemented
void KSM::setBool(Variable* var, bool value) {
    
}

bool KSM::getBooleanValue(Variable* var) {
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

        return getBooleanValue(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (hasBooleanValue(var)) {
        return var->userData.asBool;
    }
    
    if (isIntVal(var)) {
        return var->userData.asInt;
    }
    
    if (isFloat(var)) {
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
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return hasBooleanValue(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_BOOL;
}

// not matching: unimplemented
void KSM::setPtr(Variable* var, UNK_TYPE value) {
    
}

UNK_TYPE KSM::ptr(Variable* var) {
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

        return ptr(getReferenceType(var)); // wow this has tail call optimization
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
    
    if (isFunc(var) || hasBooleanValue(var) || isTableValue(var) || isIntVal(var) || isHexV(var)) {
        return var->userData.asInt;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asInt;
}

bool __attribute__((noinline)) KSM::isPtr(Variable* var) {
    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isPtr(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_PTR;
}

unsigned int KSM::hexVal(Variable* var) {
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

        return hexVal(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }
    
    if (isHexV(var)) {
        return var->userData.asUint;
    }
    
    if (isFloat(var)) {
        return var->userData.asFloat;
    }
    
    if (isIntVal(var)) {
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
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isHexV(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_HEX;
}

// not matching: compiler version
float KSM::floatVl(Variable* var) {
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

        return floatVl(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }
    
    if (isFloat(var)) {
        return var->userData.asFloat;
    }
    
    if (isIntVal(var)) {
        return var->userData.asInt;
    }

    if (hasBooleanValue(var)) {
        return var->userData.asByte ? 1.0f : 0.0f;
    }
    
    /* Cannot convert variable type! */
    logScriptError(0, "変数の型変換できません！\n", -1);
    
    return var->userData.asFloat;
}

bool __attribute__((noinline)) KSM::isFloat(Variable* var) {
    // make sure var is not a reference
    while (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isFloat(getReferenceType(var)); // wow this has tail call optimization
    }

    return var->status == VARIABLE_FLOAT;
}

int KSM::getIntVl(Variable* var) {
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

        return getIntVl(getReferenceType(var)); // wow this has tail call optimization
    }
    
    if (isSavedVar(var)) {
        return func_0029e33c(var);
    }

    if (isIntVal(var)) {
        return var->userData.asInt;
    }
    
    if (isFloat(var)) {
        return (int)var->userData.asFloat;
    }
    
    if (isHexV(var)) {
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

bool __attribute__((noinline)) KSM::isIntVal(Variable* var) {
    // make sure var is not a reference
    if (var->status == VARIABLE_REF) {
        if (getReferenceType(var) == nullptr) {
            return false;
        }

        return isIntVal(getReferenceType(var));
    }

    return var->status == VARIABLE_INT;
}

