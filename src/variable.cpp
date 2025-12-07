#include "variable.h"

void free(void* ptr);
int func_002a9a74(Variable* var);
int func_002a9a24(Variable* var);
int func_002a99d4(Variable* var);
bool isAllocVar(Variable* variable);

void KSM::uninitVariable(Variable* var) {
    if (var->name != 0) {
        free(var->name);
    }

    if (var->dataType != VARIABLE_REF) {
        if (func_002a9a74(var)) {
            if (!func_002a9a24(var) && !func_002a99d4(var) && var->userData.referenceType != 0) {
                free(var->userData.referenceType);
            }
        } else {
            if (isAllocVar(var) && var->userData.referenceType != 0) {
                free(var->userData.referenceType);
            }
        }
    }

    var->name = 0;
    var->handle = 0;
    var->status = 1;
    var->userData.referenceType = 0;
    var->field_0xc = 0;
    var->next = 0;
}

