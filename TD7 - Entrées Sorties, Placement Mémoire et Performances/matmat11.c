/* matmat1.c	test of gus() get usage (of ressources)		*/
/* -------	mesure temps produit mat x vect 		*/
/* lire taille du test sur ligne de commande			*/
/* usage : tmatvec 500 <ret>					*/
/* compil: gcc -o matmat1 matmat1.c gus.o			*/

#include <sys/time.h>
#include <unistd.h>			/* getrusage() et sysconf() */
#include <sys/times.h>		/* times() */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// declaration des matrices, variables globales
#define maxsize 5000
double A[maxsize][maxsize], X[maxsize], Y[maxsize], D;
double B[maxsize][maxsize], C[maxsize][maxsize];


// initialise les matrices A,B,C,X et Y

void initaxy(int size){		

	int i,j;

	for (i=0 ; i<size ; i++) {
		X[i] = (double)i ; 
		Y[i] = 0.0;
		for (j=0 ; j<size ; j++) {
			A[i][j] = (double)(i+j);
			B[i][j] = (double)(i+j);
			C[i][j] = 0.0;   
		}
	}
}

// calcul de C=A*B

/*
Pour chaque ligne i de A :
 Pour chaque colonne j de B :
  C[i][j] = produit scalaire de ligne i de A par col.j de B
*/

void mulaxy(int size){ 
	int i,j,k;
	for (i=0 ; i<size ; i++)
	   for (j=0; j<size ; j++)
	      for (k=0; k<size ; k++)
	         { 
	           C[i][j]= C[i][j] + A[i][k]*B[k][j] ;
		   //printf("C[%d][%d]=%f %f\n",i,j,A[i][k],B[k][j]);
		   //printf("C[%d][%d]=%f\n",i,j,C[i][j]); 
		   }
}


// calcul la valeur de la diagonale

void sumdiac(int size){ 
	int i; 
	D=0.0;
	for (i=0 ; i<size ; i++) 
		D=D+C[i][i];
}


int main(int nargs, char *args[]) { 

	int i,j,N; 
	double delai, nops, mflops;
	/* 
	nombre d'operations flottantes par secondes = nombre d'operations flottantes/duree de calcul en secondes
	on exprime generalement ce nombre en Mflops : nombre de millions d'operations flottantes par secondes (penser a diviser par 10^6)
	*/
	clock_t tbegin , tend; 
	struct tms tmsbufbegin, tmsbufend;  
	double tickspsec;
 
	if (nargs<=1) { 
	N = 100;
	printf ("-- Pas d'arguments : valeur par defaut N=%d\n",N); 
	}
	else {
	sscanf(args[1], "%d", &N);
	printf("-- argument N=%d\n",N);
	}

/* ---------------- test --------------------- */
 
	initaxy(N); 
 
	tbegin=times(&tmsbufbegin); // times retourne le nombre de top d'horloge ecoules entre tbgein et tend, il faut un point de depart et un point d'arrivee *man time si besoin*
	mulaxy(N);
	sumdiac(N);
	printf("Valeur de D:%lf \n",D);
	tend=times(&tmsbufend);

	printf("-------------- mat1 -----------------\n");

	//printf("CLK_TCK = %d \n",CLK_TCK);

	tickspsec = sysconf(_SC_CLK_TCK); // sysconf(_SC_CLK_TCK) retourne le nombre de top d'horloge par seconde *man sysconf si besoin* 
	//The frequency of the statistics clock in ticks per second
	printf("Nombre de ticks par seconde = %f \n",tickspsec);

	printf("tbegin = %lu \n",tbegin);
	printf("tend = %lu \n",tend);

	delai=(tend-tbegin)/tickspsec; // on divise le nombre de top d'horloge obtenus pour le calcul par la frequence des tops par secondes
	// on obtient le temps de calcul

	nops = (double)(2*N*N*N) /delai; // on calcule le nombre d'operations flottantes par seconde (2*N*N*N correspond au nombre d'operations)

	mflops = nops/1000000.; // on divise par 10^6 pour obtenir le mflops

	printf("Resultat pour mulaxy1(%d)\n ",N);
	printf(" Delai= %5.2f \n  Nopsec= %f \n  Mflops= %5.2f\n",delai,nops,mflops);
	printf("-------------- matmat1 -----------------\n");
  
}
