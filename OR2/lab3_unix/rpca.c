// Marius Rejdak
// Informatyka, mgr, OS1

/*
Zadanie 11, Unix
Data wykonania ćwiczenia: 2014-01-08
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <rpc/rpc.h>
#include <string.h>

#include "rpcparams.h"
#include "utils.h"

static int s_tab[TAB_SIZE_S];

int main(int argc, char *argv[])
{
    srand(time(NULL));
    gen_rands(s_tab, TAB_SIZE_S);
    qsort(s_tab, TAB_SIZE_S, sizeof(int), compare_a);

    printf("A running...\n");
    debug_pring("Generated S", s_tab, TAB_SIZE_S);

    while(1)
    {
        msgbufor bufor_o, bufor_i;
        int brak_zmian = 0;

        for(int i = 0; i < _N; ++i)
            bufor_o.liczby[i] = s_tab[i];
        
        //debug_pring("to_x entry", bufor_o.liczby, _N);
        assert(callrpc(HOST, PROGNUM, VERSNUM, PROCNUM, (xdrproc_t)xdr_intarray, (char*)&bufor_o, (xdrproc_t)xdr_intarray, (char*)&bufor_i) == 0);
        //debug_pring("to_x end", bufor_i.liczby, _N);
        qsort(bufor_i.liczby, _N, sizeof(int), compare_a);

        for(int i = 0; i < _N; ++i)
        {
            if(s_tab[i] == bufor_i.liczby[i])
                ++brak_zmian;
            else
                s_tab[i] = bufor_i.liczby[i];
        }

        if(brak_zmian == _N)
        {
            break;
        }
        
        qsort(s_tab, TAB_SIZE_S, sizeof(int), compare_a);
    }

    printf("A finished:\n");
    for(int i = 0; i < TAB_SIZE_S; ++i)
        printf("%d ", s_tab[i]);
    printf("\n");

    return 0;
}