CXX=wibo ${ARMCC_4_1_BIN}/armcc.exe
CXXFLAGS=--cpp --arm --split_sections --debug --no_debug_macros --gnu \
		--debug_info=line_inlining_extensions -O3 -Otime --data_reorder --signed_chars \
		--multibyte_chars --remove_unneeded_entities --force_new_nothrow --remarks --no_rtti \
		-Iinclude

LD=wibo ${ARMCC_4_1_BIN}/armlink.exe
LDFLAGS=--unresolved=_Z4stubv

DEPS=$(foreach dir,include,$(wildcard $(dir)/*.h))

CPPFILES := $(foreach dir,src,$(wildcard $(dir)/*.cpp))
OFILES=$(subst src, build/obj, $(CPPFILES:.cpp=.o))

# Print compiler info
$(info $(shell wibo ${ARMCC_4_1_BIN}/armcc.exe --help | head -n 1))
$(info $(shell wibo ${ARMCC_4_1_BIN}/armlink.exe --help | head -n 1))
$(info )

build/obj/%.o: src/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

build/out: $(OFILES)
	@mkdir -p $(@D)
	wibo ${ARMCC_4_1_BIN}/armlink.exe $(LDFLAGS) --output $@ $(OFILES) 
