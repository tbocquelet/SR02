/* matmat1.c	test of gus() get usage (of ressources)		*/
/* -------	mesure temps produit mat x vect 		*/
/* lire taille du test sur ligne de commande			*/
/* usage : tmatvec 500 <ret>					*/
/* compil: gcc -o matmat1 matmat1.c gus.o			*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	/* getrusage() et sysconf() */
#include <sys/times.h>		/* times() */
#include <limits.h>

#define maxsize 5000
double A[maxsize][maxsize], X[maxsize], Y[maxsize], D;
double B[maxsize][maxsize], C[maxsize][maxsize];


// INITIALISATION

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

// calcul du produit sclaire entre X et Y

double scalxy(double *X, double *Y, int size){	/* calcul S=S+X*Y */ 
	int i; 
	double s=0.0;
	for (i=0 ; i<size ; i++) {	
		s=s+X[i]*Y[i];
	}
	return s;
}

/*
MULTIPLICATION DES MATRICES C=A*B

Pour chaque colonne i de B :
      copier cette colonne dans le vecteur X
      Pour chaque ligne j de A :
           C[j][i]= produit scalaire de X par la ligne j de A

On lit ligne par ligne dans cette methode et non plus ligne par colonne.
*/

void mulaxy(int size){ 
	int i,j,k, ns=0;
	for (i=0 ; i<size ; i++){  /* pour chaque colonne i de B, copier dans X cette colonne */
		 for (j=0; j<size ; j++){ 
		 	X[j] = B[j][i];
		 }
		 /* puis, pour chaque ligne j de A, C[j][i]= produit scalaire (X*ligne[A]) */
		 for (j=0; j<size ; j++){  
		 	ns++;
			C[j][i]= scalxy( &X[0], &A[i][0], size); 
		}  
	}
	printf("ns=%d\n",ns);
}



/* -----------sumdiac: vérifier que calcul fait------------ */
void sumdiac(int size){
	int i,j; 
	D=0.0;
	for (i=0 ; i<size ; i++) 
		D=D+C[i][i];
}


int main(int nargs, char *args[]) { 
	int i,j,N; 
	double delai, nops, mflops;
	clock_t tbegin , tend;
	struct tms tmsbufbegin, tmsbufend;  
	double tickspsec;

	if (nargs<=1) {
		N = 100;
		printf ("-- Pas d'arguments retenus : valeur par defaut N=%d\n",N); 
	}
	else {
	sscanf(args[1], "%d", &N);
	printf("-- argument N=%d\n",N);
	}

/* ---------------- test --------------------- */
 
	initaxy(N); 

	tbegin=times(&tmsbufbegin);
	mulaxy(N);
	sumdiac(N);
	printf("Valeur de D:%lf \n",D);
	tend=times(&tmsbufend);

	printf("-------------- mat2 -----------------\n");
	
	tickspsec = sysconf(_SC_CLK_TCK);
	printf("Nombre de ticks par seconde = %f \n",tickspsec);

	printf("tbegin = %lu \n",tbegin);
	printf("tend = %lu \n",tend);

	delai=(tend-tbegin)/tickspsec;

	nops = (double)(2*N*N*N) /delai;

	mflops = nops/1000000.;

	printf("Resultat pour mulaxy2(%d)\n ",N);
	printf(" Delai= %5.2f \n  Nopsec= %f \n  Mflops= %5.2f\n",delai,nops,mflops);
	printf("-------------- mat2 -----------------\n");
	
}
