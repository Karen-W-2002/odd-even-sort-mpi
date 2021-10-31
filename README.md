# Parallel Odd-Even Sort 
Program written in C with the use of MPI

### Description 
A parallel odd even sort program. Each process uses the sorting algorithm to sort the local lists, then it is gathered by process 0 which then sorts it (global sort) then merges the list.

### Program Functions
- **main(int argc, char \*argv[])**
  - Main function of the program
- **void fillArray(int n, int keys, int *array);**
  - Fills the array with random generated numbers
- **void printArray(int keys, int *array);**
  - Prints the array  
- **void sortOddEvenLocal(int keys, int *array);**
  - Sorts the array locally using odd even sort algorithm
- **void sortOddEvenGlobal(int keys, int my_rank, int comm_sz, int *a, int *b);**
  - Sorts the array globally using odd even sort algorithm
- **int computePartner(int phase, int my_rank, int comm_sz);**
  - Computes the processors partner
- **void clearArray(int keys, int *a);**
  - Clears the array  

### Sections of The Program
The program can be organized into different sections
- **Initialization of the variable and functions**
- **Process 0 reads number of items to sort and broadcasts to other processors"**
- **Local sorting of all the lists**
- **Process 0 gathers and prints the local lists**
- **Global sorting of all the lists**

### List of MPI Functions Used
- MPI_Init()
- MPI_Status()
- MPI_Comm_rank()
- MPI_Comm_Size()
- MPI_Wtime()
- MPI_Send()
- MPI_Recv()
- MPI_Finalize()
- MPI_Barrier
- MPI_Gather()
- MPI_Bcast()

### Compilation
`mpiicc -o odd-even-sort-mpi.out odd-even-sort-mpi.c`
### Execution
`mpiexec -n (# of processors) odd-even-sort-mpi.out`
## Results
![Image of a graph](https://github.com/Karen-W-2002/odd-even-sort-mpi/blob/main/graph.png)

### Analysis on the results
Like every other parallel program, when you change it from no processors to two processor running a code, the change in the program's performance is super dramatic. Then, after that, when you add more processors to run the code, there is still a good amount of positive change to the performance, however it isn't as dramatic as it was with the first one.

There was an interesting observation made that when there is little to be sorted (when the program only needs 1 processor to run it because it is a very small task), the more processors you add to it, the slower it is because of how small the program is and it would take longer to set up all these processors than to actually run the code. This observation was made when n = 16, super super small array to be sorted.

This program was tested with n = 10000, meaning the array had 10000 items to be sorted.
