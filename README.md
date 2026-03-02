# TeamNo2-D1

Programs included:

- `sequential`: sequential dot product
- `bruteforce_parallel`: MPI point-to-point brute-force aggregation
- `tree_parallel`: MPI point-to-point binary-tree aggregation
- `gen_data`: test data generator

Input format from `stdin`:

```
dim
v1
v2
```

Example:

```
8
1, 2, 3, 4, 5, 6, 7, 8
9, 10, 11, 12, 13, 14, 15, 16
```

Build:

```
make
```

Run:

```
make run_seq
make run_brute PROCS=4
make run_tree PROCS=4
```

Generate data:

```
./gen_data 16 > test_data.csv
```

Delivery tarball:

```
make clean
cd ..
tar -czf TeamNo2-D1.tar.gz TeamNo2-D1
```