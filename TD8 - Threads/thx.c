/* thx.c exemple threads+X11 *
appelle routines definies dans thw.c et barx.c *
> gcc -c barx.c *
> gcc -c thw.c *
> gcc -o thx thx.c thw.o barx.o -L/usr/X11R6/lib/ -lX11 -lpthread */
/* main lance des threads qui chacun: *
fait progresser un rectangle horizontal, */
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#define nth 3

pthread_t threads[nth];
pthread_mutex_t mutex;
pthread_cond_t event;
/*is = pthread_mutex_init(&mutex, NULL);
is = pthread_cond_init (&event, NULL);
is = pthread_mutex_lock(&mutex);
is = pthread_mutex_unlock(&mutex);
is = pthread_cond_signal(&event);
is = pthread_cond_wait(&event,&mutex);
is = pthread_create(&threads[i], NULL, th_fonc, (void *)i);
is = pthread_join( threads[j], &val);*/
int cpt_global=0

void *th_fonc (void *arg) {
	int m1,m2,m3,numero,i,k;
	m1=20;
	m2=21;
	m3=22;
	numero = (int)arg;
	
	if (numero==0){
	 drawstr (30, 125, "_0_", 3);
	 drawrec(100,100,100+10*m1,30);
	 for(i=0;i<m1;i++){
	  pthread_mutex_lock(&mutex);
	  printf("num %d i=%d\n",numero,i);
	  pthread_cond_wait(&event,&mutex);
	  k=cpt_global;
	  th_wait();
	  fillrec (100,102,100+j*10,26,"yellow");
	  k++;
	  cpt_global=k;
	  pthread_mutex_unlock(&mutex);
	  
	 }
	}
	if (numero==1){
	 drawstr (30, 225, "_1_", 3);
	 drawrec(100,200,100+10*m2,30);
	}
	if (numero==2){
	 drawstr (30, 325, "_2_", 3);
	 drawrec(100,300,100+10*m3,30);
	}

}

int main(){
	int is, i;
	void *val=0;
	is = pthread_mutex_init(&mutex, NULL);
/* créer les threads */
	
	for(i=0; i<nth; i++) {
		 printf("ici main, création thread %d\n",i);
		 is = pthread_create( &threads[i], NULL, th_fonc, (void *)i );
		 ifer (is,"err. création thread");
	}
/* attendre fin des threads */
	for(i=0; i<nth; i++) {
		 is = pthread_join( threads[i], &val);
		 ifer (is,"err. join thread");
		 printf("ici main, fin thread %d\n",(int)val);
	}
	
}
