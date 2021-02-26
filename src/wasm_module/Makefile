wb: fileOutput.o importSection.o elementSection.o dataSection.o startSection.o tableSection.o memorySection.o codeSection.o exportSection.o functionSection.o globalSection.o typeSection.o byteBuffer.o wasmModule.o main.o
	gcc -o wb fileOutput.o importSection.o elementSection.o dataSection.o startSection.o tableSection.o memorySection.o codeSection.o exportSection.o functionSection.o globalSection.o typeSection.o wasmModule.o byteBuffer.o main.o

main.o: main.c
	gcc -c main.c

byteBuffer.o: byteBuffer.c byteBuffer.h
	gcc -c byteBuffer.c

importSection.o: importSection.c importSection.h
	gcc -c importSection.c

elementSection.o: elementSection.c elementSection.h
	gcc -c elementSection.c

dataSection.o: dataSection.c dataSection.h
	gcc -c dataSection.c

startSection.o: startSection.c startSection.h
	gcc -c startSection.c

tableSection.o: tableSection.c tableSection.h
	gcc -c tableSection.c

memorySection.o: memorySection.c memorySection.h
	gcc -c memorySection.c

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
