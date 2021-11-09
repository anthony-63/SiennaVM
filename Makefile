CC = gcc
CFLAGS = -o
SRC = ./src/*.c
OUT = ./bin/sienna

all:
	$(CC) $(CFLAGS) $(OUT).exe $(SRC)
	$(OUT).exe
debug:
	$(CC) -g $(CFLAGS) $(OUT)dbg.exe $(SRC)
	gdb $(OUT).exe
