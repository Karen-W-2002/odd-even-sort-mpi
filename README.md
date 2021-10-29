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

### Analysis on the results


### My thoughts

