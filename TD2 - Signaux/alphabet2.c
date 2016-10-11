#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

pid_t pid;
struct sigaction minuscule, majuscule;
int i=1,cpt_fils=0,cpt_pere=0;;

char lettre_fils[26]="abcdefghijklmnopqrstuvwxyz";
char lettre_pere[26]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// i donne le nombre de lettres a afficher

void handler_min(){
	//printf("Je suis ici. 1\n");
	int k;
	for(k=0;k<i;k++){
		if (cpt_fils<26){
			printf("%c",lettre_fils[cpt_fils]);
			fflush(stdout);
			cpt_fils++;
		}
	} 	
	if (cpt_fils<26) kill(getppid(),SIGUSR1);
	else {
		printf("\nTHE END.\n");
		exit(0);
	}
}


void handler_maj(){
	//printf("Je suis ici. 4\n");
	int k;
	for(k=0;k<i;k++){
		if (cpt_pere<26){
			printf("%c",lettre_pere[cpt_pere]);
			fflush(stdout);
			cpt_pere++;
		}
			
	} 
	if (cpt_fils<26) {
		i++;
		kill(pid,SIGUSR1);
	}
	else {
		printf("\nTHE END.\n");
		exit(0);
	}
}

int main() {

	pid=fork(); // pid a la valeur du fils apres le fork()
	switch (pid) {
		case -1: 
			printf("Erreur!\n");
			break;
		case 0: // on est dans le fils
			minuscule.sa_handler=&handler_min;
			sigaction(SIGUSR1,&minuscule,NULL);
			//kill(getppid(),SIGUSR1); 
			//printf("Je suis ici. 3\n");
			while(1)
				pause();
			break;
		default: // on est dans le pere
// on ne peut pas mettre de kill ici vers le fils car sigaction du fils n'est pas encore defini ! 
			majuscule.sa_handler=&handler_maj;
			sigaction(SIGUSR1,&majuscule,NULL);
			//printf("Je suis ici. 2\n");
			sleep(1); // permet de lancer le processus fils et de creer sigaction pour le fils
			kill(pid,SIGUSR1);
			while(1)
				pause();
	}
	return 0;
}






