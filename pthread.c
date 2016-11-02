							/* Anirudh Iyengar*/
							/* U55826708*/
		/*Help regarding the usage of mutex_trylocks was derived from youtube, stackoverflow and man.yolinux.com*/ 
#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>

struct rusage mytiming;
struct timeval mytimeval;

/* Set up for Solaris. LOH 12/26/01. 
Tested on linux 1/23/11 */
/* compile with gcc -ofname thread-template.c -lpthread */

struct shared_dat
   {
   int value;     /* shared variable to store result*/
   };

struct shared_dat  *counter;
int temp;

pthread_mutex_t lock;	/*Initializing the lock*/
 
/****************************************************************
* This function increases the value of shared variable "counter"
  by one 1500000 times
****************************************************************/
void * thread1(void *arg)
{
	int line = 0;	
	while (line < 1500000)
	{
	if (!pthread_mutex_trylock(&lock))  	/*Checking for free mutex_lock*/
	
	   {
            line++;

/* Critical Section */
	    counter->value = counter->value + 1;
	    counter->value = counter->value * 2;
	    counter->value = counter->value / 2;
	pthread_mutex_unlock(&lock);		/*Unlocking whence leaving the critical section*/
	    }
	}
	   printf("from process1 counter  =  %d\n", counter->value); 
	   printf("Thread 1 complete!\n"); 
return(NULL);
}


/****************************************************************
This function increases the value of shared variable "counter"
by one 1500000 times
****************************************************************/
void * thread2(void *arg)
{
	int line = 0;	
	while (line < 1500000)
	   {
	if (!pthread_mutex_trylock(&lock)) 	/*Checking for free mutex_lock*/
	{
            line++;

/* Critical Section */
	    counter->value = counter->value + 1;
	    counter->value = counter->value * 2;
	    counter->value = counter->value / 2;
	pthread_mutex_unlock(&lock); 		/*Unlocking whence leaving the critical section*/
	  }
	}
	   printf("from process2 counter = %d\n", counter->value); 
	   printf("Thread 2 complete!\n"); 
return(NULL);
}


/****************************************************************
*                  Main Body                                    *
****************************************************************/
main()
{
        int             r=0;
	int 		i;
	int 		shmid;	  /* shared memory ID */
        pthread_t	tid1[1];     /* process id for thread 1 */
        pthread_t	tid2[1];     /* process id for thread 2 */
        pthread_attr_t	attr[1];     /* attribute pointer array */

         counter = (struct shared_dat *) malloc(sizeof(struct shared_dat));

	/* initialize shared memory to 0 */
	counter->value = 0 ;
           printf("1 - I am here %d in pid %d\n",r,getpid());
	fflush(stdout);

 /* Required to schedule thread independently.
 Otherwise use NULL in place of attr. */
        pthread_attr_init(&attr[0]);
        pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);  /* system-wide contention */ 

	/* end to schedule thread independently */

/* Create the threads */
        pthread_create(&tid1[0], &attr[0], thread1, NULL);
        pthread_create(&tid2[0], &attr[0], thread2, NULL);

/* Wait for the threads to finish */
    pthread_join(tid1[0], NULL);
    pthread_join(tid2[0], NULL);
	        printf("from parent counter  =  %d\n", counter->value);
		printf("---------------------------------------------------------------------------\n");
		printf("\t\t	End of simulation\n");

	/*Optional statement involving the termination of mutex_locks can be imposed*/

	getrusage(RUSAGE_SELF, &mytiming);
	printf("Time used is sec: %d, usec %d\n", mytiming.ru_utime.tv_sec, mytiming.ru_utime.tv_usec);		/*Statements involving the CPU runtime*/
	printf("System Time used is sec: %d, usec %d\n", mytiming.ru_stime.tv_sec, mytiming.ru_stime.tv_usec);


		exit(0);
		
}

