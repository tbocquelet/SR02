#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

struct sigaction act_fils,act_pere;
pid_t pid_fils;
pid_t pid_pere;
int cpt_pere=1, cpt_fils=1;
int idx_pere=0, idx_fils=0;

char lettre_fils[26]="abcdefghijklmnopqrstuvwxyz";
char lettre_pere[26]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";


void handler_pere(int x)
{
	int i;
	for(i=0;i<cpt_pere;i++)
		{
		if (idx_pere<=25)
			{
			printf("%c",lettre_pere[idx_pere]);
			fflush(stdout);
			idx_pere++;
			}
		else
			{
			printf("\n");
			exit(0);
			}
		}
	cpt_pere++;
	kill(pid_fils,SIGUSR1);
}

void handler_fils(int x)
{
	int i;
	for(i=0;i<cpt_fils;i++)
		{
		if (idx_fils<=25)
			{
			printf("%c",lettre_fils[idx_fils]);
			fflush(stdout);
			idx_fils++;
			}
		else
			{
			kill(pid_pere,SIGUSR1);
			exit(0);
			}
		}
	cpt_fils++;
	kill(pid_pere,SIGUSR1);
}


int main(){
	pid_fils=fork();
	if(pid_fils==0){
		pid_pere=getppid();
		act_fils.sa_handler=handler_fils;
		sigaction(SIGUSR1,&act_fils,NULL);
		while(1){
			pause(); // on reste dans l'attente d'une interruption 
		}
	}
	else 
	{
		act_pere.sa_handler=handler_pere;
		sigaction(SIGUSR1,&act_pere,NULL);
		sleep(1);
		kill(pid_fils,SIGUSR1);
		while(1){
			pause();
		}		
	}
}
