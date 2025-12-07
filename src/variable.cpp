#include "variable.h"

extern "C" {
    void free(void* ptr);
}

bool isAllocVar(Variable* variable);

void KSM::uninitVariable(Variable* var) {
    if (var->name != 0) {
        free(var->name);
    }

    if (var->dataType == VARIABLE_REF) {
        goto cleanup;
    }
    
    if (isStringVar(var)) {
        if (!isStringMeVar(var) && !isEmptyStringVar(var) && var->userData.referenceType != 0) {
            free(var->userData.referenceType);
        }
    } else {
        if (isAllocVar(var) && var->userData.referenceType != 0) {
            free(var->userData.referenceType);
        }
    }

cleanup:
    var->name = 0;
    var->handle = 0;
    var->status = 1;
    var->userData.referenceType = 0;
    var->field_0xc = 0;
    var->next = 0;
}

