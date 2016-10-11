#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int n_fils = 0 , n_pere = 0;

void fonc_pere();
void fonc_fils();
void captpere();
void captfils();

struct sigaction sigact_pere, sigact_fils;

int main() {

	int n;

	n=fork();

	switch(n){
		case -1 : 
			printf("\n erreur dans fork\n");
			break;
		case  0 :
			fonc_fils();
			break;
		default : 
			fonc_pere();
		}
	return 0;
}

/*-------------------------------------------------*/

void fonc_pere(){

	int n;
	sigact_pere.sa_handler=captpere;
	sigaction(SIGINT,&sigact_pere,NULL);
	printf("\n PERE: \n");

	while(1)
		{
			n=sleep(10);
			printf("\n Le temps restant = %d \n",n);
		}
}

/*-------------------------------------------------*/

void fonc_fils(){

	int i=0;
	initrec();
	sigact_fils.sa_handler=captfils;
	sigaction(SIGINT,&sigact_fils,NULL);

	while(i != -1){
		i = attendreclic();
		if (i == 0) {
			printf("\n FILS envoit au PERE\n");
			kill(getppid(), SIGINT);
		}
		printf("\n FILS :  pid du mon pere est : %d\n",getppid());
	}
	printf("\n FILS :fin du fils apres clic sur FIN\n");
}

/*-------------------------------------------------*/

void captfils(){
	n_fils++;
	printf("\n FILS %d : signal %d recu \n",getpid(), n_fils);
	rectvert(2);
	if  (n_fils == 3) {
		printf("\n FILS : fin du fils , trois signaux sont deja recu pid du mon pere est : %d\n",getppid());
		exit(1);

	}
}

/*-------------------------------------------------*/

void captpere(){
	n_pere++;
	printf("\n PERE %d : signal %d recu \n", getpid(),n_pere);
	if  (n_pere == 3) {
		printf("\n PERE : fin du pere , trois signals sont deja recu \n ");
		exit(2);
	}
}

