// Marius Rejdak
// Informatyka, mgr, OS1

/*
Symulacja firmy Macierz - komunikacja niesynchroniczna w MPI
Data wykonania ćwiczenia: 2013-11-27
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
#define MESSAGE 1
#define PROBLEM_SOL 2

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

struct ZakladInfo
{
	int typ;
	bool problemRekrutacja;
	bool problemSporPrawny;
	bool problemInform;
};

struct Message
{
	int problemSource;
	int matrixSize;
	enum Problem problemType;
};

void getMatrix(int size, double **values)
{
	*values = malloc(size * size * sizeof(double));

	for(int i = 0; i < size*size; ++i)
    {
    	(*values)[i] = rand()%1000;
    }
}

int getProblemSize(enum Problem p)
{
	switch(p)
	{
		case NONE:
		default:
		return rand()%100 + 1;

		case REKRUTACJA:
		return ROZW_REKRUTACJA;

		case SPOR_PRAWNY:
		return ROZW_SPOR_PRAWN;

		case INFORM:
		return ROZW_PROB_INFOR;
	}
}

enum Problem getProblem()
{
	unsigned int s = rand();
	if(s % PORTZ_SPOR_PRAWN == 0)
	{
		return SPOR_PRAWNY;
	}
	else if((s+1) % PORTZ_REKRUTACJA == 0)
	{
		return REKRUTACJA;
	}
	else if((s+2) % PORTZ_INFOR == 0)
	{
		return INFORM;
	}
	else
	{
		return NONE;
	}
}

double solveProblem(int size, double *values)
{
	double start_time = MPI_Wtime();
	size_t num_bytes = sizeof(double)*size*size;
    double *macierz_src = malloc(num_bytes);
    memcpy(macierz_src, values, num_bytes);

    for(int i = 0; i < size*size; ++i)
    {
    	values[i] = macierz_src[i]*macierz_src[i];
    }

    free(macierz_src);
    return MPI_Wtime()-start_time;
}

inline int zakladNr(int me)
{
	return (me-1) % 3;
}

enum Problem rozwiazuje(int zaklad)
{
	switch(zaklad)
	{
		case ZAKLAD_A:
		return REKRUTACJA;

		case ZAKLAD_B:
		return SPOR_PRAWNY;

		case ZAKLAD_C:
		return INFORM;

		default:
		return NONE;
	}
}

int hasProblem(enum Problem p, struct ZakladInfo *zaklady, int mpi_size)
{
	for(int i = 1; i < mpi_size; ++i)
	{
		if(p == REKRUTACJA)
		{
			if(zaklady[i].problemRekrutacja)
				return i;
		}
		else if(p == SPOR_PRAWNY)
		{
			if(zaklady[i].problemSporPrawny)
				return i;
		}
		else if(p == INFORM)
		{
			if(zaklady[i].problemInform)
				return i;
		}
	}

	return 0;
}

void proces_zaklad(int me, int typ_zakladu, double *times)
{
	double *timeWork = times+0, *timeProblems = times+1, *timeMyProblems = times+2, *timeWait = times+3;
	bool problemRekrutacja = false, problemSporPrawny = false, problemInform = false;
	int matrixes_solved = 0;
	enum Problem rozwiazujeP = rozwiazuje(typ_zakladu);

	while(matrixes_solved < 10000)
	{
		MPI_Status status, status_tmp;
		int tag, source, flag = 0;

		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
		if(flag)
		{
			source = status.MPI_SOURCE;
			tag = status.MPI_TAG;
			if(tag == END_PROGRAM)
			{
				MPI_Recv(0, 0, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status_tmp);
				return;
			}
			else if(tag == MESSAGE)
			{
				struct Message m;
				MPI_Recv(&m, sizeof(struct Message), MPI_CHAR, source, tag, MPI_COMM_WORLD, &status_tmp);

				if(m.problemType == NONE)
				{
					double *values = NULL;
					int size = m.matrixSize;
					getMatrix(size, &values);
					*timeWork += solveProblem(size, values);
					free(values);

					matrixes_solved++;

					enum Problem p = getProblem();
					switch(p)
					{
						case REKRUTACJA:
						problemRekrutacja = true;
						break;

						case SPOR_PRAWNY:
						problemSporPrawny = true;
						break;

						case INFORM:
						problemInform = true;
						break;
					}

					if(p != NONE)
					{
						struct Message mx;
						mx.matrixSize = getProblemSize(p);
						mx.problemType = p;
						mx.problemSource = me;
						MPI_Send(&mx, sizeof(struct Message), MPI_CHAR, 0, MESSAGE, MPI_COMM_WORLD);
					}
					
				}
				else if(m.problemType == rozwiazujeP)
				{
					double *values = NULL;
					int size = m.matrixSize;
					getMatrix(size, &values);
					*timeProblems += solveProblem(size, values);
					free(values);

					MPI_Send(&m, sizeof(struct Message), MPI_CHAR, 0, PROBLEM_SOL, MPI_COMM_WORLD);
					MPI_Send(&m, sizeof(struct Message), MPI_CHAR, m.problemSource, PROBLEM_SOL, MPI_COMM_WORLD);
				}
			}
			else if(tag == PROBLEM_SOL)
			{
				struct Message m;
				MPI_Recv(&m, sizeof(struct Message), MPI_CHAR, source, tag, MPI_COMM_WORLD, &status_tmp);

				switch(m.problemType)
				{
					case REKRUTACJA:
					problemRekrutacja = false;
					break;

					case SPOR_PRAWNY:
					problemSporPrawny = false;
					break;

					case INFORM:
					problemInform = false;
					break;
				}
			}
		}
		else
		{
			struct Message m;
			m.problemType = NONE;
			MPI_Send(&m, sizeof(struct Message), MPI_CHAR, 0, MESSAGE, MPI_COMM_WORLD);
		}
	}

	MPI_Send(0, 0, MPI_CHAR, 0, END_PROGRAM, MPI_COMM_WORLD);
}

void proces_klient(int me, int mpi_size, int *send_matrixes)
{
	struct ZakladInfo zaklady[mpi_size];

	for(int i = 1; i < mpi_size; ++i)
	{
		zaklady[i].typ = zakladNr(i);
		zaklady[i].problemRekrutacja = false;
		zaklady[i].problemSporPrawny = false;
		zaklady[i].problemInform = false;
	}

	while(true)
	{
		MPI_Status status, status_tmp;
		int source, tag;

		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		source = status.MPI_SOURCE;
		tag = status.MPI_TAG;
		if(tag == END_PROGRAM)
		{
			MPI_Recv(0, 0, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status_tmp);
			for(int i = 1; i < mpi_size; ++i)
			{
				if(i != source)
				{
					MPI_Send(0, 0, MPI_CHAR, i, END_PROGRAM, MPI_COMM_WORLD);
				}
			}

			int flag = 0;
			do
			{
				MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status_tmp);
				if(flag)
				{
					struct Message m;
					MPI_Recv(&m, sizeof(struct Message), MPI_CHAR, status_tmp.MPI_SOURCE, status_tmp.MPI_TAG, MPI_COMM_WORLD, &status_tmp);
				}
			} while(flag);

			return;
		}
		else if(tag == MESSAGE)
		{
			struct Message m;
			MPI_Recv(&m, sizeof(struct Message), MPI_CHAR, source, tag, MPI_COMM_WORLD, &status_tmp);

			if(m.problemType == NONE)
			{
				enum Problem p = rozwiazuje(zakladNr(source));
				int todoProblem = hasProblem(p, zaklady, mpi_size);

				if(todoProblem)
				{
					m.problemSource = todoProblem;
					m.problemType = p;
					m.matrixSize = getProblemSize(p);
				}
				else
				{
					m.problemSource = 0;
					m.problemType = NONE;
					m.matrixSize = getProblemSize(NONE);
				}

				MPI_Send(&m, sizeof(struct Message), MPI_CHAR, source, MESSAGE, MPI_COMM_WORLD);
				(*send_matrixes)++;
			}
			else
			{
				switch(m.problemType)
				{
					case REKRUTACJA:
					zaklady[source].problemRekrutacja = true;
					break;

					case SPOR_PRAWNY:
					zaklady[source].problemSporPrawny = true;
					break;

					case INFORM:
					zaklady[source].problemInform = true;
					break;
				}
			}
		}
		else if(tag == PROBLEM_SOL)
		{
			struct Message m;
			MPI_Recv(&m, sizeof(struct Message), MPI_CHAR, source, tag, MPI_COMM_WORLD, &status_tmp);

			switch(m.problemType)
			{
				case REKRUTACJA:
				zaklady[m.problemSource].problemRekrutacja = false;
				break;

				case SPOR_PRAWNY:
				zaklady[m.problemSource].problemSporPrawny = false;
				break;

				case INFORM:
				zaklady[m.problemSource].problemInform = false;
				break;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int me, size;
	double times[4] = {0.};
	int send_matrixes = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	srand(time(0)+me); //so random

	if(me != 0)
	{
		proces_zaklad(me, zakladNr(me), times);
		printf("Zakład %d: rozwiązywano macierze %lfs, rozwiązywano problemy %lfs\n", zakladNr(me), times[0], times[1]);
	}
	else
	{
		proces_klient(me, size, &send_matrixes);
		printf("Klient: wysłano %d macierzy\n", send_matrixes);
	}

	MPI_Finalize();
}  
