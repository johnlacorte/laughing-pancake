CC=cc
LD=cc
CFLAGS=-c -Wall -Wextra -pedantic -std=c11
LDFLAGS=-o
CCOMPILE=cd build; ${CC} ${CFLAGS} ../

tests/test_char_stream: build/char_stream.o build/test_lib.o build/test_char_stream.o
	${LD} -g ${LDFLAGS} $@ $^
	./tests/test_char_stream

build/test_char_stream.o: tests/src/test_char_stream.c
	${CCOMPILE}$<

build/char_stream.o: src/token_stream/preprocessor/char_stream/char_stream.c src/token_stream/preprocessor/char_stream/char_stream.h
	${CCOMPILE}$<

build/test_lib.o: tests/test_lib/test_lib.c tests/test_lib/test_lib.h
	${CCOMPILE}$<
