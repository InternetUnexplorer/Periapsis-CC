CC_FLAGS    := $(CC_FLAGS) -Wall -g
FLEX_FLAGS  := $(FLEX_FLAGS)
BISON_FLAGS := $(BISON_FLAGS)

C_FILES = main.c util/ll.c ast.c lexer.c parser.c

.PHONY: clean

main: $(C_FILES)
	$(strip $(CC) $(CC_FLAGS) $^ -o $@)

lexer.c: lexer.l
	$(strip flex $(FLEX_FLAGS) $<)

parser.c: parser.y lexer.c
	$(strip bison $(BISON_FLAGS) $<)

clean:
	rm -f main lexer.h lexer.c parser.h parser.c
