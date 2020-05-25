CFLAGS = -Wall

CC = g++
LD = g++

SIM_C = main.cc pic.cc

SIM_O = main.o pic.o

PROGRAM = main

$(PROGRAM):$(SIM_O)
	$(LD) $^ -o $@

$(SIM_O):$(SIM_C)
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o
