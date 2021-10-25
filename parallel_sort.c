#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void fillArray(int n, int keys, int *array);
void printArray(int keys, int *array);
void sortOddEvenLocal(int keys, int *array);
void sortOddEvenGlobal(int keys, int my_rank, int comm_sz, int *a, int *b);
int computePartner(int phase, int my_rank, int comm_sz);
void clearArray(int keys, int *a);

int main(int argc, char *argv[])
{
    int i, j, n, random;
    int my_rank, comm_sz = 4;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Status status;

    // for random number gen
    srand(getpid() * time(NULL));

    // time start
    double start_time = 0.0, end_time = 0.0, total_time = 0.0;
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Process 0 reads n and broadcasts to others
    if(my_rank == 0)
    {
	n = 16;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int a[n], b[n], *recv_arr;
    int keys = n/comm_sz;

    fillArray(n, keys, a);

    // Local sort
    sortOddEvenLocal(keys, a);
    
    // Process 0 gathers and prints the local lists
    if(my_rank == 0)
    	recv_arr = (int *)malloc(comm_sz * keys * sizeof(int));

    MPI_Gather(a, keys, MPI_INT, recv_arr, keys, MPI_INT, 0, MPI_COMM_WORLD);

    if(my_rank == 0) 
    {
	printf("List sorted locally not globally:\n");   
    	printArray(comm_sz*keys, recv_arr);
    }

    // Global sort
    sortOddEvenGlobal(keys, my_rank, comm_sz, a, b);

    // Process 0 gathers and prints the local lists
    MPI_Gather(a, keys, MPI_INT, recv_arr, keys, MPI_INT, 0, MPI_COMM_WORLD);

    if(my_rank == 0)
    {
	printf("List sorted globally:\n");
	printArray(comm_sz*keys, recv_arr);
    }

    // end time
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();
    if(my_rank == 0) 
    {
    	total_time = end_time - start_time;
    	printf("Total time: %f\n", total_time);
    }

    MPI_Finalize();
    return 0;
}

void fillArray(int n, int keys, int *array)
{
    int i;
    for(i = 0; i < keys; i++) 
        array[i] = rand()%n;
}

void printArray(int keys, int *array)
{
    int i;
    for(i = 0; i < keys; i++)
        printf("%d ", array[i]);
    printf("\n");
}

void sortOddEvenLocal(int keys, int *array)
{
    int temp = 0;
    int i, j;

    //even
    for(i = 0; i < keys; i++)
    {
        //even
        if(i%2 == 0)
        {
            for(j = 1; j < keys; j += 2)
            {
                if(array[j-1] > array[j])
                {
                    temp = array[j];
                    array[j] = array[j-1];
                    array[j-1] = temp;
                }
            }
        // odd
        } else 
        {
            for(j = 1; j < keys - 1; j += 2)
            {
                if(array[j] > array[j+1])
                {
                    temp = array[j];
                    array[j] = array[j+1];
                    array[j+1] = temp;
                }
            }
        }
    }
}

void sortOddEvenGlobal(int keys, int my_rank, int comm_sz, int *a, int *b)
{
    int phase, partner;
    int i, j, temp;
    sortOddEvenLocal(keys, a);

    for(phase = 0; phase < comm_sz; phase++)
    {
	partner = computePartner(phase, my_rank, comm_sz);
	if(partner != -1)
	{
	    // idea here: the higher rank sends, lower rank receives
	    // send keys to partner
	    if(my_rank > partner)
	    {
		MPI_Send(a, keys, MPI_INT, partner, 0, MPI_COMM_WORLD);
	    }
	    else
	    {
		MPI_Recv(b, keys, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    }

	    // sort
	    // keep smaller keys
	    if(my_rank < partner) 
	    {
		for(i = 0; i < keys; i++)
		    for(j = 0; j < keys; j++)
		    	if(a[i] > b[j])
			{
			    temp = b[j];
			    b[j] = a[i];
			    a[i] = temp;
			}			    
		
		sortOddEvenLocal(keys, a);
		sortOddEvenLocal(keys, b);	
		MPI_Send(b, keys, MPI_INT, partner, 0, MPI_COMM_WORLD);
	    }
	    // keep larger keys
	    else
	    {
		clearArray(keys, a);
		MPI_Recv(a, keys, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    }
	}
	MPI_Barrier(MPI_COMM_WORLD);
    }
}

int computePartner(int phase, int my_rank, int comm_sz)
{
    int partner;

    // even phase
    if(phase%2 == 0) 
    {
        if(my_rank%2 != 0) // odd rank
            partner = my_rank - 1;
        else               // even rank
            partner = my_rank + 1;
    // odd phase
    } else
    {
        if(my_rank%2 != 0)
            partner = my_rank + 1;
        else
            partner = my_rank - 1;
    }

    // partner is out of boundary
    if(partner == -1 || partner == comm_sz)
        partner = MPI_PROC_NULL;

    return partner;
}

void clearArray(int keys, int *a)
{
    int i;
    for(i = 0; i < keys; i++)
    	a[i] = 0;
}
