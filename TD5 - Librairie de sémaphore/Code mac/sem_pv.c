////////////////////////////////////////////////////////////////////////
// Camille Soetaert - Théo Bocquelet - Groupe du mercredi 16h30.18h30 //
////////////////////////////////////////////////////////////////////////

#include "sem_pv.h"

// Compilation :
// Pour le .o : gcc -c sem_pv.c 
// Pour la libraire : ar rvs libsempv.a sem_pv.o

// Affichage des sémaphores dans le terminal : ipcs -s
// Suppression manuelle d'un sémaphore ipcrm -s IDsem

int semid; // ID de l'ensemble de sémaphores 

// Fonction pour créer et initialiser un groupe de N_SEM sémaphores
int init_semaphore(){

	errno = 0; // "errno" va contenir le code d'erreur retourné par semget(), c'est pourquoi on l'initialise ==> le même principe est utilisé pour semop() et semctl()
	
// Il faut ajouter SEM_A et SEM_R aux flags pour obtenir respectivement :
// 		le droit de modifier le sémaphore
// 		le droit de lire le sémaphore

	//int shmflag = IPC_CREAT|IPC_EXCL|SEM_A|SEM_R; 
	int shmflag = IPC_CREAT|IPC_EXCL|0666;
	semid = semget(IPC_PRIVATE,N_SEM,shmflag);  // Création de l'ensemble de sémaphores
	
	// On teste si il y a eu des erreurs lors de la création
	if(semid == -1) {
		if (errno == 17){ // errno = EEXIST = 17, erreur retournée lorsque le sémaphore existe déjà
			fprintf(stderr,"%s", "ERREUR : le sémaphore existe déjà.\n");
			return -1;
		}	
		else { 
			fprintf(stderr,"%s", "ERREUR : création échouée.\n");
			return -2;
		}
	}

	union semun argument;
	int i;
	for (i = 0; i < N_SEM; i++) { 
		argument.array[i] = 0; 
	} 

	semctl(semid, N_SEM, SETALL, argument); // On initialise l'ensemble des sémaphores à 0 
	
	printf("Sémaphore créé !\n");
	return 0; 
}

// Fonction pour détruire le groupe de sémaphores
int detruire_semaphore() {

	errno = 0;

	int retour; // Variable pour stocker la valeur de retour de semctl(), le même principe est utilisé dans les autres fonctions
	retour = semctl(semid,0,IPC_RMID); // Destruction du groupe de sémaphore

	// On teste si il n'y a pas eu d'erreur
	if (errno == 22) { // errno = EINVAL = 22 : semid a une valeur invalide 
		fprintf(stderr,"%s", "ERREUR : merci d'appeller init_semaphore en premier.\n");
		return -1;
	}
	// Autres erreurs 
	if (retour == -1) {
		perror("ERREUR");
		return -1;
	}

	printf("Sémaphore detruit !\n");
	return retour;
}

// Fonction pour attribuer la valeur "val" au sémaphore numéro "sem"
int val_sem(int sem, int val){
	
	errno = 0;

	union semun argument;
	argument.val=val;
	
	int retour; 
	retour=semctl(semid, sem, SETVAL, argument); // Affectation de la valeur "val" au "sem"-ième sémaphore du groupe 

	// On teste s'il n'y a pas eu d'erreurs
	if(retour == -1) {
		if (errno==22) { // errno = EINVAL = 22 : semid a une valeur invalide 
			fprintf(stderr,"%s", "ERREUR : merci d'appeller init_semaphore en premier.\n");
			return -1;
		}
		if (errno == 27) {  // errno = EFBIG = 27 : semnum n'est pas compris entre 0 et (N_SEM-1) 
			fprintf(stderr,"%s", "ERREUR : merci d'indiquer un semnum correct.\n");
			return -2;
		}
		// Autres erreurs
		perror("AUTRE ERREUR");
		return -3;
	}
	
	return retour;	
}

// Fonction permettant d'appliquer l'opération P au sémaphore numéro "sem" du groupe
int P(int sem){ 

	struct sembuf tab_op[1];
	tab_op[0].sem_num = sem;
	tab_op[0].sem_op = -1; // -1 correspond à l'opération P
	tab_op[0].sem_flg = 0;

	int retour;

	retour = semop(semid,tab_op,1); // Application de l'opération P

	// On teste s'il n'y a pas eu d'erreurs
	if(retour == -1) {
		if (errno == 22) { // errno = EINVAL = 22 : semid a une valeur invalide 
			fprintf(stderr,"%s", "ERREUR : merci d'appeller init_semaphore en premier.\n");
			return -1;
		}
		if (errno == 27) {  // errno = EFBIG = 27 : semnum n'est pas compris entre 0 et (N_SEM-1)
			fprintf(stderr,"%s", "ERREUR : merci d'indiquer un semnum correct.\n");
			return -2;
		}
		// Autres erreurs
		perror("AUTRE ERREUR");
		return -3;
	}

	return retour;
}

// Fonction permettant d'appliquer l'opération V au sémaphore numéro "sem" du groupe
int V(int sem){ 
	
	struct sembuf tab_op[1];
	tab_op[0].sem_num = sem;
	tab_op[0].sem_op = 1; // 1 correspond à l'operation V 
	tab_op[0].sem_flg = 0;

	int retour;

	retour = semop(semid,tab_op,1); // Application de l'opération V

	// On teste s'il n'y a pas eu d'erreurs
	if(retour == -1) {
		if (errno==22) { // errno = EINVAL = 22 : semid a une valeur invalide 
			fprintf(stderr,"%s", "ERREUR : merci d'appeller init_semaphore en premier.\n");
			return -1;
		}
		if (errno == 27) {  // errno = EFBIG = 27 : semnum n'est pas compris entre 0 et (N_SEM-1) alors il y a erreur
			fprintf(stderr,"%s", "ERREUR : merci d'indiquer un semnum correct.\n");
			return -2;
		}
		// Autres erreurs
		perror("AUTRE ERREUR");
		return -3;
	}

	return retour;
}

