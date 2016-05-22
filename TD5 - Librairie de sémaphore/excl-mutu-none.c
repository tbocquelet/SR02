#include "semaph.h"
#define PROT 0666

int main(){

	int shmflag=PROT|IPC_CREAT; 
	// shmget cree un segment de memoire partagee et retourne son id
	int shmid;
	shmid = shmget(IPC_PRIVATE,512,shmflag); // creation du segment memoire
	if(shmid == -1) perror("Creation echoue");

	int *ptr;
	ptr=shmat(shmid,0,shmflag); // attachement du segment

	int E=0;
	ptr[0]=E; // le segment se manipule sous forme de tableau 


	int i,j;
// variables du fils
	int a;
	int tps;
// variables du pere
	int A;
	int TPS;

	pid_t fils;

	switch (fils=fork()){
		case -1:
			perror("Probleme fork");
			break;	
		case 0: // fils	
			for (i = 0; i < 100; i++)
			{
				a=ptr[0];
				tps=rand()%(100-20)+20; // genere un temps d'attente entre 20 et 100
				usleep(tps);
				a++;
				ptr[0]=a;
				tps=rand()%(100-20)+20; 
				usleep(tps);
			}
			shmdt(ptr);
			break;
		default: // pere
			for (j = 0; j < 100; j++)
			{
				A=ptr[0];
				TPS=rand()%(100-20)+20; 
				usleep(TPS);
				A++;
				ptr[0]=A;
				TPS=rand()%(100-20)+20; 
				usleep(TPS);
				printf("Iteration %d | E=%d\n",j,ptr[0]);
			}
			wait(0); // on attend la fin du fils
			shmdt(ptr); // detachement du segment memoire
			shmctl(shmid, IPC_RMID,0); // destruction du segment memoire
			printf("Segment memoire supprime.\n");
	}

	return 0;
}