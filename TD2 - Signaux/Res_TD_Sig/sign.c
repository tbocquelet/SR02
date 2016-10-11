#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
int n_fils1 = 0 ,n_fils2 = 0 , n_pere = 0;

int lespid[3]={0,0,0};

void fonc_pere();
void fonc_fils1();
void fonc_fils2();
void captpere();
void captfils1();
void captfils2();

struct sigaction sigact_pere, sigact_fils1, sigact_fils2;

int main() {

	int n ,p;

	lespid[0]=getpid();

	n=fork();

	switch(n){
		case -1 : 
			printf("\n erreur dans fork1\n");
			break;
		case  0 : 
			lespid[1]=getpid();
			p=fork();
			switch(p){
				case -1 :
					printf("\n erreur dans fork2\n");
					break;
				case  0 :/*lespid[0]=getpid()
					 lespid[1]=getppid(); */
					 lespid[2]=getpid();
					 fonc_fils2();
					 break;
				default : 
					lespid[2]=p ; 
					fonc_fils1();
			}
		default : 
			lespid[1]=n ; 
			fonc_pere();
		}
	return 0;
}



/*-------------------------------------------------*/

void fonc_pere(){
	sigact_pere.sa_handler=captpere;
	sigaction(SIGINT,&sigact_pere,NULL);
	while(1);
}

/*-------------------------------------------------*/

void captpere(){
	n_pere++;
	printf("\n PERE %d : signal %d recu \n", getpid(),n_pere);
	if (n_pere == 10){
		printf("\n PERE : 10 SIGINT RECU, FIN PERE/n");
		exit(3);
	}
}



/*-------------------------------------------------*/

void fonc_fils1(){

	int i=0,j;
	sigact_fils1.sa_handler=captfils1;
	sigaction(SIGINT,&sigact_fils1,NULL);
	initrec();

	while(i != -1){
		i = attendreclic();
		printf("\n retour de clic = %d",i);
		switch(i){

			case 0  : 
				kill(lespid[0], SIGINT);
				break;
			case 1  : 
				kill(lespid[1], SIGINT);
				break;
			case 2  : 
				kill(lespid[2], SIGINT);
				break;
			case 3  : 
				kill(lespid[2], SIGINT);
				kill(lespid[0], SIGINT);
				kill(lespid[1], SIGINT);
				break;
			default : 
				break;

		}
	}
	printf("\n FILS1 :fin du fils1 apres clic sur FIN\n");
	exit(1);
}



/*-------------------------------------------------*/

void captfils1(){
	n_fils1++;
	printf("\n FILS1 %d : signal %d recu \n",getpid(), n_fils1);
	rectvert(2);
}



/*-------------------------------------------------*/

void fonc_fils2(){
	int k=0,j;
	initrec();
	sigact_fils2.sa_handler=captfils2;
	sigaction(SIGINT,&sigact_fils2,NULL);

	while(k != -1){
		k = attendreclic();
		printf("\n retour de clic = %d",k);
		switch(k){
			case 0  : 
				kill(lespid[0], SIGINT);
				break;
			case 1  : 
				kill(lespid[1], SIGINT);
				break;
			case 2  : 
				kill(lespid[2], SIGINT);
				break;
			case 3  : 
				for (j=0; j<3; j++) kill(lespid[j], SIGINT);
				break;
			default : break;
		}

		

	}
	printf("\n FILS2 :fin du fils2 apres clic sur FIN\n");
	exit(5);
}



/*-------------------------------------------------*/

void captfils2(){
	n_fils2++;
	printf("\n FILS2  %d : signal %d recu \n",getpid(), n_fils2);
	rectvert(2);
}

