#include "sharemem.h"
#define PROT 0666

int main(){

	int shmflag=PROT|IPC_CREAT;
// chemin où la cle est stockee, a modifier en fonction de l'utilisateur 
	char* path="";

	////////////////
	// QUESTION 4 //
	////////////////
	
// mappage du segment memoire non utilise
	
	int id2=555; // on remet les id correspondants a chaque segment memoire
	key_t key2=ftok(path,id2);

	int shmid2;
	shmid2 = shmget(key2,512,shmflag); // creation du segment memoire
	if(shmid2==-1) perror("Creation echoue");

	int *ptr2;
	ptr2=shmat(shmid2,0,shmflag); // attachement du segment memoire

	////////////////
	// QUESTION 3 //
	////////////////

// segment memoire utilise pour le transfert du tableau de pointeurs 
	int id=666; // on remet les id correspondants a chaque segment memoire
	key_t key=ftok(path,id);

	int shmid;
	shmid = shmget(key,512,shmflag); // creation du segment memoire
	if(shmid==-1) perror("Creation echoue");

	int *ptr;
	ptr=shmat(shmid,0,shmflag); // attachement du segment memoire
	
	int i,j; // compteurs

	int *p[5];// declaration d'un tableau de pointeurs 
// allocation dynamique du tableau
	for(i=0 ; i < 5 ; i++) {
		p[i] = malloc(3 * sizeof(int));
		if(p[i] == NULL){    //En cas d'erreur d'allocation
	        for(i=i-1 ; i >= 0 ; i--)  //Libération de l'espace déjà alloué
	             free(p[i]);

	        exit(EXIT_FAILURE);
       }
    }

	for(i=0;i<10;i++) p[i]=ptr[i]; // on recupere le tableau de pointeurs depuis le segment de memoire partagee

	printf("Adresse du premier element du tableau de pointeurs: %d\n", &p[0][0]);
	for(i=0;i<5;i++) printf("Element %d du tableau de pointeurs: %d\n",i,p[i]);

	for(i=0;i<5;i++){
		printf("Adresse du premier element du vecteur: %d\n", &p[i][0]);
		for(j=0;j<3;j++){
			printf("Vec[%d][%d]=%d\n",i,j,p[i][j]);
		}
	}

	shmdt(ptr); // detachement du segment memoire
	shmctl(shmid, IPC_RMID,0); // suppression du segment memoire

	shmdt(ptr2); // detachement du segment memoire non utilise	
	shmctl(shmid2, IPC_RMID,0); // suppression du segment memoire non utilise 

// liberation de la memoire allouee au tableau de pointeurs
	for(i=0 ; i < 5 ; i++) free(p[i]);

	return 0;
}