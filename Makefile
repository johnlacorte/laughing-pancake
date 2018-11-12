wb: fileOutput.o codeSection.o exportSection.o functionSection.o globalSection.o typeSection.o wasmModule.o main.o
	gcc -o wb fileOutput.o codeSection.o exportSection.o functionSection.o globalSection.o typeSection.o wasmModule.o main.o

main.o: main.c
	gcc -c main.c

codeSection.o: codeSection.c codeSection.h
	gcc -c codeSection.c

exportSection.o: exportSection.c exportSection.h
	gcc -c exportSection.c

fileOutput.o: fileOutput.c fileOutput.h
	gcc -c fileOutput.c

functionSection.o: functionSection.c functionSection.h
	gcc -c functionSection.c

globalSection.o: globalSection.c globalSection.h
	gcc -c globalSection.c

typeSection.o: typeSection.c typeSection.h
	gcc -c typeSection.c

wasmModule.o: wasmModule.c wasmModule.h
	gcc -c wasmModule.c
