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

%.o: %.c
	$(strip $(CC) $(CC_FLAGS) -MMD -MP -c $< -o $@)

lexer.c lexer.h: lexer.l
	$(strip flex $(FLEX_FLAGS) $<)

parser.c parser.h: parser.y ast.h lexer.h
	$(strip bison $(BISON_FLAGS) $<)

clean:
	rm -f main lexer.c lexer.h parser.c parser.h *.o */*.o *.d */*.d

-include $(DEP_FILES)
