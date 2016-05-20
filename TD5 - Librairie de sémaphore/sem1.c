// Pour la compilation :
// gcc -o sem1 sem1.c -L. -lsempv

int main(){

    int i = 0;
    int retour;
    printf("ici");
    retour = init_semaphore();
    printf("%d  ", retour );
    printf("%d\n",i++);
    retour = val_sem(2,1);
	printf("%d  ", retour );
    printf("%d\n",i++);
    retour = P(2);
	printf("%d  ", retour );
	printf("%d\n",i++);
    sleep(10);
    printf("%d\n",i++);
    retour = V(2);
	printf("%d  ", retour );
    printf("%d\n",i++);
    retour = detruire_semaphore();
	printf("%d  ", retour );
    printf("%d\n",i++);

	return 0;
}