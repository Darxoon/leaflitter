#pragma once

#include <ctx.h>
#include <variable.h>

UNK_TYPE getUnk12(Variable* var);
bool isUnk12(Variable* var);
bool isUnk9(Variable* var);
void setBool(Variable* var, bool value);
bool getBool(Variable* var);
bool isBool(Variable* var);
void setUnk6(Variable* var, UNK_TYPE value);
UNK_TYPE getUnk6(Variable* var);
bool isUnk6(Variable* var);
unsigned int getUint(Variable* var);
bool isUint(Variable* var);
float getFloat(Variable* var);
bool isFloat(Variable* var);
int getInt(Variable* var);
bool isInt(Variable* var);

