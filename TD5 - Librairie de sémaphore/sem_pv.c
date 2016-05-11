#include "semaph.h"

int init_semaphore(){

	
	semget(IPC_PRIVATE,N_SEM,IPC_CREAT|IPC_EXCL);
}