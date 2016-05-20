#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h> 
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>

#define N_SEM 5
int init_semaphore();
int detruire_semaphore();
int val_sem(int sem, int val);
int P(int sem);
int V(int sem);


