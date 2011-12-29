PROGRAM = capio
CC = g++
CFLAGS = -Wall -pedantic

all: $(PROGRAM)

$(PROGRAM): capio.cc calculate.o command.o common.o io.o variable.o
	$(CC) $(CFLAGS) -o $@ $^

calculate.o: calculate.cc common.h command.h variable.h io.h
	$(CC) $(CFLAGS) -c $*.cc

command.o: common.cc common.h io.h variable.h calculate.h
	$(CC) $(CFLAGS) -c $*.cc

common.o: common.cc
	$(CC) $(CFLAGS) -c $*.cc

io.o: io.cc common.h
	$(CC) $(CFLAGS) -c $*.cc

variable.o: variable.cc common.h
	$(CC) $(CFLAGS) -c $*.cc

clean:
	rm -f *.o
