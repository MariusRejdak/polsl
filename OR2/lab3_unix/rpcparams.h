#define TAB_SIZE_S 100
#define TAB_SIZE_T 100
#define _N 10
#define _N_SIZE (_N*sizeof(int))

#define HOST "localhost"
#define PROGNUM 1337UL
#define VERSNUM 1UL
#define PROCNUM 1UL

typedef char *(*xdrproc_c)(char *);

typedef struct {
	int liczby[_N]; 
} msgbufor;

int xdr_intarray(XDR *xdrsp, msgbufor *data)
{
	return xdr_vector(xdrsp, (char*)data->liczby, 10, sizeof(int), (xdrproc_t)xdr_int);
}
