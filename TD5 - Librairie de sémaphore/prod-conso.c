// Pour la compilation :
// gcc -o prod-conso prod-conso.c -L. -lsempv

#include "sem_pv.h"
#define PROT 0666

int nbVide=5;  // nombre d'emplacements libres dans la memoire
int nbPlein=0; // nombre de messages en attente 

int sem_vide=0;
int sem_plein=1;

int main(){

	int shmflag=PROT|IPC_CREAT; 
	// shmget cree un segment de memoire partagee et retourne son id
	int shmid;
	shmid = shmget(IPC_PRIVATE,5*sizeof(int),shmflag); // creation du segment memoire de taille 5 entiers
	if(shmid == -1) perror("Creation echoue");

	int *idx_w; // index d'ecriture
	int *idx_r; // index de lecture
	idx_w=shmat(shmid,0,shmflag);
	idx_r=shmat(shmid,0,shmflag);

	pid_t fils;
	int i, j=0, k, l=0, var;

	init_semaphore();
	// initialisation
	val_sem(sem_vide,nbVide); 
	val_sem(sem_plein,nbPlein); 

	switch (fils=fork()){
		case -1:
			perror("Probleme fork");
			break;	
		case 0: // fils	- producteur
			for (i = 1; i < 51; i++)
			{
				// var = rand()%(50-1)+1;
				P(sem_vide);
				idx_w[j]=i;
				printf("Ajoutee : %d\n", idx_w[j]);
				j++;
				j = j%5;
				V(sem_plein);
			}
			shmdt(idx_w);
			break;
		default: // pere - consommateur
			for (k = 1; k < 51; k++)
			{
				P(sem_plein);		
				printf("Consommee : %d\n", idx_r[l]);	
				l++;
				l = l%5;
				V(sem_vide);
			}
			wait(0); // on attend la fin du fils
			shmdt(idx_r); // detachement du segment memoire
			shmctl(shmid, IPC_RMID,0); // destruction du segment memoire
			printf("Segment memoire supprime.\n");
			detruire_semaphore();
	
	}
	return 0;
}

