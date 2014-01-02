#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
/*-------------------------------------------------------------------*/
#include "rpcparams.h"
static char bufor[BUFSIZE]; /*zmienna przechowujaca wiadomosc otrzymana*/
                            /*od procesu A oraz wysylana do niego*/
static int  ala = 0;        /*ala <> 0 oznacza, ze otrzymano wiadomosc*/
														/*od procesu A*/
/*-------------------------------------------------------------------*/
/*Funkcja wywolywana zdalnie sluzaca do przekazywania informacji */
/*od procesu A do procesu B */
/*Przesylana wiadomosc jest napisem (char *), wiec typem parametru */
/*tej funkcji jest wskaznik do napisu (char (*i)[])*/
static void get_string (char (*i)[]){
  Print (("otrzymany tekst: %s\n", *i));
  ala = 1;
}
/*-------------------------------------------------------------------*/
/*Funkcja wywolywana zdalnie sluzaca do przekazywania informacji */
/*od procesu B do procesu A */
/*Przesylana wiadomosc jest napisem (char *), wiec typem wyniku tej fukncji*/
/*jest wskaznik do napisu (char **) */
static char ** ret (void){
  if (ala)
    strcpy (bufor, "I co z tego?");
  else
    strcpy (bufor, "Co z ALA?");
  return ( (char **) (&bufor));
}
/*-------------------------------------------------------------------*/
int main(void){

  Print (("start B\n"));
	/*rejestracja funkcji wywolywanych zdalnie */
  assert (registerrpc(PROGNUM, VERSNUM, SENDNUM, (char*(*)())get_string, 
          (int(*)())xdr_napis, (int(*)())xdr_void) == 0);
  assert (registerrpc(PROGNUM, VERSNUM, RECVNUM, (char*(*)())ret, 
          (int(*)())xdr_void, (int(*)())xdr_napis) == 0);
	/*przejscie procesu w stan oczekiwania*/
  Print (("Start svc\n"));
  svc_run();
  return 0;
}
/*-------------------------------------------------------------------*/
