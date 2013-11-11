/* Autorzy: Marius Rejdak & Paweł Paduszyński */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 10     /* rozmiar bufora */
#define COUNT_K 5   /* ilość konsumentów */

int buffer[SIZE];   /* bufor */
sem_t buffer_producer[SIZE]; /* semafor dla producenta (czekanie na zakońcenie czytania) */
sem_t buffer_consument[SIZE]; /* semafor dla konsumenta (czekanie na zakończenie zapisu) */
int licznik = 0; /* aktualny element producenta (cyklicznie) */
int licznik_k[COUNT_K]; /* aktualny element i-tego konsumenta (cyklicznie) */
int konsument_nr[COUNT_K];

void insert_to_buffer(int value)
{
  for(int i = 0; i < COUNT_K; i++)
  {
    sem_wait(buffer_producer+licznik); /* oczekiwanie na zakończenie czytania tego elementu przez konsumentów */
  }
  buffer[licznik] = value; /* zapis wartości do bufora */
  printf("Zapisano do elementu %d: %d\n", licznik, value);
  for(int i = 0; i < COUNT_K; i++)
  {
    sem_post(buffer_consument+licznik); /* sygnał dla konsumentów że można odczytać ten element */
  }
  
  if(++licznik >= SIZE) {
    licznik = 0; /* cykliczny licznik */
  }
}

int read_from_buffer(int i)
{
  int value;
  sem_wait(buffer_consument+i); /* oczekiwanie na zapis wartości w te miejsce */
  value = buffer[i]; /* odczyt wartości */
  printf("Odczytano z elementu %d: %d\n", i, value);
  sem_post(buffer_producer+i); /* wątek odczytał wartość */
  return value;
}

/* watek producenta */
void *producer(void *arg)
{
  int value;

  do 
  {
    value = rand() % 100;     /* wyprodukowanie wartosci */
    insert_to_buffer(value);
    sleep(rand() % 5);        
  }
  while(value); 

  pthread_exit(NULL);         /* zakonczenie watku */
}

/* watek konsumenta */
void *consumer(void *arg)
{
  int value;
  int konsument_i = (int)arg;

  do 
  {
    value = read_from_buffer(licznik_k[konsument_i]);
    
    if(++licznik_k[konsument_i] >= SIZE)
      licznik_k[konsument_i] = 0; /* cykliczny licznik */

    sleep(rand() % 5);        
  }
  while(value); 

  pthread_exit(NULL);         /* zakonczenie watku */
}

int main(int argc, char *argv[])
{
  /* identyfikatory watkow producenta i konsumenta */
  pthread_t prod_t, cons_t;

  srand(time(0));             

  /* inicjacja semaforow */
  for(int i = 0; i < SIZE; ++i) {
    sem_init(buffer_producer+i, 0, COUNT_K);
    sem_init(buffer_consument+i, 0, 0);
  }

  pthread_create(&prod_t, NULL, producer, NULL);
  for(int i = 0; i < COUNT_K; ++i) {
    konsument_nr[i] = i;
    licznik_k[i] = 0;
    pthread_create(&cons_t, NULL, consumer, (void*)konsument_nr[i]);
  }  

  /* oczekiwanie na zakonczenie watkow producenta i konsumenta */
  pthread_join(prod_t, NULL);
  for(int i = 0; i < COUNT_K; ++i) {
      pthread_join(cons_t, NULL);
  }  

  /* zniszczenie semaforow */
  for(int i = 0; i < SIZE; ++i) {
    sem_destroy(buffer_producer+i);
    sem_destroy(buffer_consument+i);
  }

  return 0;
}
