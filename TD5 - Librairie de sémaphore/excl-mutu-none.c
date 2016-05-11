#include "semaph.h"
#define PROT 0666

int main(){

	int shmflag=PROT|IPC_CREAT; 
	key_t key=5854;
	// shmget cree un segment de memoire partagee et retourne son id
	int shmid;
	shmid = shmget(key,512,shmflag); // creation du segment memoire
	if(shmid==-1) perror("Creation echoue");

	int *ptr;
	ptr=shmat(shmid,0,shmflag); // attachement du segment

	int e=0;
	ptr[0]=e; // le segment se manipule sous forme de tableau 

	pid_t fils=fork();

	int i,j;

	switch (fils){
		case -1:
			perror("Probleme fork");
			break;	
		case 0: // fils	
			int a;
			int tps;
			for (i = 0; i < 100; i++)
			{
				a=ptr[0];
				tps=rand()%(100-20)+20; // genere un temps d'attente entre 20 et 100
				usleep(tps);
			}
			break;
		default: // pere
	}

	return 0;
}