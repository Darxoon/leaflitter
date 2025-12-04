#include "runtime.h"
#include "variable.h"

Variable* searchVariableID(u32* code, Runtime* runtime, int unk0, int unk1);
char* getString(Variable* var);
void loadKSMFile(char* filename);

int cmd_LoadKSM(Runtime* runtime) {
    runtime->code++;
    Variable* var = searchVariableID(runtime->code++, runtime, 0, 1);
    char* filename = getString(var);
    loadKSMFile(filename);
    return 1;
}
