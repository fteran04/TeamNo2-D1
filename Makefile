# hola2
CC		= mpicc
CFLAGS		= -Wall

PROGRAM 	= dot_product_sequential 

PROGRAMS 	= dot_product_parallel

PROCS		= 4

#
# Targets, in order of appearance
#

all: $(PROGRAMS)

dot_product_sequential: dot_product_sequential.c
	gcc -Wall -o dot_product_sequential dot_product_sequential.c

#
# Other nice thingys
#

clean:
	rm -f *% *~ core *.o $(PROGRAMS)

run:
	@echo "Will run $(PROGRAM) using $(PROCS) processes:"; mpiexec -n $(PROCS) $(PROGRAM)

auto_run:
	@clear; echo -e "Will run $(PROGRAM) using $$((`getconf _NPROCESSORS_ONLN` / 2)) cores on host `hostname`:\n`lscpu` "; mpiexec -n $$((`getconf _NPROCESSORS_ONLN` / 2)) $(PROGRAM)

auto_run_threads:
	@clear; echo -e "Will run $(PROGRAM) using `getconf _NPROCESSORS_ONLN` hardware threads on host `hostname`:\n`lscpu` "; mpiexec -n `getconf _NPROCESSORS_ONLN` --use-hwthread-cpus $(PROGRAM)
