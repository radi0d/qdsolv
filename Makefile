.PHONY: clean all docs test

VERBOSE ?=

BUILDDIR ?= build/
BUILD = ./$(BUILDDIR)

ifeq ($(strip $(VERBOSE)),)
	ECHO = @echo
	QUIET = @
else
	ECHO = @\#
	QUIET =
endif
export ECHO QUIET

DEDFLAGS = -D _DEBUG -g -ansi -std=c++20 -Wc++0x-compat -Wc++11-compat -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Warray-bounds -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat=2 -Winline -Wlarger-than=8192 -Wlogical-op -Wmissing-declarations -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wnarrowing -Wno-old-style-cast -Wvarargs -fcheck-new -fstack-check -fstack-protector-all -fstrict-overflow -fno-omit-frame-pointer

EPS ?= 1e-9

CFLAGS = $(DEDFLAGS) -Iinclude/ -lm -DEPS=$(EPS) -MMD -MF $@.d

CC ?= g++

OBJ = $(patsubst src/%.cpp, $(BUILD)/%.o, $(filter-out src/main.cpp, $(wildcard src/*.cpp)))

all: qdsolv

$(BUILD)/%.o: src/%.cpp
	$(ECHO) "CC $(notdir $@)"
	$(QUIET) $(CC) -c $(CFLAGS) $< -o $@

qdsolv: $(BUILD)/main.o $(OBJ)
	$(ECHO) "LD $(notdir $@)"
	$(QUIET) $(CC) $(CFLAGS) $^ -o $(BUILD)/$@

docs:
	$(ECHO) "DOCS"
	$(QUIET) doxygen

clean:
	$(ECHO) "CLEAN"
	$(QUIET) rm -rf $(BUILD)/*.o $(BUILD)/*.d $(BUILD)/qdsolv docs

$(BUILD)/test.o: test/test.cpp
	$(ECHO) "CC $(notdir $@)"
	$(QUIET) $(CC) -c $(CFLAGS) $< -o $@

test: $(BUILD)/test.o $(OBJ)
	$(ECHO) "LD $(notdir $@)"
	$(QUIET) $(CC) $(CFLAGS) $^ -o $(BUILD)/$@

-include $(BUILD)/*.d
