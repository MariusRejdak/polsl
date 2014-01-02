#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <rpc/rpc.h>

#define _N_ 7
#define _S_ 150
#define _T_ 200
#define HOST "localhost"
#define PROGNUM 31231231ul
#define VERSNUM 1ul
#define PROCNUM 1ul

typedef char *(*xdrproc_c)(char *);
int buffer[_N_] = {0}, tabS[_S_], tabT[_T_];

void gen_rands(int *tab, size_t size)
{
	for(size_t i = 0; i < size; ++i)
	{
		tab[i] = rand()%1000;
	}
}

bool_t xdr_int_array(XDR *xdrsp, int *a)
{
	unsigned int sizep = _N_;
	return xdr_array(xdrsp, (char**)&a, &sizep, sizep, sizeof(int), (xdrproc_t)xdr_int);
}

int** send_values(int (*i)[])
{
	return &buffer;
}

int main(int argc, char *argv[])
{
	int status, pid = 0;
	pid = fork();

	if(pid == 0)
	{
		srand(time(0)+pid);
		gen_rands(tabS, _S_);

		printf("Test 0\n");
	}
	else
	{
		int rpc_stat;
		srand(time(0)+pid);
		gen_rands(tabT, _T_);

		rpc_stat = registerrpc(PROGNUM, VERSNUM, PROCNUM, (xdrproc_c)send_values, (xdrproc_t)xdr_int_array, (xdrproc_t)xdr_int_array);
		assert(rpc_stat == 0);

		svc_run();
	}

	pid = wait(&status);
	return 0;
}