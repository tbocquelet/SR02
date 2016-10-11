#include "sharemem.h"
#define PROT 0666

////////////////
// QUESTION 2 //
////////////////

int main(){

	int shmflag=PROT|IPC_CREAT;
// chemin où la cle est stockee, a modifier en fonction de l'utilisateur 
	char* path="";
	int id=777; // meme id que dans shrmem_e
	key_t key=ftok(path,id);

	int shmid;
	shmid = shmget(key,512,shmflag); // creation du segment memoire
	if(shmid==-1) perror("Creation echoue");

	int *ptr;
	ptr=shmat(shmid,0,shmflag); // attachement du segment memoire
	
	int tab[10];
	int i;
	for(i=0;i<10;i++){
		tab[i]=ptr[i];
		printf("tab[%d] = %d \n",i,tab[i]);
	}

	shmdt(ptr); // detachement du segment memoire
	shmctl(shmid, IPC_RMID,0); // suppression du segment memoire
	printf("Segment memoire supprime.\n");
	return 0;
}