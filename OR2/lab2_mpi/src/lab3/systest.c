// Marius Rejdak
// Informatyka, mgr, OS1

/*
Data wykonania ćwiczenia: 2013-11-20
*/

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define ZAKLAD_A 0
#define ZAKLAD_B 1
#define ZAKLAD_C 2

//Tags
#define END_PROGRAM 0
#define GET_MATRIX 1
#define MATRIX_SIZE 2
#define MATRIX_VALUES 3
#define MATRIX_SOL 4
#define PROBLEM_REQ 5
#define PROBLEM_VALUES 6
#define PROBLEM_SOL 7

//Limit wykonanych działań
#define ILOSC_MACIERZY 100

//Średnie występowanie problemów
#define PORTZ_REKRUTACJA 10 	//Zakład A
#define PORTZ_SPOR_PRAWN 20 	//Zakład B
#define PORTZ_INFOR 5 			//Zakład C

//Trudności problemów
#define ROZW_REKRUTACJA 10
#define ROZW_SPOR_PRAWN 1000
#define ROZW_PROB_INFOR 100

enum Problem {NONE = 0, REKRUTACJA, SPOR_PRAWNY, INFORM};

struct Zaklad
{
	int typ;
	enum Problem problem;
	bool problemSend;
	int problemSendFrom;
	int sendMatrixSize;
};

void getMatrix(int *size, double **values)
{
	if(*size == 0)
	{
		*size = rand()%100 + 1;
	}
	
	*values = malloc((*size) * (*size) * sizeof(double));

	for(int i = 0; i < size*size; ++i)
    {
    	values[i] = rand()%1000;
    }
}

bool getProblem(enum Problem *p)
{
	unsigned int s = rand();
	if(s % PORTZ_SPOR_PRAWN == 0)
	{
		*p = SPOR_PRAWNY;
		return true;
	}
	else if(s+1 % PORTZ_REKRUTACJA == 0)
	{
		*p = REKRUTACJA;
		return true;
	}
	else if(s+2 % PORTZ_INFOR == 0)
	{
		*p = INFORM;
		return true;
	}
	else
	{
		*p = NONE;
		return false;
	}
}

inline int getProblemSize(enum Problem *p)
{
	switch(*p)
	{
		case REKRUTACJA:
		return ROZW_REKRUTACJA;
		case SPOR_PRAWNY:
		return ROZW_SPOR_PRAWN;
		case INFORM:
		return ROZW_PROB_INFOR;
		default:
		return 0;
	}
}

double solveProblem(int size, double values[])
{
	double start_time = MPI_Wtime();
	size_t num_bytes = sizeof(double)*size*size;
    double macierz_src[] = malloc(num_bytes);
    memcpy(macierz_src, values, num_bytes);

    //computations here
    for(int i = 0; i < size*size; ++i)
    {
    	values[i] = macierz_src[i]*macierz_src[i];
    }

    free(macierz_src);
    return MPI_Wtime()-start_time;
}

inline int zakladNr(int me)
{
	return me-1 % 3;
}

void process_zaklad(int me, short typ_zakladu, double *times)
{
	double *timeWork = times+0, *timeProblems = times+1, *timeMyProblems = times+2, *timeWait = times+3;
	int i = 0;

	while(i < ILOSC_MACIERZY)
	{
		MPI_Status status;
		int flag = 0;

		*timeWork += doRegularWork();

		MPI_Iprobe(MPI_ANY_SOURCE, TAG(0,END_PROGRAM), MPI_COMM_WORLD, &flag, &status);
		if(flag)
		{
			MPI_Recv(0, 0, MPI_CHAR, status.MPI_SOURCE, TAG(0,END_PROGRAM), MPI_COMM_WORLD, &status);
			break;
		}

		MPI_Iprobe(MPI_ANY_SOURCE, TAG(typ_zakladu,PROBLEM_SRC), MPI_COMM_WORLD, &flag, &status);
		if(flag)
		{
			int source = status.MPI_SOURCE, buf;
			MPI_Recv(&p, sizeof(enum Problem), MPI_CHAR, source, TAG_PROBLEM, MPI_COMM_WORLD, &status);
			*timeProblems += solveProblem(ROZW_REKRUTACJA);
			MPI_Send(&p, sizeof(enum Problem), MPI_CHAR, source, TAG_SOLUTION, MPI_COMM_WORLD);
		}

		if(getProblem(&p))
		{
			if(p == REKRUTACJA)
				*timeMyProblems += solveProblem(ROZW_REKRUTACJA);
			else
				*timeWait += waitForSolution(&p);
		}
	}

	MPI_Send(0, 0, MPI_INT, ZAKLAD_B, TAG_FINISHED, MPI_COMM_WORLD);
	MPI_Send(0, 0, MPI_INT, ZAKLAD_C, TAG_FINISHED, MPI_COMM_WORLD);
}

void proces_klient(int me, int *send_matrixes, int *got_solved)
{
	int mpi_size;
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
	struct Zaklad zaklady[mpi_size];

	for(int i = 1; i < mpi_size; ++i)
	{
		zaklady[i].typ = zakladNr(me);
		zaklady[i].problem = NONE;
		zaklady[i].problemSend = false;
		zaklady[i].problemSendFrom = 0;
		zaklady[i].sendMatrixSize = 0;
	}

	while(true)
	{
		MPI_Status status, status_tmp;

		MPI_probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if(status.MPI_TAG == GET_MATRIX)
		{
			int source = status.MPI_SOURCE;
			int size = 0;
			double *values;
			enum Problem canSolve = NONE;

			MPI_Recv(0, 0, MPI_CHAR, source, GET_MATRIX, MPI_COMM_WORLD, &status_tmp);

			switch(zakladNr(source))
			{
				case ZAKLAD_A:
				canSolve = REKRUTACJA;
				break;

				case ZAKLAD_B:
				canSolve = SPOR_PRAWNY;
				break;

				case ZAKLAD_C:
				canSolve = INFORM;
				break;

				default:
				break;
			}

			for(int i = 1; i < mpi_size; ++i)
			{
				if(i != source && zaklady[i].problem == canSolve)
				{
					
				}
			}

			getMatrix(&size, &values);
			zaklady[source].sendMatrixSize = size;
			zaklady[source].problemSendFrom = 0;
			MPI_Send(&size, 1, MPI_INT, source, MATRIX_SIZE, MPI_COMM_WORLD);
			MPI_Send(values, size*size, MPI_DOUBLE, source, MATRIX_VALUES, MPI_COMM_WORLD);
			send_matrixes++;

			free(values);
		}
		else if(status.MPI_TAG == MATRIX_SOL)
		{
			int source = status.MPI_SOURCE;
			int size = zaklady[source].sendMatrixSize;
			double *values = malloc(size*size*sizeof(double));

			MPI_Recv(values, size*size, MPI_DOUBLE, source, MATRIX_SOL, MPI_COMM_WORLD, &status_tmp);

			got_solved++;

			free(values);
		}
		else if(status.MPI_TAG == PROBLEM_REQ)
		{
			int source = status.MPI_SOURCE;
			enum Problem p = NONE;

			MPI_Recv(&p, sizeof(enum Problem), MPI_CHAR, source, PROBLEM_REQ, MPI_COMM_WORLD, &status_tmp);
			zaklady[source].problem = p;
		}
		else if(status.MPI_TAG == PROBLEM_SOL)
		{
			int source = status.MPI_SOURCE;
			int size = zaklady[source].sendMatrixSize;
			double *values = malloc(size*size*sizeof(double));

			MPI_Recv(values, size*size, MPI_DOUBLE, source, MATRIX_SOL, MPI_COMM_WORLD, &status_tmp);
			MPI_Send(values, size*size, MPI_DOUBLE, zaklady[source].problemSendFrom, MATRIX_SOL, MPI_COMM_WORLD);

			zaklady[source].problemSendFrom = 0;
			free(values);
		}
		else if(status.MPI_TAG == END_PROGRAM)
		{
			int source = status.MPI_SOURCE;

			MPI_Recv(0, 0, MPI_CHAR, source, END_PROGRAM, MPI_COMM_WORLD, &status_tmp);
			for(int i = 1; i < mpi_size; ++i)
			{
				if(i != source)
					MPI_Send(0, 0, MPI_CHAR, i, END_PROGRAM, MPI_COMM_WORLD);
			}
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	int me;
	double times[4] = {0.};
	int send_matrixes = 0, got_solved = 0;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);

	srand(time(0)+me); //so random

	if(me == 0)
		proces_klient(me, &send_matrixes, &got_solved);
	else
		switch(zakladNr(me))
		{
			case ZAKLAD_A:
			//proces_ZA(me, times);
			break;
			case ZAKLAD_B:
			//proces_ZB(me, times);
			break;
			case ZAKLAD_C:
			//proces_ZC(me, times);
			break;
			default:
			break;
		}

	if(me != 0)
		printf("Zakład %d: %lf %lf %lf %lf\n", zakladNr(me), times[0], times[1], times[2], times[3]);
	else
		printf("Klient: wysłano %d macierzy\n", send_matrixes);

	MPI_Finalize();
}  
