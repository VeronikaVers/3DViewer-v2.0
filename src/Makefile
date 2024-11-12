CC=gcc -std=c++17
CFLAGS=-Wall -Werror -Wextra -lstdc++
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
TEST_PATH= tests/tests.cc

all: install


install: clean uninstall
	@mkdir build
	@cd view && qmake && make && make clean && rm Makefile && cd ../ && mv view/3D_viewer.app build/3D_viewer.app
	pen build/3D_viewer.app

uninstall:
	@rm -rf build

dvi:
	open readme.html

dist:
	mkdir archive
	cp -r build/3D_viewer.app archive
	tar -cvzf archive.tgz archive
	rm -fr archive

clean:
	@rm -rf test 
	@cd view
	@rm -rf *.o *.a *.out 
	@rm -rf *.gcda *.gcno *.info *.gch
	@rm -rf *.a \

clang:
	@cp ../materials/linters/.clang-format ./
	@clang-format -style=Google -n controller/*.h model/*.h model/*.cc view/*.h view/*.cc

clang_correct:
	@clang-format -style=Google -i controller/*.h model/*.h model/*.cc view/*.h view/*.cc