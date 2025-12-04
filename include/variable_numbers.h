#pragma once

#include <ctx.h>
#include <variable.h>

namespace KSM {
    UNK_TYPE tableValue(Variable* var);
    bool isTableValue(Variable* var);
    // TODO: i have no idea if this name is correct, i jsut need to get the function ordering lmao
    bool isUserFunction(Variable* var);
    void setBooleanValu(Variable* var, bool value);
    bool getBooleanValue(Variable* var);
    bool hasBooleanValue(Variable* var);
    void stPtr(Variable* var, UNK_TYPE value);
    UNK_TYPE getPtr(Variable* var);
    bool hasPtr(Variable* var);
    unsigned int hexVal(Variable* var);
    bool isHexV(Variable* var);
    float floatVal(Variable* var);
    bool hasFloat(Variable* var);
    int getIntVal(Variable* var);
    bool hasIntVal(Variable* var);
}

