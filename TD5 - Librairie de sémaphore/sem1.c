// Pour la compilation :
// gcc -o sem1 sem1.c -L. -lsempv

#include "sem_pv.h"

int main(){

	int retour;
   	int i=1;
	retour = init_semaphore();
    printf("%d. Valeur de retour pour init_semaphore : %d\n\n", i++, retour );
    
    retour = val_sem(2,1);
	printf("%d. Valeur de retour pour val_sem : %d\n\n", i++, retour );
    
    retour = P(2);
	printf("%d. Valeur de retour pour P : %d\n\n", i++, retour );
	
    sleep(10);
    
    retour = V(2);
	printf("%d. Valeur de retour pour V : %d\n\n", i++, retour );
    
    retour = detruire_semaphore();
	printf("%d. Valeur de retour pour detruire_semaphore : %d\n\n", i++, retour );
    
	return 0;
}
