#include "sem_pv.h"

int semid;

int init_semaphore(){
// si le groupe n'existe pas on le cree et on l'initialise
// s'il existe, on l'initialise 
	int shmflag=IPC_CREAT|IPC_EXCL; 
	semid = semget(IPC_PRIVATE,N_SEM,shmflag); // utiliser errno.h pour le cas ou le semaphore existe deja
	if(semid==-1) {
		perror("Creation echoue");
		return -2;
	}
// gerer le retour -1
	union semun argument;
	for (int i = 0; i < N_SEM; i++) { argument.array[i]=0; }
	semctl(semid, N_SEM, SETALL, argument);
	return 0; // tout va bien, semaphore cree
}

int detruire_semaphore() {
	semctl(semid,0,IPC_RMID);
	return 0;
}

int val_sem(int sem, int val){
//Attribue la valeur "val" au sémaphore "sem" du groupe de sémaphores créé par "init_semaphore".
//Retourne la valeur de retour de "semctl" en cas de réussite, -1 si "init_semaphore" n'a pas été appelé avant, -2 si le numéro de sémaphore est incorrect.
	
// gerer le -1
	union semun argument; // existe deja pas besoin de la redefinir 
	argument.val=val;
	int retour; // valeur retour
	retour= semctl(semid, sem, SETVAL, argument);
	if(retour == -1)
	{
	    perror("Initialisation semaphore echoue\n");
	    retour=-2;
	}
	return retour;
}

int P(int sem){ // wait
//Réalise l'opération (P) sur le sémaphore numéro "sem" du groupe de sémaphores créé par "init_semaphore".
//Retourne la valeur de retour de "semop" en cas de réussite, -1 si "init_semaphore" n'a pas été appelé avant, -2 si le numéro de sémaphore est incorrect.

	struct sembuf *tab_op;
	int retour;
	tab_op->sem_num=sem;

	int val_sem;
	val_sem=semctl(semid, sem, GETVAL);

	if (val_sem>0) {
		tab_op->sem_op=-1; // applique l'operation P au semaphore
		retour=semop(semid, tab_op,1);
	}
	else {
		tab_op->sem_op=0; // bloque le semaphore
		retour=semop(semid, tab_op,1);
	}

	return 0;
}

int V(int sem){ // signal
	
	struct sembuf *tab_op;
	int retour;
	tab_op->sem_num=sem;

	int nb_bloque;
	nb_bloque=semctl(semid, sem, GETNCNT);
	if (nb_bloque>0)
	{	
		tab_op->sem_op=1;
		retour=semop(semid, tab_op,1);
	}
	else {
		tab_op->sem_op=-1;
		retour=semop(semid, tab_op,1);
	}

	
	return 0;
}

int main(){
	return 0;
}

