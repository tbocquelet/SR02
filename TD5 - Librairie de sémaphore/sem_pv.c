#include "semaph.h"

int  semid;

int init_semaphore(){
	int shmflag=IPC_CREAT|IPC_EXCL; 
	int n_sem; // a initialiser
	semid = semget(IPC_PRIVATE,n_sem,shmflag); // utiliser errno.h pour le cas ou le semaphore existe deja
	if(semid==-1) {
		perror("Creation echoue");
		return -2;
	}
	
	return 0; // tout va bien, semaphore cree
}

int detruire_semaphore() {
	semctl(semid,0,IPC_RMID);
	return 0;
}

int main(){
	return 0;
}