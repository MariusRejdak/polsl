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
    int s_tab[TAB_SIZE_S];
    int klucz, pamiec, semafor;
    int *pamwsk;
    struct sembuf op[1];
    union semun{
        int val;
        struct semid_ds *bf;
        unsigned short *array;
    } val;

    srand(time(NULL));
    gen_rands(s_tab, TAB_SIZE_S);
    qsort(s_tab, TAB_SIZE_S, sizeof(int), compare_a);

    klucz = ftok("sm", 'x');
    pamiec = shmget(klucz, (_N+1)*sizeof(int), IPC_CREAT | 0600);
    pamwsk = (int*)shmat(pamiec, 0, SHM_RND);
    semafor = semget(klucz, 2, IPC_CREAT | 0600);

    assert(klucz != -1);
    assert(pamiec != -1);  
    assert((int)pamwsk != -1);
    assert(semafor != -1);

    val.val = 0;
    semctl(semafor, SEMA, SETVAL, val);
    semctl(semafor, SEMB, SETVAL, val);

    pamwsk[0] = 1;

    printf("A running...\n");
    debug_pring("Generated S", s_tab, TAB_SIZE_S);

    while(pamwsk[0])
    {
        int *bufor = pamwsk+1;
        int brak_zmian = 0;

        for(int i = 0; i < _N; ++i)
            bufor[i] = s_tab[i];

        //debug_pring("to_x entry", bufor, _N);

        op[0].sem_num = SEMB;
        op[0].sem_op = 1;
        op[0].sem_flg = 0;
        semop(semafor, op, 1);

        op[0].sem_num = SEMA;
        op[0].sem_op = -1;
        op[0].sem_flg = 0;
        semop(semafor, op, 1);

        qsort(bufor, _N, sizeof(int), compare_a);

        //debug_pring("to_x end", bufor, _N);

        for(int i = 0; i < _N; ++i)
        {
            if(s_tab[i] == bufor[i])
                ++brak_zmian;
            else
                s_tab[i] = bufor[i];
        }

        if(brak_zmian == _N)
        {
            pamwsk[0] = 0;
        }

        qsort(s_tab, TAB_SIZE_S, sizeof(int), compare_a);
    }

    printf("A finished:\n");
    for(int i = 0; i < TAB_SIZE_S; ++i)
        printf("%d ", s_tab[i]);
    printf("\n");

    shmdt(pamwsk);
    shmctl(pamiec, IPC_RMID, NULL);
    semctl(semafor, 0, IPC_RMID, 0);

    return 0;
}