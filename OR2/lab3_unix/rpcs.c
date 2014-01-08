// Marius Rejdak
// Informatyka, mgr, OS1

/*
Zadanie 11, Unix
Data wykonania ćwiczenia: 2014-01-08
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <rpc/rpc.h>
#include <string.h>

#include "rpcparams.h"
#include "utils.h"

static int t_tab[TAB_SIZE_T+_N];

void siginthandler(int param)
{
    printf("B finished:\n");
    for(int i = _N; i < TAB_SIZE_T+_N; ++i)
            printf("%d ", t_tab[i]);
    printf("\n");
    exit(0);
}

msgbufor* to_x(msgbufor *bufe)
{
    memcpy(t_tab, bufe->liczby, _N_SIZE);
    qsort(t_tab, TAB_SIZE_T+_N, sizeof(int), compare_b);
    //debug_pring("to_x entry", bufe->liczby, _N);

    static msgbufor buf;
    memcpy(buf.liczby, t_tab, _N_SIZE);
    qsort(buf.liczby, _N, sizeof(int), compare_a);
    //debug_pring("to_x end", buf.liczby, _N);

    return &buf;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    gen_rands(t_tab+_N, TAB_SIZE_T);
    qsort(t_tab+_N, TAB_SIZE_T, sizeof(int), compare_b);

    debug_pring("Generated T", t_tab+_N, TAB_SIZE_T);

    assert(registerrpc(PROGNUM, VERSNUM, PROCNUM, (xdrproc_c)to_x, (xdrproc_t)xdr_intarray, (xdrproc_t)xdr_intarray) == 0);
    
    signal(SIGINT, siginthandler);
    printf("B waiting...\n");
    svc_run();

    return 0;
}