#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_AFF 99

int main ()
{
	int tab[MAX_AFF];
	int i,fd;
	//ouverture
	fd=open("titi.dat",O_RDONLY,0666);
	//lecture
	read(fd,tab,MAX_AFF*sizeof(int));
	//fermeture
	close(fd);
	//affichage
	for(i=0;i<MAX_AFF;i++) printf("nb %d : %d\n",i,tab[i]);
	return 0;
}