#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_AFF 99

int main ()
{
	// tableau à mettre dans le fichier
	int tab[MAX_AFF];
	int i=0;
	int fd;
	for(i=0;i<MAX_AFF;i++)tab[i]=0;
	//ouverture du fichier avec possibilité lecture, ecriture, création et réécriture
	fd=open("titi.dat",O_WRONLY|O_CREAT|O_TRUNC,0666);
	// inscription du tableau dans le fichier
	write(fd,tab,MAX_AFF*sizeof(int));
	//fermeture du fichier
	close(fd);
	return 0;
}