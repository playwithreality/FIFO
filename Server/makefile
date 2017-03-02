CC=gcc
CFLAGS = -Wall -pedantic
FILES = server.c serverfiles.c serverfiles.h
OUT_EXE = server
build:	$(FILES)
	$(CC) $(CFLAGS) $(FILES) -pthread -o $(OUT_EXE)

run: server
	./server
