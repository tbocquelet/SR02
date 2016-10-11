#include "sharemem.h"
#define PROT 0666

////////////////
// QUESTION 1 //
////////////////

int main()
{
// 1.1 - creation des tableaux de 10 entiers initialises avec des 1
	int tab1[10]={1,1,1,1,1,1,1,1,1,1};
	int tab2[10]={1,1,1,1,1,1,1,1,1,1};

// 1.2
	int shmflag=PROT|IPC_CREAT; 
	key_t key=5854;
// shmget cree un segment de memoire partagee et retourne son id
	int shmid;
	shmid = shmget(key,512,shmflag); // creation du segment memoire
	if(shmid==-1) perror("Creation echoue");

	int *ptr;
	ptr=shmat(shmid,0,shmflag); // attachement du segment
// La fonction shmat attache le segment de  mémoire  partagée identifié par shmid au segment de données du processus appelant.   

	int i;
	int *status; // pour le wait 
// 1.3
	pid_t fils=fork();
	switch (fils){
	case -1:
		perror("Probleme fork");
		break;	
	case 0: // fils	
		printf("Modification du tableau par le fils:\n");
		for(i=0;i<10;i++){
// 1.5
			tab2[i]=2;
// 1.6
			printf("Fils - tab2[%d]=%d\n",i,tab2[i]);
// 1.8
			ptr[i]=tab2[i];
		}
		printf("\n");
		break;
	default:
// 1.4
// usleep fait attendre le processus un certain nombre de microsecondes
		usleep(500);
// 1.7
		for(i=0;i<10;i++){
			printf("Pere - tab2[%d]=%d\n",i,tab2[i]);
		}
		printf("\n");
// 1.9
		usleep(500);	
// 1.10	
		printf("Recuperation depuis le segment de memoire partagee:\n");
		for(i=0;i<10;i++){
			tab2[i]=ptr[i];
// 1.11
			printf("Pere - tab2[%d]=%d\n",i,tab2[i]);
		}
// 1.12
		waitpid(fils,status,0);
// 1.14
		shmdt(ptr); // detachement du segment memoire
		shmctl(shmid, IPC_RMID,0); // destruction du segment memoire
		printf("Segment memoire supprime.\n");
		break;
	}
	return 0;
}
















