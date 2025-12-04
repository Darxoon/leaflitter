#pragma once

#include <ctx.h>
#include <variable.h>

namespace KSM {
    UNK_TYPE tableValue(Variable* var);
    bool isTableValue(Variable* var);
    // TODO: i have no idea if this name is correct, i jsut need to get the function ordering lmao
    bool isUserFunction(Variable* var);
    void setBool(Variable* var, bool value);
    bool getBooleanValue(Variable* var);
    bool hasBooleanValue(Variable* var);
    void setPtr(Variable* var, UNK_TYPE value);
    UNK_TYPE ptr(Variable* var);
    bool isPtr(Variable* var);
    unsigned int hexVal(Variable* var);
    bool isHexV(Variable* var);
    float floatVl(Variable* var);
    bool isFloat(Variable* var);
    int getIntVl(Variable* var);
    bool isIntVal(Variable* var);
}

