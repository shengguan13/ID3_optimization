CC=gcc
CFLAGS=-lm

all:
	$(CC) $(CFLAGS) -mavx2 -fopenmp -o Digger digger.h digger.c load.c plant.c predict.c navigate.c
clean:
	rm -rf Digger Test Rules Timing_parallel
