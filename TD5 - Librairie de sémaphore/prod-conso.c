// Pour la compilation :
// gcc -o prod-conso prod-conso.c -L. -lsempv

#include "sem_pv.h"
#define PROT 0666

int nvide=5;  // nombre d'emplacements libres dans la memoire
int nplein=0; // nombre de messages en attente 

int sem_places_libres=0;
int sem_places_occupees=1;

int main(){

	int shmflag=PROT|IPC_CREAT; 
	// shmget cree un segment de memoire partagee et retourne son id
	int shmid;
	shmid = shmget(IPC_PRIVATE,512,shmflag); // creation du segment memoire de taille 5 entiers
	if(shmid == -1) perror("Creation echoue");

	int *ptr;
	ptr=shmat(shmid,0,shmflag); // attachement du segment

	pid_t fils;
	int i, j=0, k, l=0, v;

	init_semaphore();
	// initialisation
	val_sem(sem_places_libres,nvide); // nvide
	val_sem(sem_places_occupees,nplein); // nplein

	switch (fils=fork()){
		case -1:
			perror("Probleme fork");
			break;	
		case 0: // fils	- producteur
			for (i = 1; i < 21; i++)
			{
				nvide -= 1;
				// il n'y a pas de place disponible, on attend
				if(nvide == -1) {
					P(sem_places_libres);
				}
				// il y a de la place, on ecrit une lettre sur le buffer
				else  {
					ptr[j]=i;
					printf("Ajoutee : %d\n", ptr[j]);
					j++;
					j = j%5;
					nplein += 1;
					val_sem(sem_places_occupees,nplein);
				}
				// si le consommateur est en attente, on le reveil
				if(nplein == 0) {
					V(sem_places_occupees);
				}

			}
			shmdt(ptr);
			break;
		default: // pere - consommateur
			for (k = 1; k < 21; k++)
			{
				// il n'y a pas de message, on attend
				nplein -= 1;
				if(nplein == -1) {
					P(sem_places_occupees);
				}
				// il y a un message en attente, on le recupere
				else {
					v=ptr[l];
					printf("Consommee : %d\n", v);	
					l++;
					l = l%5;
					nvide += 1;
					val_sem(sem_places_libres,nvide);
				}
				// si le producteur est en attente, on le reveil
				if(nvide == 0) {
					V(sem_places_libres);
				}
			}
			wait(0); // on attend la fin du fils
			shmdt(ptr); // detachement du segment memoire
			shmctl(shmid, IPC_RMID,0); // destruction du segment memoire
			printf("Segment memoire supprime.\n");
			detruire_semaphore();
	}

	return 0;
}

