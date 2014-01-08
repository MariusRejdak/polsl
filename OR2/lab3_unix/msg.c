#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"

#define TAB_SIZE_S 100
#define TAB_SIZE_T 100
#define _N 10
#define BUFSIZE (sizeof(int)*(_N+1))


typedef struct {
    long typ;
    int liczby[_N+1];
} msgbufor;

// Zadanie:
// Wszystkie elementy zbioru S mają być mniejsze od elementów zbioru T
// S - s_tab
// T - t_tab+_N

int main(int argc, const char* argv[])
{
    int pid = 0;
    int klucz;
    int kolejka;

    pid = fork();

    klucz = ftok("kolejka", 'c');
    kolejka = msgget(klucz, IPC_CREAT | 0666);

    if (pid > 0) //Proces A - przechowuje S
    {
        int *s_tab = (int*)calloc(TAB_SIZE_S, sizeof(int));
        srand(time(NULL));

        gen_rands(s_tab, TAB_SIZE_S);
        qsort(s_tab, TAB_SIZE_S, sizeof(int), compare_a);

        while(1)
        {
            msgbufor bufor;
            int brak_zmian = 0;

            bufor.typ = 1L;
            bufor.liczby[0] = 0;
            for(int i = 0; i < _N; ++i)
                bufor.liczby[i+1] = s_tab[i];

            msgsnd(kolejka, &bufor, BUFSIZE, 0);
            msgrcv(kolejka, &bufor, BUFSIZE, 2L, 0);
            qsort(bufor.liczby+1, _N, sizeof(int), compare_a);

            for(int i = 0; i < _N; ++i)
            {
                if(s_tab[i] == bufor.liczby[i+1])
                    ++brak_zmian;
                else
                    s_tab[i] = bufor.liczby[i+1];
            }

            if(brak_zmian == _N)
            {
                bufor.typ = 1L;
                bufor.liczby[0] = 1;
                msgsnd(kolejka, &bufor, _N, 0);
                break;
            }
            
            qsort(s_tab, TAB_SIZE_S, sizeof(int), compare_a);
        }

        //printf("Finished A\n"); fflush(stdout);

        FILE *fp = fopen("a.txt", "w");
        fprintf (fp, "Zbiór S\n");
        for(int i = 0; i < TAB_SIZE_S; ++i)
            fprintf (fp, "%d ", s_tab[i]);
        fprintf (fp, "\n");
        fprintf (fp, "\n");
        fclose(fp);

        free(s_tab);
        exit(0);
    }
    else if(pid == 0) //Proces B - przechowuje T
    {
        int *t_tab = (int*)calloc (TAB_SIZE_T+_N, sizeof(int));
        srand(time(NULL)+1);

        gen_rands(t_tab+_N, TAB_SIZE_T);
        qsort(t_tab+_N, TAB_SIZE_T, sizeof(int), compare_b);

        while(1)
        {
            msgbufor bufor;

            msgrcv(kolejka, &bufor, BUFSIZE, 1L, 0);
            if(bufor.liczby[0] == 1)
                break;
            qsort(bufor.liczby+1, _N, sizeof(int), compare_b);

            for(int i = 0; i < _N; ++i) {
                t_tab[i] = bufor.liczby[i+1];
            }

            qsort(t_tab, TAB_SIZE_T+_N, sizeof(int), compare_b);

            bufor.typ = 2L;
            for(int i = 0; i < _N; ++i)
                bufor.liczby[i+1] = t_tab[i];
            
            msgsnd(kolejka, &bufor, BUFSIZE, 0);
        }

        //printf("Finished B\n"); fflush(stdout);

        FILE *fp = fopen("b.txt", "w");
        for(int i = _N; i < TAB_SIZE_T+_N; ++i)
            fprintf (fp, "%d ", t_tab[i]);
        fprintf (fp, "\n");
        fclose(fp);

        //Proces T odebrał ostatnią wiadomość - usuwa kolejke
        msgctl(kolejka, IPC_RMID, NULL);

        free(t_tab);
        exit(0);
    }

    return 1;
}