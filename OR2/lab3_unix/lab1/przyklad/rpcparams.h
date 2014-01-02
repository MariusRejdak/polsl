/*----------------------------------------------------------------*/
#define PRINT
#ifdef PRINT
#define Print(x) printf x
#else
#define Print(x)
#endif
/*---------------------------------------------------------------*/
#define BUFSIZE 30
#define HOST "localhost"
#define PROGNUM 31231231
#define VERSNUM 1
#define SENDNUM 1
#define RECVNUM 2
/*funkcja dokonujaca konwersji napisu z/do formatu XDR */
int xdr_napis (XDR *xdrsp, char t[]){
  return xdr_string (xdrsp, &t, 30);
}

