////////////////////////////////////////////////////////////////////////
// Camille Soetaert - Théo Bocquelet - Groupe du mercredi 16h30.18h30 //
////////////////////////////////////////////////////////////////////////

// Pour la compilation :
// gcc -o excl-mutu excl-mutu.c -L. -lsempv

#include "sem_pv.h"
#define PROT 0666

int main(){

	int shmflag=PROT|IPC_CREAT; 
	int shmid; // ID du segment de mémoire partagée
	shmid = shmget(IPC_PRIVATE,512,shmflag); // Création du segment memoire
	
	if(shmid == -1) perror("Création échouée");
	else printf("Segment mémoire créé.\n");

	int *ptr;
	ptr=shmat(shmid,0,shmflag); // Attachement du segment

	int E=0; 
	ptr[0]=E; 

	int i,j;
// Variables du fils
	int a;
	int tps;
// Variables du père
	int A;
	int TPS;

// Création d'un sémaphore mutex
	init_semaphore();
	int mutex=0; // On attribue le sémaphore 0 du groupe

// On initialise mutex à 1 
	val_sem(mutex,1);

	pid_t fils;

	switch (fils=fork()){
		case -1:
			perror("Probleme fork");
			break;	
		
		case 0: // Fils	
			for (i = 0; i < 100; i++)
			{
				P(mutex); // On applique l'opération P à mutex
				// Début de la section critique
				a=ptr[0];
				tps=rand()%(100-20)+20; // Génère un entier entre 20 et 100
				usleep(tps);
				a++;
				ptr[0]=a;
				tps=rand()%(100-20)+20; 
				// Fin de la section critique
				V(mutex); // On applique l'opération V à mutex
				usleep(tps);
			} 
			shmdt(ptr); // Détachement du segment mémoire
			break;
		
		default: // Père
			for (j = 0; j < 100; j++)
			{
				P(mutex); 
				// Début de la section critique
				A=ptr[0];
				TPS=rand()%(100-20)+20; 
				usleep(TPS);
				A++;
				ptr[0]=A;
				TPS=rand()%(100-20)+20; 
				// Fin de la section critique
				V(mutex);
				usleep(TPS);
				printf("Iteration %d | E=%d\n",j,ptr[0]);
			}
			wait(0); // On attend la fin du fils
			shmdt(ptr); // Détachement du segment mémoire
			shmctl(shmid, IPC_RMID,0); // Destruction du segment mémoire
			printf("Segment mémoire supprimé.\n");
			detruire_semaphore(); // Destruction du sémaphore
	}

	return 0;
}