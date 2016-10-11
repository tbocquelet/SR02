#include "sharemem.h"
#define PROT 0666

////////////////
// QUESTION 2 //
////////////////

int main(){

	int shmflag=PROT|IPC_CREAT;
// chemin où la cle est stockee, a modifier en fonction de l'utilisateur 
	char* path=""; 
	int id=777;
	key_t key=ftok(path,id);

	int shmid;
	shmid = shmget(key,512,shmflag); // creation du segment memoire
	if(shmid==-1) perror("Creation echoue");

	int *ptr;
	ptr=shmat(shmid,0,shmflag); // attachement du segment memoire
	
	int tab[10]={1,1,1,1,1,1,1,1,1,1};
	int i;
	for(i=0;i<10;i++) ptr[i]=tab[i]; // chargements des donnees sur le segment memoire

	shmdt(ptr); // detachement du segment memoire

// execution du second programme "shrmem_r"
	char *path2="";
	char *arguments[] = {NULL};
	execv(path2,arguments);

	sleep(30);
	return 0;
}