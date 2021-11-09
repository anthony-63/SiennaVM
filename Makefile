CC = gcc
CFLAGS = -o
SRC = ./src/*.c
OUT = ./bin/sienna.exe

all:
	$(CC) $(CFLAGS) $(OUT) $(SRC)
	$(OUT)
