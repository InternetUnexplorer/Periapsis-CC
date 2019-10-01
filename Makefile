CC_FLAGS    := $(CC_FLAGS) -Wall -g
FLEX_FLAGS  := $(FLEX_FLAGS)
BISON_FLAGS := $(BISON_FLAGS)

SRC_FILES = main.c util/alloc.c util/ll.c util/vec.c ast.c lexer.c parser.c
OBJ_FILES = $(SRC_FILES:.c=.o)
DEP_FILES = $(SRC_FILES:.c=.d)

.PHONY: clean

main: $(OBJ_FILES)
	$(strip $(CC) $(CC_FLAGS) $^ -o $@)

main.o: main.c parser.h

lexer.o: parser.h

%.o: %.c
	$(strip $(CC) $(CC_FLAGS) -MMD -MP -c $< -o $@)

lexer.h: lexer.c
lexer.c: lexer.l
	$(strip flex $(FLEX_FLAGS) $<)

parser.h: parser.c
parser.c: parser.y lexer.h
	$(strip bison $(BISON_FLAGS) $<)

clean:
	rm -f main lexer.c lexer.h parser.c parser.h *.o */*.o *.d */*.d

-include $(DEP_FILES)
