#include "sem_pv.h"

// Pour la compilation :
// gcc -o sem1 sem1.c -L. -lsempv

int main(){

	int retour;
     
	retour = init_semaphore();
    printf("Valeur de retour pour init_semaphore() : %d \n", retour );
    
    retour = val_sem(2,1);
	printf("Valeur de retour pour val_sem(sem,val) : %d \n", retour );
    
    retour = P(2);
	printf("Valeur de retour pour P(sem) : %d \n", retour );
	
    sleep(10);
    
    retour = V(2);
	printf("Valeur de retour pour V(sem) : %d  \n", retour );
    
    retour = detruire_semaphore();
	printf("Valeur de retour pour detruire_semaphore() : %d  \n", retour );
    
	return 0;
}