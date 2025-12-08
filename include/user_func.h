#pragma once

struct UserFunc {
    char* name;
    int field1_0x4;
    int (*fn_ptr)(void* runtime);
    int field3_0xc;
    UserFunc* next;
};

namespace KSM {
    extern UserFunc* allUserFuncs[512];
    
    UserFunc* GetUserFunc(const char* name);
}
