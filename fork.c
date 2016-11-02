							/* Name : Anirudh Iyengar*/
							/* U55826708*/
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int n;
char str[100];

int main(int argc, char *argv[])
{
	pid_t pid;  										/* Process ID	*/
	
	n = atoi(argv[1]);
	if ((n > 35) || (n < 1)){								/* Ensuring the number is between 1 and 35*/
		sprintf(str, "Invalid Entry! Please enter a number between 1 & 35");
		puts(str);
		return 0;
		}
	else {	

	pid = fork();										/* Forking first child*/
	
	if (pid < 0){
		fprintf(stderr, "Fork Failed");
		return 0;	
	}
	else if (pid == 0){									/* Child 1*/
		int a;
		a = n;
		printf("From Child 1 init n= %d, ",a);						/* Child One Initial value*/
	
		while ( a > 0){									/* Run until the number reduces to 1*/
			if (a == 1){							
				printf("\n");
				printf("One Done!\n");
				return 0;
			}
			else {
				if (a%2 == 0){							/* Even*/
					a = a/2;
					printf("From Child 1 n = %d, ", a);		
				}
				else {								/* Odd */
					a = 3*a + 1;
					printf("From Child 1 n = %d, ", a);			
				}
			}
		}
	}
	
	else {											/* Parent process*/
		pid = fork();									/* Fork next child (Child 2)*/
		
		if (pid < 0){
			fprintf(stderr, "Fork Failed");
			return 0;	
		}
		else if (pid == 0){								/* Child 2 */
			int b;
			b = n + 3;
			printf("From Child 2 init n = %d, ",b);					/*Child 2 intial value*/

			while ( b > 0){
				if (b == 1){
					printf("\n");
					printf("Children Complete!\n");
					return 0;
				}
				else {
					if (b%2 == 0){						/* Even */
						b = b/2;
						printf("From Child 2 n = %d, ", b);		
					}
					else {							/* Odd */
						b = 3*b + 1;
						printf("From Child 2 n = %d, ", b);			
					}
				}
			}
			return 0;
		}
		else {
		wait(NULL);
		wait(NULL);									/* wait for child2 to fin*/
		}
	}	
}
}
