#include "user_func.h"

extern "C" {
    int strlen(const char *input_string);
    int strcmp(const char *param_1, const char *name);
}

UserFunc* allUserFuncs[512];

UserFunc* KSM::GetUserFunc(const char* name) {
    int iVar4;
    
    const char* origName = name;
    UserFunc** localUserFuncs = allUserFuncs;
    
    int hashCode = 0;
    unsigned int len = strlen(name);
    const char* iter = name + (len >> 1);
    while (*iter != 0) {
        hashCode += *iter++;
    }
    
    for (UserFunc *func = localUserFuncs[hashCode & 0x1ff]; func != 0; func = func->next) {
        iVar4 = strcmp(func->name, origName);
        if (iVar4 == 0) return func;
    }
    
    return 0;
}
