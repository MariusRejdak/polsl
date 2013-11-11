// Marius Rejdak
// Informatyka, mgr, OS1

#include "mpi.h"
#include <stdio.h>

void MPI_Bcast_custom(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
	int me, numprocs;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	//printf("Bcast called from %d\n", me);

	if(root == me) 
	{
		for(int i = 0; i < numprocs; ++i)
		{
			if(i != me)
			{
				MPI_Send(buffer, count, datatype, i, 0, comm);
			}
		}
	}
	else
	{
		MPI_Recv(buffer, count, datatype, root, 0, comm, &status);
	}
}

void MPI_Reduce_sum_ints(int *send, int *recv, int root, MPI_Comm comm)
{
	int me, numprocs, buffer;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	//printf("Reduce called from %d\n", me);

	if(root == me) 
	{
		*recv = *send;
		for(int i = 0; i < numprocs; ++i)
		{
			if(i != me)
			{
				MPI_Recv(&buffer, 1, MPI_INT, i, 0, comm, &status);
				*recv += buffer;
			}
		}
	}
	else
	{
		MPI_Send(send, 1, MPI_INT, root, 0, comm);
	}
}

#define _MPI_ROOT 0

int main(int argc, char *argv[])
{
	int me, option, namelen, size, test = -1, test2 = -1;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	while(test != 0) {
		if (me == _MPI_ROOT)
		{
			printf("Enter number (zero exits): ");
			fflush(stdout);
			scanf("%d", &test); //don't work with _MPI_ROOT != 0
		}

		MPI_Bcast_custom(&test, 1, MPI_INT, _MPI_ROOT, MPI_COMM_WORLD);

		MPI_Reduce_sum_ints(&test, &test2, _MPI_ROOT, MPI_COMM_WORLD);
		
		if (me == _MPI_ROOT)
		{
			printf("I am %d of %d, got sum %d\n", me, size, test2);
		}
	}
	
	MPI_Finalize();
}  
