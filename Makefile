CC=cc
LD=cc
CFLAGS=-c -Wall -Wextra -pedantic -std=c11
LDFLAGS=-o
CCOMPILE=cd build; ${CC} ${CFLAGS} ../

bin/wb: tests/test_input_file_list tests/test_char_stream
	touch bin/wb

tests/test_input_file_list: build/char_stream.o build/input_file_list.o build/test_lib.o build/test_input_file_list.o
	${LD} -g ${LDFLAGS} $@ $^
	cd tests; ./test_input_file_list

tests/test_char_stream: build/char_stream.o build/test_lib.o build/test_char_stream.o
	${LD} -g ${LDFLAGS} $@ $^
	cd tests; ./test_char_stream

build/test_input_file_list.o: tests/src/test_input_file_list.c
	${CCOMPILE}$<

build/test_char_stream.o: tests/src/test_char_stream.c
	${CCOMPILE}$<

build/preprocessor.o: src/preprocessor/preprocessor.c src/preprocessor/preprocessor.h
	${CCOMPILE}$<

build/input_file_list.o: src/preprocessor/input_file_list/input_file_list.c src/preprocessor/input_file_list/input_file_list.h
	cd build; ${CC} -c -Wall -Wextra -pedantic -D_BSD_SOURCE ../$<

build/char_stream.o: src/preprocessor/input_file_list/char_stream/char_stream.c src/preprocessor/input_file_list/char_stream/char_stream.h
	${CCOMPILE}$<

build/test_lib.o: tests/test_lib/test_lib.c tests/test_lib/test_lib.h
	${CCOMPILE}$<
