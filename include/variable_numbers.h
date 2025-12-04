#pragma once

#include <ctx.h>
#include <variable.h>

UNK_TYPE getTable(Variable* var);
bool isTable(Variable* var);
bool isFunc2(Variable* var);
void setBool(Variable* var, bool value);
bool getBool(Variable* var);
bool isBool(Variable* var);
void setPtr(Variable* var, UNK_TYPE value);
UNK_TYPE getPtr(Variable* var);
bool isPtr(Variable* var);
unsigned int getHex(Variable* var);
bool isHex(Variable* var);
float getFloat(Variable* var);
bool isFloat(Variable* var);
int getInt(Variable* var);
bool isInt(Variable* var);

