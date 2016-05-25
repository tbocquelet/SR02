////////////////////////////////////////////////////////////////////////
// Camille Soetaert - Théo Bocquelet - Groupe du mercredi 16h30.18h30 //
////////////////////////////////////////////////////////////////////////

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
#include <string.h>

#define N_SEM 5 // nombre de sémaphores contenus dans l'ensemble créé par init_semaphore
int init_semaphore();
int detruire_semaphore();
int val_sem(int sem, int val);
int P(int sem);
int V(int sem);


