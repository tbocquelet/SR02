////////////////////////////////////////////////////////////////////////
// Camille Soetaert - Théo Bocquelet - Groupe du mercredi 16h30.18h30 //
////////////////////////////////////////////////////////////////////////

// Pour la compilation :
// gcc -o sem1 sem1.c -L. -lsempv

#include "sem_pv.h"

int main(){

	int retour; // Variable stockant la valeur de retour des différentes fonctions
   	
	retour = init_semaphore();
    printf("Valeur de retour pour init_semaphore : %d\n\n", retour );
    
    retour = val_sem(2,1);
	printf("Valeur de retour pour val_sem : %d\n\n", retour );
    
    retour = P(2);
	printf("Valeur de retour pour P : %d\n\n", retour );
	
    sleep(30); // Attente de 30 secondes
    
    retour = V(2);
	printf("Valeur de retour pour V : %d\n\n", retour );
    
    retour = detruire_semaphore();
	printf("Valeur de retour pour detruire_semaphore : %d\n\n", retour );
    
	return 0;
}
