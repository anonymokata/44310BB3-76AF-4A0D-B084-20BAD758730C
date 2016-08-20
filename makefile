CFLAGS=-Wall
LIBS=-lcheck -lm -lpthread -lrt

all: roman-numeral-calculator

roman-numeral-calculator: roman-numeral-calculator.o
	gcc -o roman-numeral-calculator roman-numeral-calculator.o

roman-numeral-calculator.o: roman-numeral-calculator.c roman-numeral-calculator.h
	gcc $(CFLAGS) -c roman-numeral-calculator.c

test: roman-numeral-calculator-test
	./roman-numeral-calculator-test

roman-numeral-calculator-test: roman-numeral-calculator-test.o roman-numeral-calculator.o
	gcc -o roman-numeral-calculator-test roman-numeral-calculator.o roman-numeral-calculator-test.o $(LIBS)

roman-numeral-calculator-test.o: roman-numeral-calculator-test.c roman-numeral-calculator.h
	gcc $(CFLAGS) -c roman-numeral-calculator-test.c

roman-numeral-calculator-test.c: roman-numeral-calculator-test.check
	checkmk roman-numeral-calculator-test.check > roman-numeral-calculator-test.c
