CC = gcc
FLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -std=c11 -g

PROJECT_NAME = tetris
LIB_NAME = tetris
LIB_SRC = $(wildcard brick_game/tetris/*.c)
GUI_SRC = $(wildcard gui/cli/*.c)

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, $(OBJDIR)%.o, $(SOURCES))
TEST = s21_test
TEST_DIR = tests/
RM_EXTS := o a out gcno gcda gcov info html css txt gz

OS := $(shell uname)
MAC_X86 := $(shell uname -a | grep -o _X86_64)
MAC_M1 := $(shell uname -a | grep -o _ARM64)

ifeq ($(OS), Linux)
	TEST_LIBS = -lcheck -pthread -lrt -lm -lsubunit
	MATH = -lm
	OPEN = xdg-open
endif

ifeq ($(MAC_M1), _ARM64)
	TEST_LIBS := -I/opt/homebrew/opt/check/include -L/opt/homebrew/opt/check/lib -lcheck
	OPEN = open
endif
ifeq ($(MAC_X86), _X86_64)
	TEST_LIBS = $(shell pkg-config --libs --cflags check) -lm
	OPEN = open
endif

all: run

run: install
	build/$(PROJECT_NAME)

build: all

repo:
	mkdir -p $(OBJ)
$(OBJDIR)%.o: %.c
	$(CC) $(FLAGS) -c $< -o obj/$@


install: tetris.a gui.o main.o
	mkdir -p build/
	$(CC) *.o -lncurses -o build/$(PROJECT_NAME)
# ln -s build/$(PROJECT_NAME) символическая ссылка на билд
	rm -rf *.o

uninstall:
	rm -rf build/$(PROJECT_NAME)

tetris.a: $(LIB_NAME).o
	ar rcs $(LIB_NAME).a *.o
	ranlib $(LIB_NAME).a

$(LIB_NAME).o:
	$(CC) $(FLAGS) -c $(LIB_SRC) $(DEBUG_FLAGS)

gui.o:
	$(CC) $(FLAGS) -c $(GUI_SRC)

main.o:
	$(CC) $(FLAGS) -c main.c

dvi:
	doxygen Doxyfile
	open html/index.html

dist: clean uninstall
	tar -czf tetris.install.tar.gz ./*
	
test:  tetris.a 
	$(CC) $(FLAGS) tests/*.c $(LIB_SRC) tetris.a $(TEST_LIBS) -o s21_test
	./$(TEST)


brew:
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	reset
	brew install lcov
	brew install check
	sleep 1
	reset
gcov_flag:
	$(eval FLAGS += --coverage)
gcov_report: clean tetris.a
	gcc --coverage -Wall -Wextra -Werror -c brick_game/tetris/backend.c brick_game/tetris/fsm.c -std=c11 -g
	ar rcs tetris.a *.o
	ranlib tetris.a
	gcc -fprofile-arcs -ftest-coverage $(LIB_SRC) $(TEST_DIR)test.c tetris.a -o report.out -lm -lcheck
	./report.out
	mkdir -p report
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory report
	rm -rf *.gcno *.gcda *.gcov *.info
	$(OPEN) report/index.html
    # gcovr --html-details -o report.html --exclude $(TEST_DIR)*.c
    # open report.html

fix:
	cp ../materials/linters/.clang-format ../.clang-format
	clang-format -n *.c *.h tests/*.c tests/*.h test_db/*.c
	clang-format -i *.c *.h tests/*.c tests/*.h test_db/*.c
	sleep 1
	reset
	clang-format -n *.c *.h tests/*.c tests/*.h test_db/*.c

add_db_flag:
	$(eval FLAGS += $(DB))
mem: clean add_db_flag test
ifeq ($(OS),Linux)
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(TEST)
else
	leaks --atExit -- ./$(TEST)
endif

cpp: reset
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction $(LIB_SRC) *.h $(GUI_SRC) *.h tests/*.c tests/*.h

rebuild: clean build
	
clean:
	rm -rf $(TEST) *.o *.a *.gcno *.gcda *.gcov *.info report a.out *.dSYM obj
	@for ext in $(RM_EXTS); do \
	find ./ -type f -name "*.$$ext" -exec rm {} \;; \
	done
	@rm -rf ./report
	@rm -rf ./obj
	@rm -rf dist/
	@rm -rf ./build
	@rm -rf ./latex
	@rm -rf html/
.PHONY: clean
	reset

reset:
	reset | bash

format-check :
	@cp ../materials/linters/.clang-format ./
	@find ./ -type f -name "*.[ch]" -exec clang-format -n {} \;
	@rm ./.clang-format
.PHONY: format-check

format :
	@cp ../materials/linters/.clang-format ./
	@find ./ -type f -name "*.[ch]" -exec clang-format -i {} \;
	@rm ./.clang-format
.PHONY: format

cppcheck :
	@find ./ -type f -name "*.c" -exec cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction {} \;
.PHONY: cppcheck




