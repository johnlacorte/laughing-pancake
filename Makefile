CC=cc
LD=cc
CFLAGS=-c -Wall -Wextra -pedantic -std=c11
LDFLAGS=-o
CCOMPILE=cd build; ${CC} ${CFLAGS} ../

bin/wb: tests/test_utf8_encoder tests/test_utf8_file tests/test_preprocessor
	touch bin/wb

tests/test_preprocessor: build/preprocessor.o build/test_lib.o build/utf8_encoder.o build/utf8_file.o build/test_preprocessor.o
	${LD} -g ${LDFLAGS} $@ $^
	cd tests; ./test_preprocessor

tests/test_utf8_encoder: build/utf8_encoder.o build/test_lib.o build/test_utf8_encoder.o
	${LD} -g ${LDFLAGS} $@ $^
	cd tests; ./test_utf8_encoder

tests/test_utf8_file: build/utf8_file.o build/test_lib.o build/test_utf8_file.o
	${LD} -g ${LDFLAGS} $@ $^
	cd tests; ./test_utf8_file

build/test_preprocessor.o: tests/src/test_preprocessor.c
	${CCOMPILE}$<

build/test_utf8_encoder.o: tests/src/test_utf8_encoder.c
	${CCOMPILE}$<

build/test_utf8_file.o: tests/src/test_utf8_file.c
	${CCOMPILE}$<

build/preprocessor.o: src/preprocessor/preprocessor.c src/preprocessor/preprocessor.h
	${CCOMPILE}$<

build/utf8_encoder.o: src/preprocessor/utf8_encoder/utf8_encoder.c src/preprocessor/utf8_encoder/utf8_encoder.h
	${CCOMPILE}$<

build/utf8_file.o: src/preprocessor/utf8_file/utf8_file.c src/preprocessor/utf8_file/utf8_file.h
	${CCOMPILE}$<

build/test_lib.o: tests/test_lib/test_lib.c tests/test_lib/test_lib.h
	${CCOMPILE}$<
