#include <assert.h>
#include <stdio.h>
#include <rpc/rpc.h>
#include <string.h>
/*----------------------------------------------------------------*/
#include "rpcparams.h"
/*----------------------------------------------------------------*/
int main(void){
  char bufor[BUFSIZE];

  Print (("Start A - klienta\n"));
  strcpy (bufor, "ALA MA KOTA");

  Print (("Wysylanie wiadomosci do B\n"));
  assert ( callrpc(HOST, PROGNUM, VERSNUM, SENDNUM, (xdrproc_t)xdr_napis, 
                  bufor, (xdrproc_t)xdr_void, NULL) == 0);

  Print (("Odbieranie wiadomosci od B\n"));
  assert (callrpc(HOST, PROGNUM, VERSNUM, RECVNUM, (xdrproc_t)xdr_void, NULL,
          (xdrproc_t)xdr_napis, bufor) == 0);

  Print (("Tekst odebrany od B: %s\n", bufor));

  return 0;

}
/*----------------------------------------------------------------*/
