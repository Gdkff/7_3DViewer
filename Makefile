CC	=	gcc 
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
SOURCES = *.c
OBJECTS = *.o
TESTEXEC = test_run

BUILD_DIR = build

# ALL_PROJECT = *.c *.h qt_project/*.cpp qt_project/*.h tests/*.c tests/*.h


LIBS_ADDITIONAL = 
ifeq ($(OS), Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname -s)
endif

ifeq ($(detected_OS), Linux)
	detected_Linux := $(shell cat /etc/issue | sed -n '1p' | awk '{print $$1}')
	ifeq ($(detected_Linux), Arch)
	LIBS_ADDITIONAL = -lm
	endif

	ifeq ($(detected_Linux), Ubuntu)
	LIBS_ADDITIONAL = -lm -lsubunit
	BUILD_DIR = build/
	endif
	
	ifeq ($(detected_Linux), Debian)
	LIBS_ADDITIONAL = -lm -lsubunit
	endif
	
endif




all: install run_app

install:
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && qmake ../qtProject
	make -C ./$(BUILD_DIR)

run_app:
	./$(BUILD_DIR)/3DViewer_v1_0.app/Contents/MacOS/3DViewer_v1_0

uninstall:
	rm -Rf build/
#		rm -rf Calc.tar

clean:
	rm -Rf html/
	rm -rf tests/*.html tests/*.css
	rm -f $(TESTEXEC) SmartCalc.tar .gitkeep .DS_Store qt_project/.qmake.stash
	find . -name '*.gcno' -type f -delete
	find . -name '*.gcda' -type f -delete
	find . -name '*.gcov' -type f -delete
	rm -rf *.o .qmake.stash .clang-format
	rm -Rf qt_project/.qtc_clangd/
	rm -Rf qt_project/.clang-format
	rm -rf *Debug *.dSYM *.tar *on_delete .vscode
	
dvi:
		doxygen Doxyfile
		open html/index.html

dist: uninstall clean
	tar -cf ./3DViewer_v1_0.tar * 

tests: 
#find . -name '*.gcda' -type f -delete
	$(CC) $(CFLAGS) -c tests/test_main.c
	$(CC) $(CFLAGS) test_main.o --coverage $(SOURCES) -o $(TESTEXEC) -lcheck $(LIBS_ADDITIONAL)
	./$(TESTEXEC)
#gcov tests/tests-c_calc_polish.gcda

gcov_report: test
	gcovr -b
	gcovr
	gcovr --html-details -o tests/report.html
	open tests/report.html
	find . -name '*.gcno' -type f -delete
	find . -name '*.gcda' -type f -delete

check:
	cppcheck --enable=all --force --check-config for details *.c *.h
#cp ../../materials/linters/.clang-format .clang-format
	clang-format -n *.c

# check: test
# 	cppcheck --enable=all --force --check-config for details *.c *.h
# 	cp ../materials/linters/.clang-format .clang-format
# 	cp ../materials/linters/.clang-format qt_project/.clang-format
# 	clang-format -n $(ALL_PROJECT)
