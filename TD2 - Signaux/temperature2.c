#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>



void handler_fils() {
	printf("Temperature : 30\n");
	fflush(stdout);
}

void handler_pere(){
	printf("-");
	alarm(1);
	fflush(stdout);

}

int main (){

struct sigaction temp;
struct sigaction p; // renvoie vers le handler

	pid_t fils;
	fils=fork();
	switch (fils){
		case -1:
			printf("erreur\n");
			break;
		case 0: // fils
			temp.sa_handler=&handler_fils;
			sigaction(SIGUSR1,&temp,NULL);
			while (1) {
				sleep(1);
				raise(SIGUSR1);
				sigaction(SIGUSR1,&temp,&temp);

				
			}
			break;
		default: // pere, en premier generalement
			p.sa_handler=&handler_pere;
			sigaction(SIGALRM,&p,NULL); // renvoie vers la structure 
			alarm(1);
			printf("hello\n");
			while (1) // permet d'empecher le processus de se terminer
				sleep (1); // on utilise sleep car a la fin de chaque sleep, le processus se reveille et verifie s'il n'y a pas d'interruptions
			// en vrai il verifie a chaque ligne de code
		
	}
	return 0;
}