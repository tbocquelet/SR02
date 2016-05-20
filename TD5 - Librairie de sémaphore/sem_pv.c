#include "sem_pv.h"

// Pour le .o : gcc -c sem_pv.c --> sem_pv.o
// Pour la libraire : ar rvs libsempv.a sem_pv.o


int semid=-2;

int init_semaphore(){

	errno = 0; // pour la gestion des erreurs

	int shmflag = IPC_CREAT|IPC_EXCL; 
	semid = semget(IPC_PRIVATE,N_SEM,shmflag); 
	// on teste si il y a eu des erreurs
	if(semid == -1) {
		if (errno == 17){ // EEXIST = 17, erreur retournee lorsque le semaphore existe deja
			fprintf(stderr, "ERREUR : le semaphore existe deja.\n");
			return -1;
		}	
		else {
			fprintf(stderr, "ERREUR : creation echoue.\n");
			return -2;
		}
	}

	union semun argument;
	for (int i = 0; i < N_SEM; i++) { argument.array[i] = 0; } // on initialise l'ensemble des semaphores a 0
	semctl(semid, N_SEM, SETALL, argument);
	
	printf("Semaphore cree !\n");
	return 0; // tout va bien, semaphore cree
}

int detruire_semaphore() {

	errno = 0;

	int retour; // variable pour stocker la valeur de retour de semctl
	retour = semctl(semid,0,IPC_RMID);

	// on teste si il n'y a pas eu d'erreur
	if (errno == 22) { // EINVAL = 22 : semid a une valeur invalide 
		fprintf(stderr, "ERREUR : merci d'appeller init_semaphore en premier.\n");
		return -1;
	}

	printf("Semaphore detruit !\n");
	return retour;
}

int val_sem(int sem, int val){
	
	errno = 0;

	union semun argument;
	argument.val=val;

	int retour; // variable pour stocker la valeur de retour de semctl
	retour=semctl(semid, sem, SETVAL, argument);

	// on teste s'il n'y a pas eu d'erreurs
	if(retour == -1) {
		if (errno==22) { // EINVAL = 22 : semid a une valeur invalide 
			fprintf(stderr, "ERREUR : merci d'appeller init_semaphore en premier.\n");
			return -1;
		}
		if (sem<0 || sem>(N_SEM-1)) {  // si semnum n'est pas compris entre 0 et (N_SEM-1) alors il y a erreur
			fprintf(stderr, "ERREUR : merci d'indiquer un semnum correct.\n");
			return -2;
		}
	}

	printf("Valeur du sempahore modifee !\n");
	return retour;
}

int P(int sem){ 

	struct sembuf tab_op[1];
	tab_op[0].sem_num = sem;
	tab_op[0].sem_op = -1; // applique l'operation P au semaphore
	tab_op[0].sem_flg = 0;

	int retour;

	retour = semop(semid,tab_op,1);

	// on teste s'il n'y a pas eu d'erreurs
	if(retour == -1) {
		if (errno == 22) { // EINVAL = 22 : semid a une valeur invalide 
			fprintf(stderr, "ERREUR : merci d'appeller init_semaphore en premier.\n");
			return -1;
		}
		if (errno == 27) {  // EFBIG = 27 : si semnum n'est pas compris entre 0 et (N_SEM-1) alors il y a erreur
			fprintf(stderr, "ERREUR : merci d'indiquer un semnum correct.\n");
			return -2;
		}
	}

	printf("Operation P appliquee ! \n");
	return retour;
}

int V(int sem){ 
	
	struct sembuf tab_op[1];
	tab_op[0].sem_num = sem;
	tab_op[0].sem_op = 1; // applique l'operation V au semaphore
	tab_op[0].sem_flg = 0;

	int retour;

	retour = semop(semid,tab_op,1);

	// on teste s'il n'y a pas eu d'erreurs
	if(retour == -1) {
		if (errno==22) { // EINVAL = 22 : semid a une valeur invalide 
			fprintf(stderr, "ERREUR : merci d'appeller init_semaphore en premier.\n");
			return -1;
		}
		if (errno == 27) {  // EFBIG = 27 : si semnum n'est pas compris entre 0 et (N_SEM-1) alors il y a erreur
			fprintf(stderr, "ERREUR : merci d'indiquer un semnum correct.\n");
			return -2;
		}
	}

	printf("Operation V appliquee ! \n");
	return retour;
}

