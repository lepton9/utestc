SRC := ./src
BIN := ./bin
OBJS := ./objs
INC := -I ./include
FLAGS := -c $(INC)
LINK := -lm -lncurses
CC := gcc

TESTS := ./tests
TEST_TARGETS := testLib_test

OBJ := ./objs/*

#Testing
test: all_tests
	@for target in $(TEST_TARGETS); do \
		$(TESTS)/bin/$$target; \
	done

all_tests: $(addprefix $(TESTS)/bin/, $(TEST_TARGETS))

$(TESTS)/bin/%_test: utestC.c $(TESTS)/%_test.c #$(OBJ)
	$(CC) -I utestC.h $^ $(LINK) -g -o $@

clean:
	rm -rf $(OBJS)/*.o $(BIN)/*
	rm -rf $(TESTS)/bin/*

run:
	$(BIN)/main

