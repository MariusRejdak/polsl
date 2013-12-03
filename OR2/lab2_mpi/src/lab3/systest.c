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

#define KLIENT 0
#define ZAKLAD_A 1
#define ZAKLAD_B 2
#define ZAKLAD_C 3

#define MACIERZ_SRC 1
#define MACIERZ_RZW 2
#define PROBLEM_SRC 3
#define PROBLEM_RZW 4
#define END_PROGRAM 0

#define TAG(zaklad,problem) zaklad*10+problem

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

int getProblemSize(enum Problem *p)
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

double waitForSolution(enum Problem *p, double values[])
{
	MPI_Status status;

	switch(*p)
	{
		case REKRUTACJA:
		MPI_Send(p, getProblemSize(p), MPI_CHAR, ZAKLAD_A, TAG_PROBLEM, MPI_COMM_WORLD);
		MPI_Recv(p, sizeof(enum Problem), MPI_CHAR, ZAKLAD_A, TAG_SOLUTION, MPI_COMM_WORLD, &status);
		break;

		case SPOR_PRAWNY:
		MPI_Send(p, sizeof(enum Problem), MPI_CHAR, ZAKLAD_B, TAG_PROBLEM, MPI_COMM_WORLD);
		MPI_Recv(p, sizeof(enum Problem), MPI_CHAR, ZAKLAD_B, TAG_SOLUTION, MPI_COMM_WORLD, &status);
		break;

		case INFORM:
		MPI_Send(p, sizeof(enum Problem), MPI_CHAR, ZAKLAD_C, TAG_PROBLEM, MPI_COMM_WORLD);
		MPI_Recv(p, sizeof(enum Problem), MPI_CHAR, ZAKLAD_C, TAG_SOLUTION, MPI_COMM_WORLD, &status);
		break;

		default:
		break;
	}

	return 1.;
}

double doRegularWork()
{
	return 1.;
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
}*/

int main(int argc, char *argv[])
{
	int me;
	double times[4] = {0.};
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	//MPI_Comm_size(MPI_COMM_WORLD, &size);

	srand(time(0)+me);

	switch(me)
	{
		case ZAKLAD_A:
		//process_ZA(me, times);
		break;
		case ZAKLAD_B:
		//process_ZB(me, times);
		break;
		case ZAKLAD_C:
		//process_ZC(me, times);
		break;
		default:
		break;
	}

	printf("%d: %lf %lf %lf %lf\n", me, times[0], times[1], times[2], times[3]);

	MPI_Finalize();
}  
