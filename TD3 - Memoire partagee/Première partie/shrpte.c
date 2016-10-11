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
	
	int id2=555; // changement d'id
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
	int id=666; // changement d'id par rapport aux programmes precedents
	key_t key=ftok(path,id);

	int shmid;
	shmid = shmget(key,512,shmflag);
	if(shmid==-1) perror("Creation echoue");

	int *ptr;
	ptr=shmat(shmid,0,shmflag); // attachement du segment memoire
	
	int i,j; // compteurs

	int *p[5]; // declaration d'un tableau de pointeurs 
// allocation dynamique du tableau
	for(i=0 ; i < 5 ; i++) {
		p[i] = malloc(3 * sizeof(int));
		if(p[i] == NULL){    //En cas d'erreur d'allocation
	        for(i=i-1 ; i >= 0 ; i--)  //Libération de l'espace déjà alloué
	             free(p[i]);

	        exit(EXIT_FAILURE);
       }
    }

// creation et initialisation des 5 vecteurs
	int vec1[3]={1,2,3};
	int vec2[3]={4,5,6};
	int vec3[3]={7,8,9};
	int vec4[3]={10,11,12};
	int vec5[3]={13,14,15};

// affectation du tableau de pointeurs avec les adresses du premier element de chaque vecteur
	p[0]=&vec1[0]; 
	p[1]=&vec2[0];
	p[2]=&vec3[0];
	p[3]=&vec4[0];
	p[4]=&vec5[0];
	
	printf("Adresse du premier element du tableau de pointeurs: %d\n", &p[0][0]);
	for(i=0;i<5;i++) printf("Element %d du tableau de pointeurs: %d\n",i,p[i]);

	for(i=0;i<5;i++){
		printf("Adresse du premier element du vecteur: %d\n", &p[i][0]);
		for(j=0;j<3;j++){
			printf("Vec[%d][%d]=%d\n",i,j,p[i][j]);
		}
	}

	for(i=0;i<5;i++) ptr[i]=p[i]; // on passe le tableau de pointeurs sur le segment de memoire partagee

// on passe les 5 vecteurs sur le segment de memoire partagee
	for(i=0;i<3;i++) ptr[i+5]=vec1[i];
	for(i=0;i<3;i++) ptr[i+8]=vec2[i];
	for(i=0;i<3;i++) ptr[i+11]=vec3[i];
	for(i=0;i<3;i++) ptr[i+14]=vec4[i];
	for(i=0;i<3;i++) ptr[i+17]=vec5[i];
		
	shmdt(ptr); // detachement du segment memoire
	shmdt(ptr2); // detachement du segment memoire non utilise	

// execution du second programme "shrptr"
	char *path2="";
	char *arguments[] = {NULL};
	execv(path2,arguments);

	sleep(40);

// liberation de la memoire allouee au tableau de pointeurs
	for(i=0 ; i < 5 ; i++) free(p[i]);
	
	return 0;
}