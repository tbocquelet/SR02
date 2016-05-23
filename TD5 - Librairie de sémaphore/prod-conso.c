// Pour la compilation :
// gcc -o prod-conso prod-conso.c -L. -lsempv

#include "sem_pv.h"
#define PROT 0666

int nvide=4;
int nplein=0;

int main(){

	int shmflag=PROT|IPC_CREAT; 
	// shmget cree un segment de memoire partagee et retourne son id
	int shmid;
	shmid = shmget(IPC_PRIVATE,5*sizeof(int),shmflag); // creation du segment memoire de taille 5 entiers
	if(shmid == -1) perror("Creation echoue");

	int *ptr;
	ptr=shmat(shmid,0,shmflag); // attachement du segment

	pid_t fils;
	int i, j, k, l;

	init_semaphore();
	val_sem(0,nvide); // nvide
	val_sem(1,nplein); // nplein

	switch (fils=fork()){
		case -1:
			perror("Probleme fork");
			break;	
		case 0: // fils	- producteur
			for (int i = 1; i < 51; i++)
			{
				nvide -= 1;
				val_sem(0,nvide);
				if(nvide == -1) {
					P(0);
				}
				else {
					ptr[j]=i;
					j++;
					j = j%5;
					nplein += 1;
					val_sem(1,nplein);
					V(1);
				}
				if(nplein == 0) {
					V(1);
				}

			}
			shmdt(ptr);
			break;
		default: // pere - consommateur
			for (int k = 1; k < 51; k++)
			{
				nplein -= 1;
				val_sem(1,nplein);
				if(nplein == -1) {
					P(1);
				}
				else {
					k=ptr[l];
					printf("Buffer[%d] = %d\n",l,k);
					l++;
					l = l%5;
					nvide += 1;
					val_sem(0,nvide);
					V(0);
				}
				if(nvide == 0) {
					V(0);
				}
			}
			wait(0); // on attend la fin du fils
			shmdt(ptr); // detachement du segment memoire
			shmctl(shmid, IPC_RMID,0); // destruction du segment memoire
			printf("Segment memoire supprime.\n");
	}

	detruire_semaphore();
	return 0;
}

