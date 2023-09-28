/* File:  
 *    pth_sem_bar.c
 *
 * Purpose:
 *    Use semaphore barriers to synchronize threads.
 *
 * Input:
 *    none
 * Output:
 *    Time for BARRIER_COUNT barriers
 *
 * Compile:
 *    gcc -g -Wall -o pth_sem_bar pth_sem_bar.c -lpthread
 *    timer.h needs to be available
 *
 * Usage:
 *    ./pth_sem_bar <thread_count>
 *
 * Note:
 *    Setting compile flag -DDEBUG will cause a message to be
 *    printed after completion of each barrier.
 *
 * IPP:   Section 4.8.2  (pp. 177  and ff.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "timer.h"

#define BARRIER_COUNT 100

int thread_count;
int counter; //contador de cuantos hilos han llegado
sem_t barrier_sems[BARRIER_COUNT]; //array de semaforos y cada semaforo se utiliza para sincronizar los hilos en una barrera en especifico
sem_t count_sem; //semaforo que se utiliza para controlar el acceso al contador

void Usage(char* prog_name);
void *Thread_work(void* rank);

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread, i;
   pthread_t* thread_handles; 
   double start, finish;

   if (argc != 2)
      Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);

   thread_handles = malloc (thread_count*sizeof(pthread_t));
   for (i = 0; i < BARRIER_COUNT; i++)
      sem_init(&barrier_sems[i], 0, 0);
   sem_init(&count_sem, 0, 1);

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], (pthread_attr_t*) NULL,
          Thread_work, (void*) thread);

   for (thread = 0; thread < thread_count; thread++) {
      pthread_join(thread_handles[thread], NULL);
   }
   GET_TIME(finish);
   printf("Elapsed time = %e seconds\n", finish - start);

   sem_destroy(&count_sem);
   for (i = 0; i < BARRIER_COUNT; i++)
      sem_destroy(&barrier_sems[i]);
   free(thread_handles);
   return 0;
}  /* main */


/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   exit(0);
}  /* Usage */


/*-------------------------------------------------------------------
 * Function:    Thread_work
 * Purpose:     Run BARRIER_COUNT barriers
 * In arg:      rank
 * Global var:  thread_count, count, barrier_sems, count_sem
 * Return val:  Ignored
 */
void *Thread_work(void* rank) {
#  ifdef DEBUG
   long my_rank = (long) rank;
#  endif
   int i, j;

   for (i = 0; i < BARRIER_COUNT; i++) {
      sem_wait(&count_sem); //asegurarse que el acceso al contador sea exclusivo
      if (counter == thread_count - 1) { //si es igual a thread_count-1 es que todos han llegado
         counter = 0; //si han llegado reinicia el counter
         sem_post(&count_sem); //desbloquea el semaforo
         for (j = 0; j < thread_count-1; j++)
            sem_post(&barrier_sems[i]); //desbloquean los semaforos de la barrera actual
      } else {
         counter++; //si no e igual se incrementa el contador
         sem_post(&count_sem);//se libera el semaforo del contador
         sem_wait(&barrier_sems[i]); //y se pone a esperar el semaforo de la barrera
      }
#     ifdef DEBUG
      if (my_rank == 0) {
         printf("All threads completed barrier %d\n", i);
         fflush(stdout);
      }
#     endif
   }

   return NULL;
}  /* Thread_work */
