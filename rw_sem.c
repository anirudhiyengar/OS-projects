						            /* Anirudh Iyengar
			    					 U55826708
		              Help regarding the usage of semaphore was derived from youtube, stackoverflow  and the text book
				P.S: Writing formstion is mentioned in the comments above the writer section. Thanks.*/ 
#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <semaphore.h>


void *writer_thread(void *arg);
void *reader_thread(void *arg);
void relaxandspendtime();

struct shared_dat
   {
   int value;    					 		/* shared variable to store result*/
   };

struct shared_dat  *counter;						/* Shared Counter*/

int cnt =0;								/* Read Counter*/

int in_cs =0;								/* Initialize the CS fla(in_cs) */
sem_t r_sem, rw_sem, lock;						/*Initializing the various semaphores*/

/****************************************************************
* This function increases the value of shared variable "counter"
  ~ WRITER ~

   Depending on where the 2 semaphores (In/Out the while loop) are,
   either one write per cycle is completed or the entire write is 
   completed in one go. This seemed a little ambiguious to me, 
   after reading the posts on piazza and the one mentioned in the 
   assignment PDF. Kindly oblige.
****************************************************************/
void * writer_thread(void *arg)
{
	int val = 0;							/* write count*/
	 sem_wait(&rw_sem);         					/* rw_sem = semaphore shared between reader and writer*/
         sem_wait(&lock);  						/* lock = semaphore set by the reader to the writer to enter the CS */
	while (val < 25000)
	{                
	
	val = val +1;                                       
	/* Critical Section */	
	in_cs = 1;							/* Flag set during the write operation */
	    counter->value = counter->value + 1;
	in_cs = 0;  
        
	}  
	 sem_post(&rw_sem);						/* Unlock the semaphore after ONE WRITE */
	 sem_post(&lock);                        			/* Unlock teh semaphore and wait for the reader to allow another write after the reader queue is empty */
	   printf("Writing complete!, from writer, count  =  %d\n", counter->value); 
return(NULL);
}

/****************************************************************
* This function is the CS for the READER
****************************************************************/
void relaxandspendtime()	
{
int j;
for(j = 0; j < 250000000; j++){ j=j;
}
}

/****************************************************************
*                  READER                                       *
****************************************************************/
void * reader_thread(void *arg)
{
 int n = (int)arg; 							/* Issuing the void pointer to a variable to better understand the reader coming in (reader nomenclature) */
	
     sem_wait(&rw_sem);                          			/* Waits to see if the writer needs its turn, if so... it gets blocked here and wont allow new readers to enter */
     cnt = cnt + 1;       						/* Increments the read count depending on teh number of readers entering the critical section */
     
	if (cnt == 1)							/* first reader */
	         sem_wait(&lock);               			/* locks out the writer */
     sem_post(&rw_sem);   						/* Allows for the next reader to enter */
                       
     if (in_cs == 1)							/* If reader enters teh CS while writer is in its CS --> ERROR!! */
	{ printf("Reading ERROR\n");
	  return(NULL); }
     else
     /* Critical Section */
	relaxandspendtime();                         
     
     sem_wait(&r_sem);     						/* Semaphore used by the reader to increment the count atomically w.r.t the other readers */                 
     cnt = cnt - 1;       
     if (cnt == 0)							/* decrementing the reaader count after successful read, checking to see when there are no more readers in line */
        sem_post(&lock);              					/* Unlocking the writing semaphore to allow the writer to enter the CS */
     sem_post(&r_sem);                         				/* Unlocking the read semaphore to allow the next reader to make the change to the rad counter */
                                           
	printf("Reader %d Completed Reading\n", n);			/* Printing teh completeion of reading by the respective readers */
return(NULL);
}  
 
/*****************************************************************
*          		 Main Body                               *
*****************************************************************/

int main(int argc, char *argv[])
{
	int i, ii, k;							/* Internal Variables */

	int numOfReaders = atoi(argv[1]);				/* Assigning the number of readers from the command line to an internal variable */

	if (numOfReaders > 10 || numOfReaders < 1)			/* Checkign to see if the assigned input is valid i.e. between 1-10*/
	{	printf("Invalid Input!!\n");
		exit(0);
	} 
        int             r=0;
        pthread_attr_t	attr[1];   					  /* attribute pointer array */

        pthread_t  readers[numOfReaders]; 
        pthread_t  writer[1];
        counter = (struct shared_dat *) malloc(sizeof(struct shared_dat));

	/* initialize shared memory to 0 */
	counter->value = 0 ;
           printf("1 - I am here %d in pid %d\n",r,getpid());
	fflush(stdout);



 	/* Required to schedule thread independently.
	 Otherwise use NULL in place of attr. */
        pthread_attr_init(&attr[0]);
        pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM); 		/* system-wide contention */ 

	/* end to schedule thread independently */

	int x = sem_init(&r_sem, 0, 1);					/*Initialize the semaphore '0'= success, '1'= unsuccessful*/
	int y = sem_init(&rw_sem, 0, 1);				/*Initialize the semaphore '0'= success, '1'= unsuccessful*/
	int z = sem_init(&lock, 0, 1);					/*Initialize the semaphore '0'= success, '1'= unsuccessful*/


	/* Check to see if the semaphores have been initialized correctly */
	 if ((x == -1) || (y == -1) || (z == -1))
	{ 
		 printf("ERROR in Semaphore initialization\n");
		 exit(1); 
	}

	/* Create the threads */
	k = (numOfReaders/2);
	for(i = 0; i < k; i++){
	pthread_create(&readers[i], &attr[0], reader_thread, (void*) i);
	}
	/* Create the writer thread */
	pthread_create(&writer[0], &attr[0], writer_thread, NULL);

	/* Create the remainder of hte reader threads */
	for(i = k ; i < numOfReaders ; i++) {
	pthread_create(&readers[i], &attr[0], reader_thread, (void*) i);
	}

	/* Join the threads */
	for(ii = 0; ii < k; ii++){
	pthread_join(readers[ii], NULL);
	}
	/* Join the writer thread */
	pthread_join(writer[0], NULL);

	/* Join the remainder of the reader threads */
	for(ii = k ; ii < numOfReaders ; ii++) {
	pthread_join(readers[ii], NULL);
	}

	sem_destroy(&r_sem);						/* Destroy the semaphore*/
	sem_destroy(&rw_sem);						/* Destroy the semaphore*/
	sem_destroy(&lock);						/* Destroy the semaphore*/

		printf("---------------------------------------------------------------------------\n");
		printf("\t\t	End of simulation\n");

	exit(0);
}


