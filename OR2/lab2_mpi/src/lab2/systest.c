// Marius Rejdak
// Informatyka, mgr, OS1

/*
Data wykonania ćwiczenia: 2013-11-13
*/

#include "mpi.h"
#include <stdio.h>

#define _MPI_ROOT 0
#define _T_ 2

int main(int argc, char *argv[])
{
	unsigned int me, size;
	float tab[8] = {1.0, 1.1, 1.2, 1.3, 10.4, 1.5, 1.6, 1.7}, temp = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Scatter(tab, 1, MPI_FLOAT, &temp, 1, MPI_FLOAT, _MPI_ROOT, MPI_COMM_WORLD);

	for(int i = 0; i < _T_; ++i)
	{
		MPI_Status status;
		float temp_p = 0, temp_n = 0;

		MPI_Send(&temp, 1, MPI_FLOAT, (me-1)%8, 0, MPI_COMM_WORLD);
		MPI_Send(&temp, 1, MPI_FLOAT, (me+1)%8, 0, MPI_COMM_WORLD);
		MPI_Recv(&temp_p, 1, MPI_FLOAT, (me-1)%8, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&temp_n, 1, MPI_FLOAT, (me+1)%8, 0, MPI_COMM_WORLD, &status);

		temp = (temp*2.f + temp_p + temp_n) / 4.f;
	}

    MPI_Gather(&temp, 1, MPI_FLOAT, tab, 1, MPI_FLOAT, _MPI_ROOT, MPI_COMM_WORLD);

    if(me == _MPI_ROOT)
	{
		for(int i = 0; i < 8; ++i)
		{
			printf("%f\n", tab[i]);
		}
	}
	
	MPI_Finalize();
}  
