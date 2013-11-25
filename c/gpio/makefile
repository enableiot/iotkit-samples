# First define which compiler to use...
CC=gcc
CFLAGS=-c 

all: gpio_demo

gpio_demo: gpio_demo.o gpio.o
	$(CC) -m32 gpio_demo.o gpio.o -o gpio_demo

gpio_demo.o: gpio_demo.c
	$(CC) -m32 $(CFLAGS)  gpio_demo.c

gpio.o: gpio.c
	$(CC) -m32 $(CFLAGS) gpio.c

.PHONY: clean
clean: 
	rm -rf *.o gpio_demo

debug: CFLAGS += -Ddebug -Wall 
debug: clean
debug: all
