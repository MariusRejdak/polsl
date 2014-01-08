// Marius Rejdak
// Informatyka, mgr, OS1

/*
Zadanie 11, Unix
Data wykonania ćwiczenia: 2014-01-08
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "semparams.h"


int main(int argc, char *argv[])
{
    int t_tab[TAB_SIZE_T+_N];
    int klucz, pamiec, semafor;
    int *pamwsk;
    struct sembuf op[1];

    srand(time(NULL));
    gen_rands(t_tab+_N, TAB_SIZE_T);
    qsort(t_tab+_N, TAB_SIZE_T, sizeof(int), compare_b);

    klucz = ftok("sm", 'x');
    pamiec = shmget(klucz, (_N+1)*sizeof(int), IPC_CREAT | 0600);
    pamwsk = (int*)shmat(pamiec, 0, SHM_RND);
    semafor = semget(klucz, 2, IPC_CREAT | 0600);

    printf("B running...\n");
    debug_pring("Generated T", t_tab+_N, TAB_SIZE_T);

    while(pamwsk[0])
    {
        int *bufor = pamwsk+1;

        op[0].sem_num = SEMB;
        op[0].sem_op = -1;
        op[0].sem_flg = 0;
        semop(semafor, op, 1);

        if(!pamwsk[0])
            break;

        //debug_pring("to_x entry", bufor, _N);

        memcpy(t_tab, bufor, _N_SIZE);
        qsort(t_tab, TAB_SIZE_T+_N, sizeof(int), compare_b);
        memcpy(bufor, t_tab, _N_SIZE);

        //debug_pring("to_x end", bufor, _N);

        op[0].sem_num = SEMA;
        op[0].sem_op = 1;
        op[0].sem_flg = 0;
        semop(semafor, op, 1);
    }

    printf("B finished:\n");
    for(int i = _N; i < TAB_SIZE_T+_N; ++i)
            printf("%d ", t_tab[i]);
    printf("\n");

    return 0;
}