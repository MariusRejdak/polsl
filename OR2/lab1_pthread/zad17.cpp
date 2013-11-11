/* Autorzy: Marius Rejdak & Paweł Paduszyński */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define TYTON 1 
#define PAPIER 2
#define ZAPALKI 4

/* klasa reprezentujaca monitor dla problemu producenta i konsumenta */
class monitor
{
  //zasoby
  int zasoby;

  pthread_cond_t not_full;      /* uzyte do zatrzymania watku kiedy bufor jest pelny */
  pthread_cond_t not_empty;     /* uzyte do zatrzymania watku kiedy bufor jest pusty */
  pthread_mutex_t mutex;        /* muteks skojarzony ze zmiennymi warunkowymi */

public:
  monitor();                    
  ~monitor();
  void put(const int);
  int get();
  int check_and_get(const int);
};

/* konstruktor */
monitor::monitor()
{
  zasoby = 0;

  /* inicjacja muteksu domyslnymi atrybutami */
  pthread_mutex_init(&mutex, NULL);

  /* inicjacja zmiennych warunkowych domyslnymi atrybutami */
  pthread_cond_init(&not_full, NULL);
  pthread_cond_init(&not_empty, NULL);
}

/* destruktor */
monitor::~monitor()
{
  /* zniszczenie muteksu i zmiennych warunkowych */
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&not_full);
  pthread_cond_destroy(&not_empty);
}

/* metoda wstawia nowa wartosc do bufora */
void monitor::put(const int value)
{
  pthread_mutex_lock(&mutex);        /* zablokowanie muteksu */

  /* bufor jest pelny - czekanie na wolne miejsce */ 
  if (zasoby > 0)
    pthread_cond_wait(&not_full, &mutex);

  zasoby = value;

  /* sygnalizacja, ze pojawila sie nowa wartosc w buforze */
  pthread_cond_signal(&not_empty);   
  pthread_mutex_unlock(&mutex);      /* zwolnienie muteksu */
}

int monitor::check_and_get(const int zasob_palacza)
{
  int zasob_pobrany = 0;
  pthread_mutex_lock(&mutex);         /* zablokowanie muteksu */

  /* bufor jest pusty - czekanie na produkty w buforze */ 
  if (zasoby == 0)
    pthread_cond_wait(&not_empty, &mutex);

  if((zasob_palacza & zasoby) == 0) {
    zasob_pobrany = zasoby;
    zasoby = 0;
    pthread_cond_signal(&not_full);
  }
  else {
    pthread_cond_signal(&not_empty);
  }

  /* sygnalizacja, ze pojawilo sie wolne miejsce w buforze */
  pthread_mutex_unlock(&mutex);       /* zwolnienie muteksu */
  return zasob_pobrany;                       
}


/* klasa monitora */
monitor prod_cons_mon;        

/* watek producenta */
void *agent(void *arg)
{
  int zasoby = 0;
  do      
  {
    switch(rand()%3)
    {
      case 0: 
      zasoby = TYTON | PAPIER;
      break;
 
      case 1: 
      zasoby = TYTON | ZAPALKI;
      break;

      default: 
      zasoby = PAPIER | ZAPALKI;
    }

    printf("Położono na stół: %d\n", zasoby);
    prod_cons_mon.put(zasoby);  /* wstawienie wartosci do bufora */
    sleep(rand() % 5);         
  }
  while(true); 
 
  pthread_exit(NULL);         /* zakonczenie watku */
}

/* watek konsumenta */
void *palacz(void *arg)
{
  int &zasob_palacza = *reinterpret_cast<int*>(arg);
  int zasob_pobrany = 0;

  do    
  {
    zasob_pobrany = prod_cons_mon.check_and_get(zasob_palacza);

    if((zasob_pobrany | zasob_palacza) == (TYTON | PAPIER | ZAPALKI))
      printf("Smokin' :) %d\n", zasob_palacza); /* palenie */
    else if(zasob_pobrany != 0)
      printf("Tragic mistake :(\n"); /* pobrano zły zasób - nie powinno wystąpić, assert */

    sleep(rand() % 5);
  }
  while(true); 
 
  pthread_exit(NULL);              /* zakonczenie watku */
}

int main(int argc, char *argv[])
{
  /* identyfikatory watkow producenta i konsumenta */
  pthread_t prod_t1, prod_t2, prod_t3, cons_t;
  int zasob1 = TYTON;
  int zasob2 = PAPIER;
  int zasob3 = ZAPALKI;

  srand(time(0));             

  /* utworzenie watkow producenta i konsumenta */
  pthread_create(&prod_t1, NULL, palacz, &zasob1);
  pthread_create(&prod_t2, NULL, palacz, &zasob2);
  pthread_create(&prod_t3, NULL, palacz, &zasob3);
  pthread_create(&cons_t, NULL, agent, NULL);

  /* oczekiwanie na zakonczenie watkow producenta i konsumenta */
  pthread_join(prod_t1, NULL);
  pthread_join(prod_t2, NULL);
  pthread_join(prod_t3, NULL);
  pthread_join(cons_t, NULL);

  return 0;
}
