#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define MAX_AFF 99

int main()
{
	int fd;
	int* adr;
	//ouverture du fichier 
	if((fd=open("titi.dat",O_RDWR))==-1)
	{
		perror("ouverture_fichier erreur : ");
		exit(0);
	}
	adr=(int*)mmap(NULL,sizeof(int)*MAX_AFF,PROT_EXEC|PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	/*arguments de mmap : 
	1. ancienne adresse mémoire virtuelle du segment 
	2. taille à allouer au segment
	3. Protection du segment (usages) cf man mmap
	4. paramètre du mapping
	5. fichier partagée
	6. adresse de début*/
	if(adr==MAP_FAILED)
	{
		perror("creation_mmap erreur : ");
		exit(0);
	}

	//recherche MAX_AFF
	int j=0;
	int i=0;
	int length;
	char choix[2];
	while(i!=MAX_AFF)
	{
		printf("Entrer un nombre entre 0 et 98: \n");
		scanf("%s",choix);
        length=strlen(choix);
        if ((length>2)
        	||((length==1)&&((choix[0]<'0')||(choix[0]>'9')))
        	||((length==2)&&((choix[0]<'0')||(choix[0]>'9')||(choix[1]<'0')||(choix[1]>'9'))))
        	{//si le nombre ne fait pas partie de ceux demande
        	printf("erreur ceci ne correspond pas a ce qui est demande \n");
            i=-1;
        }
        else
        {
            i = atoi(choix); 
        }
		if((i!=MAX_AFF)&&(i!=-1))
			 //affichage du tableau
			for(j=0; j<MAX_AFF; j++)
			{
				printf("nb %d : %d \n",j,adr[j]);
			}
	}
	//destruction du mapping du segment et fermeture du fichier
	munmap(adr,MAX_AFF*sizeof(int));
	close(fd);
	return 0;
}