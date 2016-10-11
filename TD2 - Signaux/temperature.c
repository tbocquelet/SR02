#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

struct sigaction act_thermometre,act_alarme;
pid_t pid;

void handler_thermometre(int x){
	int rnd=(random()/(RAND_MAX*1.0))*40;
	printf("Temperature = %d C°\n",rnd);
}

void handler_alarme(int x){
	kill(pid,SIGUSR1); // pour envoyer un signal a un autre processus 
	alarm(5);
}


int main(){
	pid=fork();
	if(pid==0){
	// processus fils : thermometre
		act_thermometre.sa_handler=handler_thermometre;
		sigaction(SIGUSR1,&act_thermometre,NULL);
		while(1){
			raise(SIGUSR1); // pour s'autoenvoyer un signal
			sleep(1);
			}
		}
	else 
	{
		// processus pere : mesure periodique
		act_alarme.sa_handler=handler_alarme;
		sigaction(SIGALRM,&act_alarme,NULL);
		alarm(5);
		while(1)
			{
			sleep(1);
			printf("-");
			fflush(stdout);
			}	
	}
}
