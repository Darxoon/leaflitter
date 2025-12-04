CXX=wibo ${ARMCC_4_1_BIN}/armcc.exe
CXXFLAGS=--cpu=MPCore --cpp --arm --split_sections --debug --no_debug_macros --gnu \
		--debug_info=line_inlining_extensions -O3 -Otime --data_reorder --signed_chars \
		--multibyte_chars --remove_unneeded_entities --force_new_nothrow --remarks --no_rtti \
		-Iinclude

LD=wibo ${ARMCC_4_1_BIN}/armlink.exe
LDFLAGS=--unresolved=_Z4stubv

ASM=wibo ${ARMCC_4_1_BIN}/armasm.exe
ASMFLAGS=--cpu=MPCore

DEPS := $(shell find include -name '*.h')

CPPFILES := $(shell find src -name '*.cpp')
OFILES := $(subst src,build/obj,$(CPPFILES:.cpp=.o))

SPLIT_TXTS :=$(shell find split -name '*.txt')
ORIG_OBJS :=$(subst split,build/orig,$(SPLIT_TXTS:.txt=.o))

# Print compiler info
ifneq (${ARMCC_4_1_BIN},)
COMPILED_TARGETS=build/out $(ORIG_OBJS)

$(info $(shell wibo ${ARMCC_4_1_BIN}/armcc.exe --help | head -n 1))
$(info $(shell wibo ${ARMCC_4_1_BIN}/armlink.exe --help | head -n 1))
$(info $(shell wibo ${ARMCC_4_1_BIN}/armasm.exe --help | head -n 1))
$(info )
else
$(info No compiler found, only disassembling.)
$(info Pass in the armcc 'win-x86_64' directory through the $$ARMCC_4_1_BIN environment varible)
$(info to enable compilation.)
$(info )
endif

all: asm/disasm.s $(COMPILED_TARGETS) objdiff.json

.PHONY: disassemble
disassemble: asm/disasm.s

# Disassembly
code.bin:
	@echo "Missing code.bin file in the root of this project's folder!"
	@echo "Please follow the setup instructions first."
	@exit 1

asm/disasm.s: stickerstar.cfg code.bin
	@mkdir -p $(@D)
	@echo Disassembling game binary...
	n3dsdisasm code.bin -c stickerstar.cfg > asm/disasm.s

# Source code
build/obj/%.o: src/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

build/out: $(OFILES)
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) --output $@ $(OFILES) 

# Target objects (for objdiff)
.PRECIOUS: build/orig/%.s

build/orig/%.s: split/%.txt asm/disasm.s
	@mkdir -p $(@D)
	python3 tools/gen_armasm_target.py $@ $<

build/orig/%.o: build/orig/%.s
	$(ASM) $(ASMFLAGS) -o $@ $<

objdiff.json: $(SPLIT_TXTS)
	python3 tools/configure_objdiff.py

# Misc
.PHONY: clean
clean:
	@echo Cleaning...
	@rm -rf build
	@rm -f objdiff.json

