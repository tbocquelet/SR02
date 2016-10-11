/* rwscat.c	test of gus() get usage (of ressources)		*/
/* -------	mesure temps produit mat x vect 		*/
/* lire taille du test sur ligne de commande			*/
/* usage : tmatvec 500 <ret>					*/
/* compil: gcc -o rwscat rwscat.c gus.h			*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h> // readv et writev
#include <fcntl.h> /* def O_RDWR */
#include <sys/stat.h> /* def S_IRWXU */

#include "gus.h"
#define maxsize 5000

double A[maxsize][maxsize], X[maxsize], Y[maxsize], S,D;
double B[maxsize][maxsize], C[maxsize][maxsize];
long nop=0;	/* compter bestialement les opérations */
double scalxy(double *X, double *Y, int size);
double scalxy(double *X, double *Y, int size);

int des;
struct iovec vect[maxsize]; // pour readv et writev

/* ---------------------------------- init --------------------- */

// on initialise B comme la transposee de A

void initaxy(int size){			/* init pour A x B = C */
	int i,j;
	S = 0.0; 
	D=0.0;
	for (i=0 ; i<size ; i++) {
		X[i] = (double)i ; 
		Y[i] = 0.0;
		for (j=0 ; j<size ; j++){
			A[i][j] = (double)(1000*i+j);
			B[j][i] = (double)(1000*i+j); /* transposée de A, c'est notre temoin, on la comparera a la fin avec C */
			C[i][j] = 0.0;  // va devenir la transposee de A via les operations writev et readv
		}
	}
}

void imprim() {
	printf("A[0][0] [0][1] [0][2]= %f %f %f\n",A[0][0],A[0][1],A[0][2]);
	printf("A[0][0] [1][0] [2][0]= %f %f %f\n",A[0][0],A[1][0],A[2][0]);
	printf("B[0][0] [0][1] [0][2]= %f %f %f\n",B[0][0],B[0][1],B[0][2]);
	printf("B[0][0] [1][0] [2][0]= %f %f %f\n",B[0][0],B[1][0],B[2][0]);
	printf("C[0][0] [0][1] [0][2]= %f %f %f\n",C[0][0],C[0][1],C[0][2]);
	printf("C[0][0] [1][0] [2][0]= %f %f %f\n",C[0][0],C[1][0],C[2][0]);
}

void impadr() {
	printf("&A[0][0] &A[1][0] &A[2][0]=%lf %lf %lf\n",A[0][0],A[1][0],A[2][0]); // & enleves des A[][] buguaient !
	printf("ecart &A[1]-&A[0] &A[2]-&A[1]=%ld %ld\n", &A[1][0]-&A[0][0],&A[2][0]-&A[1][0]);
}

/* ---------------------------------- write --------------------- */

/* écrire la matrice par colonne et la relire par ligne => transpose */
/* écrire A par colonne, la relire par ligne dans C et comparer B et C */

void wrscat(int size) {
	int k,i, nw=0;

	// ouverture du fichier mat.dat
	if ( (des = open("mat.dat",O_CREAT|O_RDWR|O_TRUNC,S_IRWXU)) ==-1) { 
		perror("open"); 
		exit(2);
	}

	for (k=0 ; k<size ; k++) { 
	/* pour chaque colonne k de A initialiser vect en fonction du nombre de ligne N effectif */
		for (i=0 ; i<size ; i++) {
			vect[i].iov_base = &A[i][k]; /* adresse de l'element i de la colonne k */
			vect[i].iov_len  = 1*sizeof(double); /* taille de l'element a transferer */
		}
// Pour chaque colonne de A : écrire la colonne de A dans le fichier mat.dat en utilisant une opération writev(fd, vect, size)
		nw = writev(des, vect, size); // écrire la colonne courante désignée par vect, on rentre les informations par bloc dans chaque tampon
		/* printf("col %d écrit %d\n",k,nw); */
	}

	close(des);
}


/* ---------------------------------- read --------------------- */

/* relire par ligne dans C */

void rdscat(int size) { 
	int k,i, nw=0;

	// ouverture de mat.dat
	if ( (des = open("mat.dat",O_RDONLY)) ==-1) { 
		perror("open"); 
		exit(2);
	}

	/* initialiser vect en fonction du nombre de ligne N effectif */
	for (i=0 ; i<size ; i++) { /* pour chaque ligne i de C */
		vect[i].iov_base = &C[i][0]; /* adresse de l'élément 0 ligne i  */
		vect[i].iov_len  = size*sizeof(double); /* size élém à chaque fois */
	}
	/* dans ce cas nous n'avons besoin que d'une seule lecture */
// Pour chaque ligne de C : en utilisant une opération readv(fd, vect, size) remplir la ligne de C avec les "size" valeurs suivantes lues dans le fichier
	nw = readv(des, vect, size);
	printf("lu %d\n",nw);

	close(des);
}

/* ---------------------------------- vérif --------------------- */

/*
La fonction diffbc(size) va effectuer une vérification en calculant la somme de toutes les différences B[i][j] - C[i][j]. 
Si les opérations ont été bien faites, cette somme est nulle puisque B et C sont identiques. On utilisera S pour calculer cette somme.
*/

void diffbc(int size){	/* vérifier diff entre B et C */
	int i,j;
	D=0.0;
	for (i=0 ; i<size ; i++) {
	    for (j=0 ; j<size ; j++) {
			D = D + B[i][j] - C[i][j];  
		}
	}
}

/* ---------------------------------- main --------------------- */

int main(int nargs, char *args[]) { 
	int i,j,N; 
	double nops,mflops;
	long ela_s, ela_c, prc, ut_t, ut_se, ut_us, st_t, st_se, st_us;
	long maxrss, ixrss, idrss, isrss, minflt, majflt, nswap;
	long inblock, oublock, msgsnd, msgrcv, nsignals, nvcsw, nivcsw;

	if (nargs<=1) { 
		N = 100;
		printf ("-- pas d'args : val. par def N=%d\n",N);
	}
	else {
		sscanf(args[1], "%d", &N);
		printf("-- arg N=%d\n",N);
	}

	/* ---------------------------------- test 1 --------------------- */
	printf("-----------test 1 rwscat  A.X=Y\n");
	nop=0;
	initaxy(N); 
	imprim();

	gus_ela__ (&ela_s, &ela_c, &prc, &ut_se, &ut_us, &st_se, &st_us);
	wrscat(N);
	rdscat(N);
	gus_ela__ (&ela_s, &ela_c, &prc, &ut_se, &ut_us, &st_se, &st_us);
	printf("elapsed=%ld.%0.2ld s. %ld%%cpu %ld.%0.2ld user %ld.%0.2ld syst\n", ela_s, ela_c, prc, ut_se, ut_us, st_se, st_us);
	imprim();
	impadr();
	diffbc(N);
	printf("apres mulaxy N=%d D=%f\n",N,D);
}
