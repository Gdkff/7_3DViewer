.PHONY: all install run_app uninstall clean dvi dist tests gcov_report check

LIBS_ADDITIONAL = 
RUN_APP_PATH = /3DViewer_v1_0.app/Contents/MacOS/3DViewer_v1_0
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
	RUN_APP_PATH = 3DViewer_v1_0
	endif
	
	ifeq ($(detected_Linux), Debian)
	LIBS_ADDITIONAL = -lm -lsubunit
	endif
	
endif

CC	=	gcc 
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
SOURCES = *.c
OBJECTS = *.o
TESTEXEC = test_run
BUILD_DIR = build/


all: install run_app

install:
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && qmake ../qtProject
	make -C ./$(BUILD_DIR)

run_app:
	./$(BUILD_DIR)$(RUN_APP_PATH)

uninstall:
	rm -Rf build/

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
	$(CC) $(CFLAGS) -c tests/test_main.c
	$(CC) $(CFLAGS) test_main.o --coverage $(SOURCES) -o $(TESTEXEC) -lcheck $(LIBS_ADDITIONAL)
	./$(TESTEXEC)


gcov_report: test
	gcovr -b
	gcovr
	gcovr --html-details -o tests/report.html
	open tests/report.html
	find . -name '*.gcno' -type f -delete
	find . -name '*.gcda' -type f -delete

check:
	cppcheck --enable=all --force --check-config for details *.c *.h
