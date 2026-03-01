# hola2
CC		= mpicc
CFLAGS		= -Wall

PROGRAM 	= bruteforce_parallel 

PROGRAMS 	= sequential bruteforce_parallel

PROCS		= 4

#
# Targets, in order of appearance
#

all: $(PROGRAMS)

sequential: sequential.c
	$(CC) $(CFLAGS) -o sequential sequential.c

bruteforce_parallel: bruteforce_prallel.c
	$(CC) $(CFLAGS) -o $(PROGRAM) bruteforce_prallel.c

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
