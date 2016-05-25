////////////////////////////////////////////////////////////////////////
// Camille Soetaert - Théo Bocquelet - Groupe du mercredi 16h30.18h30 //
////////////////////////////////////////////////////////////////////////

// Pour la compilation :
// gcc -o prod-conso prod-conso.c -L. -lsempv

#include "sem_pv.h"
#define PROT 0666

int sem_vide=0; // Représente le nombre d'emplacements libres dans le buffer - on attribue le sémaphore 0 à cette tâche
int sem_plein=1; // Représente le nombre de messages en attente dans le buffer - on attribue le sémaphore 1 à cette tâche

int main(){

	int shmflag=IPC_CREAT|PROT; 
	int shmid; // ID du segment de mémoire partagée
	shmid = shmget(IPC_PRIVATE,5*sizeof(int),shmflag); // Création d'un segment de mémoire partagée de taille 5 entiers
	
	if(shmid == -1) perror("Création échouée");
	else printf("Segment mémoire créé.\n");

	int *idx_w; // Index d'écriture
	int *idx_r; // Index de lecture
	idx_w=shmat(shmid,0,shmflag); // On attache le segment mémoire à l'index d'écriture
	idx_r=shmat(shmid,0,shmflag); // On attache le segment mémoire à l'index de lecture

	pid_t fils;

	// Variables du fils
	int i, j=0;
	// Variables du père
	int k, l=0;

	// Création des sémaphores
	init_semaphore();
	// Initialisation des sémaphores
	val_sem(sem_vide,5); // Au début, il y a 5 emplacements libres dans le buffer 
	val_sem(sem_plein,0); // Au début, il n'y a pas de message en attente

	switch (fils=fork()){
		case -1:
			perror("Probleme fork");
			break;	
		case 0: // Fils	- PRODUCTEUR
			for (i = 1; i < 51; i++) // Pour afficher 50 valeurs 
			{
				P(sem_vide); // On applique P au sémaphore comptant le nombre d'emplacement libres
				idx_w[j]=i;
				printf("Produit : %d\n", idx_w[j]);
				j++;
				j = j%5; // Le buffer est circulaire de taille 5, on accède uniquement aux cases 0 1 2 3 et 4
				V(sem_plein); // On applique V au sémaphore comptant le nombre de messages en attente
			}
			shmdt(idx_w); // Détachement du segment mémoire
			break; 
		default: // Père - CONSOMMATEUR
			for (k = 1; k < 51; k++)
			{
				P(sem_plein); // On applique P au sémaphore comptant le nombre de messages en attente
				printf("Consommé : %d\n", idx_r[l]);	
				l++;
				l = l%5; // Le buffer est circulaire de taille 5, on accède uniquement aux cases 0 1 2 3 et 4
				V(sem_vide); // On applique V au sémaphore comptant le nombre d'emplacement libres
			}
			wait(0); // On attend la fin du fils
			shmdt(idx_r); // Détachement du segment mémoire
			shmctl(shmid, IPC_RMID,0); // Destruction du segment mémoire
			printf("Segment mémoire supprimé.\n");
			detruire_semaphore();
	}
	return 0;
}

