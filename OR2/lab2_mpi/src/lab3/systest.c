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

//Numeracja zakładów (!= numeracja procesów)
#define ZAKLAD_A 0
#define ZAKLAD_B 1
#define ZAKLAD_C 2

//Tagi
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

//Oznaczenie problemu
enum Problem {NONE = 0, REKRUTACJA, SPOR_PRAWNY, INFORM};

//Struktura do przesyłania informacji o każdym zakładzie
struct ZakladInfo
{
	int typ;
	bool problemRekrutacja;
	bool problemSporPrawny;
	bool problemInform;
};

//Strunktura do przesyłania opisu problemu
struct Message
{
	int problemSource;
	int matrixSize;
	enum Problem problemType;
};

//Generacji macierzy
void getMatrix(int size, double **values)
{
	*values = malloc(size * size * sizeof(double));

	for(int i = 0; i < size*size; ++i)
    {
    	(*values)[i] = rand()%1000;
    }
}

//Zwraca wielkość macierzy do rozwiązania
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

//Generacja problemu
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

//Rozwiązanie zadanego problemu
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

//Przekształca numer procesu na numer zakładu
inline int zakladNr(int me)
{
	return (me-1) % 3;
}

//Dla numeru zakładu zwraca numer rozwiązywanego problemu
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

//Zwraca numer zakładu który może rozwiązać zadany problem
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

//Główny proces zakładu
void proces_zaklad(int me, int typ_zakladu, double *times)
{
	//Zapisywane łączne czasy wykonywanych operacji
	double *timeWork = times+0, *timeProblems = times+1, *timeMyProblems = times+2, *timeWait = times+3;
	//Czy ten zakład ma nierozwiązany problem
	bool problemRekrutacja = false, problemSporPrawny = false, problemInform = false;
	//Licznik rozwiązanych macierzy od klientów
	int matrixes_solved = 0;
	//Rodzaj rozwiązywanych problemów przez zakład
	enum Problem rozwiazujeP = rozwiazuje(typ_zakladu);

	//Maksymalna liczba rozwiązanych macierzy
	while(matrixes_solved < 10000)
	{
		MPI_Status status, status_tmp;
		int tag, source, flag = 0;

		//Sprawdzenie czy istnieje oczekująca wiadomość
		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
		if(flag) //Oczekująca wiadomość na odebranie
		{
			source = status.MPI_SOURCE; //Nadawca wiadomości
			tag = status.MPI_TAG; //Tag wiadomości
			if(tag == END_PROGRAM) //Oczekująca wiadomość to informacja o zakończeniu programu
			{
				//Opcjonalne, odebranie wiadomości o zakończeniu programu
				MPI_Recv(0, 0, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status_tmp);
				return;
			}
			else if(tag == MESSAGE) //Oczekująca wiadomość to problem do rozwiązania
			{
				struct Message m;

				//Odebranie wiadomości
				MPI_Recv(&m, sizeof(struct Message), MPI_CHAR, source, tag, MPI_COMM_WORLD, &status_tmp);

				if(m.problemType == NONE) //Wiadomość to macierz do rozwiązania od klient
				{
					//Rozwiązanie macierzy od klienta
					double *values = NULL;
					int size = m.matrixSize;
					getMatrix(size, &values);
					*timeWork += solveProblem(size, values);
					free(values);

					matrixes_solved++;

					//"Generacja" problemu
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

					if(p != NONE) //Wystąpił problem, powiadomienie procesu nadzorcy o oczekującym na rozwiązanie problemie
					{
						struct Message mx;
						mx.matrixSize = getProblemSize(p);
						mx.problemType = p;
						mx.problemSource = me;
						MPI_Send(&mx, sizeof(struct Message), MPI_CHAR, 0, MESSAGE, MPI_COMM_WORLD);
					}
					
				}
				else if(m.problemType == rozwiazujeP) //Wiadomość to problem który może rozwiązać ten zakłąd
				{
					//Rozwiązanie problemu
					double *values = NULL;
					int size = m.matrixSize;
					getMatrix(size, &values);
					*timeProblems += solveProblem(size, values);
					free(values);

					//Powiadomienie procesu nadzorcy o rozwiązaniu problemu
					MPI_Send(&m, sizeof(struct Message), MPI_CHAR, 0, PROBLEM_SOL, MPI_COMM_WORLD);
					//Powiadomienie "autora" problemu o rozwiązaniu problemu
					MPI_Send(&m, sizeof(struct Message), MPI_CHAR, m.problemSource, PROBLEM_SOL, MPI_COMM_WORLD);
				}
			}
			else if(tag == PROBLEM_SOL) //Oczekująca wiadomość to informacja o rozwiązaniu problemu
			{
				//Odebranie wiadomości
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
		else //Brak oczekującej wiadomości, żądanie otrzymania zadania do wykonania
		{
			struct Message m;
			m.problemType = NONE;
			MPI_Send(&m, sizeof(struct Message), MPI_CHAR, 0, MESSAGE, MPI_COMM_WORLD);
		}
	}

	//Powiadomienie procesu nadzorcy o zakończeniu programu
	MPI_Send(0, 0, MPI_CHAR, 0, END_PROGRAM, MPI_COMM_WORLD);
}

//Główny proces klienta (nadzorcy)
void proces_klient(int me, int mpi_size, int *send_matrixes)
{
	//Tablica przechowująca informacje o zakładach
	struct ZakladInfo zaklady[mpi_size];
	//Inicjalizacja w/w tablicy
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

		//Czekanie na nową oczekującą wiadomość
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		source = status.MPI_SOURCE; //Nadawca wiadomości
		tag = status.MPI_TAG; //Tag wiadomości
		if(tag == END_PROGRAM) //Zakończenie programu
		{
			//Odebranie wiadomości
			MPI_Recv(0, 0, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status_tmp);
			//Powiadomienie pozostałych procesów o zakończeniu programu
			for(int i = 1; i < mpi_size; ++i)
			{
				if(i != source)
				{
					MPI_Send(0, 0, MPI_CHAR, i, END_PROGRAM, MPI_COMM_WORLD);
				}
			}

			//Informacja o zakończeniu programu może dojść do zakładów w momencie kiedy będą zablokowane
			//na operacji wysyłania, należy ich wiadomości odebrać aby mogły się normalnie zakończyć
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
			//Odebranie wiadomości
			struct Message m;
			MPI_Recv(&m, sizeof(struct Message), MPI_CHAR, source, tag, MPI_COMM_WORLD, &status_tmp);

			if(m.problemType == NONE) //Zakład wysłał rządanie problemu do rozwiązania
			{
				//Sprawdzenie czy oczekuje jakiś problem oczekujący na rozwiązanie dla tego zakładu
				enum Problem p = rozwiazuje(zakladNr(source));
				int todoProblem = hasProblem(p, zaklady, mpi_size);

				if(todoProblem)
				{
					m.problemSource = todoProblem;
					m.problemType = p;
					m.matrixSize = getProblemSize(p);
				}
				else //Wygenerowanie macierzy "od klienta"
				{
					m.problemSource = 0;
					m.problemType = NONE;
					m.matrixSize = getProblemSize(NONE);
				}

				//Wysłanie wiadomości
				MPI_Send(&m, sizeof(struct Message), MPI_CHAR, source, MESSAGE, MPI_COMM_WORLD);

				//Zliczanie wysłanych macierzy
				(*send_matrixes)++;
			}
			else //Zakład zgłosił wystąpienie problemu
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
		else if(tag == PROBLEM_SOL) //Zakład zgłosił rozwiązanie problemu
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

	//Inicjalizacja OpenMPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//Na jednym komputerze wszystkie procesy dochodzą do tego momentu jednocześnie
	//+me gwarantuje różne ziarna generatora liczb pseudolowych we wszystkich procesach
	srand(time(0)+me);

	//Wypisanie danych wynikowych
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

	//Zwolnienie zasobów OpenMPI
	MPI_Finalize();
}  
