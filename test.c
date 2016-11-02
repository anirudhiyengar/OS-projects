											/*   ANIRUDH IYENGAR
											      U55826708
						Help regarding the project was obtained from youtube.com, stackoverflow.com and other similar forums      */

						/* WAS UNABLE TO USE FLOAT VALUES FOR SIMULATION, ALL THE CALCULATIONS ARE MADE USING INT DATA TYPE */
#include <stdlib.h> 
#include <sys/resource.h>
#include <stdio.h>
#include <sys/types.h>

#define Max 500								/* Assigning a Maximum temporary array size */

struct process	
{
	int p_id;
	int et;								/* Using a structure to hold the process id, Event time, arrival time, burst time and Process completion*/
	int at;
	int bt;
	int fin;
}temp;

int main(int argc, char* argv[]){					/* Obtaining input conditions from the commandline */

	/* Initializations of local variables using through the code */
int tmp[500];								/* Initializations of an array to store the values from the input file */
int a,tq,cst,ptr,tq1;							/* local variables used in the code */
int stime =0;								/* system time */
int flag = 0;								/* flag to denote if the process is currently under execution */
int asize = 0;								/* number of Rows in the input file i.e. number of processes */
int i = 0; 
int j = 0;
int fcnt = 0;								/* counter to check how many process have finished execution */
int complete =0;							/* Flag that denotes the end of Execution */
int ncs = 0;
int CPU_uti = 0;						

	/* File handling operations */
	FILE *fh;
	tq = atoi(argv[1]);						/* assigning Time Quantum read from the command line */
	cst = atof(argv[2]);						/* Assigning the Context switch time */
	fh = fopen(argv[3], "r");					/* File handler */

	/* Body of the Function */
	if(fh == NULL){							/* To ensure the file being read is valid and appropriate */
		printf("Unable to open file\n");
		exit(1);
	}
	while(fscanf(fh, "%d", &tmp[i]) != EOF){			/* Scanning the lines in the file and assigning the values to our temporary array */
		i++;
	}
	fclose(fh);							/* Close the Input file */
	asize = i / 4;							/* Number of processes i.e. Number of rows */	
	struct process p[asize];					/* construct a process of the type structure defined earlier */
	
	/* STORING THE VALUES READ FROM THE INPUT FILE (FROM THE 
		TEMP ARRAY) INTO THE RESPECTIVE ARRAY ELEMENTS */
	for(i = 0; i < (asize * 4); i = (i + 4)){
		p[j].p_id = tmp[i];
		p[j].et = tmp[i + 1];
		p[j].at = tmp[i + 2];
		p[j].bt = tmp[i + 3];
		p[j].fin = 0;
		j++;
	}
	
	/* sORTING THE STRUCTURE PROCESS ARRAY W.R.T THE ARRIVAL TIME */
	for (i = 0; i < asize ; i++){
		ptr = i;						/* set the minimum pointer */
		for(j = i; j < asize ; j++){
			if(p[ptr].at > p[j].at){
				ptr = j;				/* swap to the new minimum */
			}
			}
		temp = p[i];						/* Swapping section */
		p[i] = p[ptr];
		p[ptr] = temp;
	}
		
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
								 Main Body - ROUND ROBIN
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	i = 0;								/* Local Pointers */
	j = 0;
   while (!complete){							/* While hold true until the complete execution of all the processes */

	if (stime >= p[0].at)						/* Condition that is satisfied upon the arrival of the first process, else the system time just increments */
	{
		if (p[j].at == stime){					/* Condition to check the arrival of the processes */
		printf("Time %d P%d arrives\n",stime,p[j].p_id);
		j = (j+1)%asize;					/* Increment in a circular loop fashion */
		}
				
		if(flag  == 0){
		        tq1 = tq;					/* Setting the Time Quantum for every process to use during its execution */
		        if (p[i].fin == 0){
				ncs ++;	
				stime = stime + cst;			/* Adding context switch time when the process in the execution loop switches */
		    	printf("Time %d P%d runs\n",stime,p[i].p_id);	/* Print the current process under execution */
			}	
			while (!(p[i].bt == 0 || tq1 == 0)){		/* Execute as long as either the Burst time is 0 or the Time quantum has been used up */
			flag = 1;					/* Set the execution flag to not allow any other process to enter this loop */
			if (p[j].at == stime){				/* Concurrently we check to see if any other process has arrived */
			printf("Time %d P%d arrives\n",stime,p[j].p_id);
			printf("Time %d P%d runs\n",stime,p[i].p_id);
			j = (j+1)%asize;				/* Increment the process value and wait until the next process arrives */
			}
				p[i].bt = p[i].bt - 1;			/* EXECUTION : The Burst time is subtracted with every cycle of the system time */
				tq1--; 					/* Time quantum is decremented with each cycle */
				stime ++;				/* System Time is incremented for each cycle */
			}
			if ((p[i].bt == 0) && (p[i].fin == 0)){		/* Check to see if the burst time = 0 and set the process as finished by changing its finished flag */
				fcnt ++;				/* Counter that checks the number of processes that have finished */
				flag = 0;
				printf("Time %d P%d finishes\n",stime, p[i].p_id);
				p[i].fin = 1;
				i = (i+1)%asize;			/* Move to the next process in a cyclic fashion */
			}
			else{ 
				i = (i+1)%asize;			/* Move to the next process if the Time Quantum expires */
				flag = 0;				/* Set the flag to 0 to allow the next process to enter the execution loop */
			}
			if (fcnt == asize){				/* Check the number of process completed */
				complete = 1; }				/* If the number of process completed = total processes , exit the program */
		}	
	}
		
	else
		stime ++;						/* Increment the system time as long the System Time is not being incremented in the Process loops */
}
printf("No of context switches is = %d\n", ncs);			/* Number of context switches that occur */
CPU_uti = ((stime*100 - (ncs*cst + cst*p[0].at)*100)/stime);			/* CPU Utilization time */
printf(" Final system time is = %d\n", stime);				/* FinalTime read by the system clock */
printf(" CPU Utilization is = %d\n", CPU_uti);				/* CPU Utilization */

	return 0;
	
}

