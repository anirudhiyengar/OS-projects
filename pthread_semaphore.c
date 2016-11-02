						            /* Anirudh Iyengar
			    					 U55826708
		              Help regarding the usage of semaphore was derived from youtube, stackoverflow  and man.yolinux.com*/ 
#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <semaphore.h>

struct rusage mytiming;
struct timeval mytimeval;

/* Set up for Solaris. LOH 12/26/01. 
Tested on linux 1/23/11 */
/* compile with gcc -ofname thread-template.c -lpthread */

struct shared_dat
   {
   int value;    					 /* shared variable to store result*/
   };

struct shared_dat  *counter;
int temp;

sem_t lock;						/*Initializing the semaphore lock (variable)*/
 
/****************************************************************
* This function increases the value of shared variable "counter"
  by one 1500000 times
****************************************************************/
void * thread1(void *arg)
{
	int line = 0;	
	while (line < 1500000)
	{
	sem_wait(&lock);				/*locks the semaphore*/
	
            line++;

	/* Critical Section */
	    counter->value = counter->value + 1;
	    counter->value = counter->value * 2;
	    counter->value = counter->value / 2;

	sem_post(&lock);				/*unlocks the semaphore*/
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
	sem_wait(&lock);				/*locks the semaphore*/

            line++;

	/* Critical Section */
	    counter->value = counter->value + 1;
	    counter->value = counter->value * 2;
	    counter->value = counter->value / 2;

	sem_post(&lock);				/*unlocks the semaphore*/
	}
	   printf("from process2 counter = %d\n", counter->value); 
	   printf("Thread 2 complete!\n"); 
return(NULL);
}

/****************************************************************
* This function increases the value of shared variable "counter"
  by one 1500000 times
****************************************************************/
void * thread3(void *arg)
{
	int line = 0;	
	while (line < 1500000)
	{
	sem_wait(&lock);				/*locks the semaphore*/
	
            line++;

	/* Critical Section */
	    counter->value = counter->value + 1;
	    counter->value = counter->value * 2;
	    counter->value = counter->value / 2;

	sem_post(&lock);				/*unlocks the semaphore*/
	}
	   printf("from process3 counter  =  %d\n", counter->value); 
	   printf("Thread 3 complete!\n"); 
return(NULL);
}


/****************************************************************
*                  Main Body                                    *
****************************************************************/
main()
{
        int             r=0;
	int 		i;
	int 		shmid;	     /* shared memory ID */
        pthread_t	tid1[1];     /* process id for thread 1 */
        pthread_t	tid2[1];     /* process id for thread 2 */
        pthread_t	tid3[1];     /* process id for thread 3 */
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

	sem_init(&lock, 0, 1);					/*Initialize the semaphore '0'= success, '1'= unsuccessful*/

	/* if (sem_init(&lock, 0, 1) == -1),  printf("error");, exit(1); 

	/* Create the threads */
        pthread_create(&tid1[0], &attr[0], thread1, NULL);
        pthread_create(&tid2[0], &attr[0], thread2, NULL);	/*Create threads*/
        pthread_create(&tid3[0], &attr[0], thread3, NULL);

	/* Wait for the threads to finish */
   	 pthread_join(tid1[0], NULL);
   	 pthread_join(tid2[0], NULL);				/*Join Thread*/
   	 pthread_join(tid3[0], NULL);

	sem_destroy(&lock);					/* Destroy the semaphore*/

	        printf("from parent counter  =  %d\n", counter->value);
		printf("---------------------------------------------------------------------------\n");
		printf("\t\t	End of simulation\n");

	/*Optional statement involving the termination of mutex_locks can be imposed*/

	getrusage(RUSAGE_SELF, &mytiming);
	printf("Time used is sec: %d, usec %d\n", mytiming.ru_utime.tv_sec, mytiming.ru_utime.tv_usec);		/*Statements involving the CPU runtime*/
	printf("System Time used is sec: %d, usec %d\n", mytiming.ru_stime.tv_sec, mytiming.ru_stime.tv_usec);


	exit(0);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
	The user level time and the system level time for 2 thereads using mutex solution and sempahore solution are shown below. 
	
	MUTEX	
	
	1 - I am here 0 in pid 8038
	from process2 counter = 1858920
	Thread 2 complete!
	from process1 counter  =  3000000
	Thread 1 complete!
	from parent counter  =  3000000
	---------------------------------------------------------------------------
				End of simulation
	Time used is sec: 0, usec 532918
	System Time used is sec: 0, usec 1999

	SEMAPHORE

	1 - I am here 0 in pid 8146
	from process2 counter = 2078816
	Thread 2 complete!
	from process1 counter  =  3000000
	Thread 1 complete!
	from parent counter  =  3000000
	---------------------------------------------------------------------------
				End of simulation
	Time used is sec: 0, usec 303953
	System Time used is sec: 0, usec 18997


	  ** After repeated execution of the previous(mutex_trylock) code w.r.t the current code (semaphores), we can see a clear distinction between the mutex usage and semaphore usege for a critical
	     section problem. 

	  ** The User time for mutexes is larger due to its larger wait, i.e. (trylock--> spinlock). However, in case of semaphore, the thread is made to sleep, thus reducing the block waiting. 

	  ** On the other hand, the system time in case of mutex is much smaller as it doesnt incurr any context switching. In case of semaphore based solution, as the thread si made to
	     sleep when inactive, it needs to be woken up on activation, thus causing the need for context switching. 

	  ** Both these methods have their pros and cons. It all depends on the critical section problem to determine the most effective solution to the critical section.
   
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

