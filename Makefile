CC=cc
LD=cc
CFLAGS=-c -Wall -Wextra -pedantic -std=c11
LDFLAGS=-o
CCOMPILE=cd build; ${CC} ${CFLAGS} ../

tests/test_char_stream: build/char_stream.o build/test_lib.o build/test_char_stream.o
	${LD} -g ${LDFLAGS} $@ $^
	cd tests; ./test_char_stream

build/test_char_stream.o: tests/src/test_char_stream.c
	${CCOMPILE}$<

build/preprocessor.o: src/preprocessor/preprocessor.c src/preprocessor/preprocessor.h
	${CCOMPILE}$<

build/input_file_stack.o: src/preprocessor/input_file_stack/input_file_stack.c src/preprocessor/input_file_stack/input_file_stack.h
	${CCOMPILE}$<

build/char_stream.o: src/preprocessor/input_file_stack/char_stream/char_stream.c src/preprocessor/input_file_stack/char_stream/char_stream.h
	${CCOMPILE}$<

build/test_lib.o: tests/test_lib/test_lib.c tests/test_lib/test_lib.h
	${CCOMPILE}$<
