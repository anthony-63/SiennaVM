CC = gcc
CFLAGS = -o
SRCVM = ./src/vm/*.c
SRCASM = ./src/assembler/*.c
OUTVM = ./bin/sienna
OUTASM = ./bin/siennac

vm:
	@echo ----- Building VM -----
	$(CC) $(CFLAGS) $(OUTVM).exe $(SRCVM)
	$(OUTVM).exe
debugvm:
	@echo ----- Builing VM with symbols -----
	$(CC) -g $(CFLAGS) $(OUTVM)dbg.exe $(SRCVM)
	gdb $(OUTVM).exe
asm: 
	@echo ----- Building Assembler -----
	$(CC) $(CFLAGS) $(OUTASM).exe $(SRCASM)
	$(OUTASM).exe ./programs/main.svm ./programs/main.sbin
debugasm:
	@echo ----- Builing Assembler with symbols -----
	$(CC) -g $(CFLAGS) $(OUTASM)dbg.exe $(SRCASM)
	gdb $(OUTASM)dbg.exe
stacktest:
	@echo ----- Building stack test -----
	$(CC) -g $(CFLAGS) ./bin/stackt.exe ./src/vm/stackt.c ./src/vm/stack.c
	./bin/stackt.exe