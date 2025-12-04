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
OFILES := $(subst src, build/obj, $(CPPFILES:.cpp=.o))

SPLIT_TXTS :=$(shell find split -name '*.txt')
ORIG_OBJS :=$(subst split, build/orig, $(CPPFILES:.txt=.o))

# Print compiler info
$(info $(shell wibo ${ARMCC_4_1_BIN}/armcc.exe --help | head -n 1))
$(info $(shell wibo ${ARMCC_4_1_BIN}/armlink.exe --help | head -n 1))
$(info $(shell wibo ${ARMCC_4_1_BIN}/armasm.exe --help | head -n 1))
$(info )

all: build/out objdiff.json $(ORIG_OBJS)

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

