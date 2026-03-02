CC      = mpicc
CFLAGS  = -Wall -Wextra -O2

PROCS   = 4
DATA    = test_data.csv

PROGRAMS = sequential bruteforce_parallel tree_parallel gen_data

all: $(PROGRAMS)

dot_product.o: dot_product.c dot_product.h
	$(CC) $(CFLAGS) -c dot_product.c

sequential: sequential.c dot_product.o
	$(CC) $(CFLAGS) -o sequential sequential.c dot_product.o

bruteforce_parallel: bruteforce_parallel.c dot_product.o
	$(CC) $(CFLAGS) -o bruteforce_parallel bruteforce_parallel.c dot_product.o

tree_parallel: tree_parallel.c dot_product.o
	$(CC) $(CFLAGS) -o tree_parallel tree_parallel.c dot_product.o

gen_data: gen_data.c
	$(CC) $(CFLAGS) -o gen_data gen_data.c

run_seq: sequential
	./sequential < $(DATA)

run_brute: bruteforce_parallel
	mpiexec -n $(PROCS) ./bruteforce_parallel < $(DATA)

run_tree: tree_parallel
	mpiexec -n $(PROCS) ./tree_parallel < $(DATA)

clean:
	rm -f *% *~ core *.o $(PROGRAMS)
