/* gus.c   interface d'appel de times() et getrusage()  */
/* (c)Michel_Vayssade -- Juin 2000                      */
/* version 2 : interface simplifiee passant des doubles */
/* void gus_elacpu__(double* ela, double* cpu)          */
/* avec ces cons de underscores du fortran              */

#include <sys/time.h>		/* getrusage() */
#include <sys/resource.h>	/* getrusage() */
#include <unistd.h>	/* getrusage() et sysconf() */
#include <sys/times.h>		/* times() */

static struct tms tmsbuf;  /* current value */
static struct tms tmslast; /* keep it for next call */
static struct rusage last; /* store last values, get current, */
static struct rusage curr; /* then rus=curr-last; last=curr; */
static struct rusage rus;  /* keep it for gus_mem and gus_io */

/*--------------------------------------------------- gus_ela --*/
void gus_ela__(long* ela_s, long* ela_c, long* prc, long* ut_se, long* ut_us, long* st_se, long* st_us){
	clock_t elapsed, delta;
	static clock_t last; /* remenber last call ticks num; */
	static int flag=0;
	long deluser,delsys; /* delta user et sys depuis last */
	static long tps = -1, scale; /* init them and keep them */

	if (flag==0) { 
		flag=1;
		last=times(&tmsbuf);
		tmslast.tms_utime = tmsbuf.tms_utime;
		tmslast.tms_stime = tmsbuf.tms_stime;

		tps = sysconf(_SC_CLK_TCK);
		scale = 1000000 / tps;
		#ifdef DEBUG
		printf("gus_ela -- tps _SC_CLK_TCK=%d scale=%d\n",tps,scale);
		#endif
	}
	/* get current delta, cpu user et cpu sys en clock ticks */
	elapsed = times(&tmsbuf); 

	/* delta = current - last */
	delta = elapsed-last;
	deluser = tmsbuf.tms_utime - tmslast.tms_utime;
	delsys  = tmsbuf.tms_stime - tmslast.tms_stime;

	if ( delta==0) *prc=0;
	else *prc = 100 * deluser / delta;

	#ifdef DEBUG
	printf("gus_ela -- elapsed=%d last=%d ela= %d tmsbuf.tms_utime=%d deluser=%d  %%CPU got=%d%%\n", elapsed, last, delta, tmsbuf.tms_utime, deluser, *prc);
	#endif

	/* convertir clock ticks en sec et centièmes */

	*ela_s = delta  / tps;	 /* ela en sec */
	*ela_c = delta  % tps;  /* ela en centièmes de sec */

	*ut_se = deluser  / tps;	/* sec */
	*ut_us = deluser  % tps; 	/* cent. de sec */

	*st_se = delsys  / tps;
	*st_us = delsys  % tps;

	#ifdef DEBUG
	printf("gus_ela -- elapsed=%d.%0.2d s. %d%%cpu %d.%0.2d user %d.%0.2d syst\n",*ela_s, *ela_c, *prc, *ut_se, *ut_us/10000, *st_se, *st_us/10000);
	#endif

	/* last = current */
	last=elapsed;
	tmslast.tms_utime = tmsbuf.tms_utime;
	tmslast.tms_stime = tmsbuf.tms_stime;

}
 
/*------------------------------------------------ gus_elacpu --*/
void gus_elacpu__(double* ela, double* cpu){
	static int flag=0;
	long ela_s, ela_c, prc, ut_t, ut_se, ut_us, st_t, st_se, st_us;

	gus_ela__ (&ela_s, &ela_c, &prc, &ut_se, &ut_us, &st_se, &st_us);

	*ela = (double)( ela_s*100 + ela_c ) / 100.;
	*cpu = (double)( (ut_se+st_se)*100 +ut_us+st_us ) / 100.;

/* printf("--gus_elacpu ela= %f cpu= %f\n",*ela,*cpu); */
}

/*--------------------------------------------------- gus_cpu --*/
void gus_cpu (long* ut_se, long* ut_us, long* st_se, long* st_us){

	static int flag=0;
	/* warn: ut_us et st_us sont renvoyés en unités de microsecondes */
	/* à transformer en centièmes de sec. (ss.cc) en div par 10000 */

	/* get current values */
	getrusage(RUSAGE_SELF,&curr);
	/* rus = curr - last (last=0 if first call) */
	if (flag==0) { 
		flag=1;
		last.ru_utime.tv_sec  = 0;
		last.ru_utime.tv_usec = 0;
		last.ru_stime.tv_sec  = 0;
		last.ru_stime.tv_usec = 0;
		last.ru_maxrss = 0;
		last.ru_ixrss  = 0;
		last.ru_idrss  = 0;
		last.ru_isrss  = 0;
		last.ru_minflt = 0;
		last.ru_majflt = 0;
		last.ru_nswap  = 0;
		last.ru_inblock = 0;
		last.ru_oublock = 0;
		last.ru_msgsnd  = 0;
		last.ru_msgrcv  = 0;
		last.ru_nsignals= 0;
		last.ru_nvcsw   = 0;
		last.ru_nivcsw  = 0;
	}

	#ifdef DEBUG
	 printf("gus_cpu -- last.ru_utime.tv_sec=%d last.ru_utime.tv_usec=%d curr.ru_utime.tv_sec=%d curr.ru_utime.tv_usec=%d\n",last.ru_utime.tv_sec, last.ru_utime.tv_usec,curr.ru_utime.tv_sec, curr.ru_utime.tv_usec);
	#endif

	if(curr.ru_utime.tv_usec < last.ru_utime.tv_usec) {
		curr.ru_utime.tv_sec--; 
		curr.ru_utime.tv_usec= curr.ru_utime.tv_usec + 1000000; 
	}
	if(curr.ru_stime.tv_usec < last.ru_stime.tv_usec) {
		curr.ru_stime.tv_sec--; 
		curr.ru_stime.tv_usec= curr.ru_stime.tv_usec + 1000000; 
	}
	rus.ru_utime.tv_sec  = curr.ru_utime.tv_sec  - last.ru_utime.tv_sec;
	rus.ru_utime.tv_usec = curr.ru_utime.tv_usec - last.ru_utime.tv_usec;
	rus.ru_stime.tv_sec  = curr.ru_stime.tv_sec  - last.ru_stime.tv_sec;
	rus.ru_stime.tv_usec = curr.ru_stime.tv_usec - last.ru_stime.tv_usec;
	rus.ru_maxrss =  curr.ru_maxrss -  last.ru_maxrss;
	rus.ru_ixrss  =  curr.ru_ixrss  -  last.ru_ixrss ;
	rus.ru_idrss  =  curr.ru_idrss  -  last.ru_idrss ;
	rus.ru_isrss  =  curr.ru_isrss  -  last.ru_isrss ;
	rus.ru_minflt =  curr.ru_minflt -  last.ru_minflt;
	rus.ru_majflt =  curr.ru_majflt -  last.ru_majflt;
	rus.ru_nswap  =  curr.ru_nswap  -  last.ru_nswap ;
	rus.ru_inblock = curr.ru_inblock - last.ru_inblock;
	rus.ru_oublock = curr.ru_oublock - last.ru_oublock;
	rus.ru_msgsnd  = curr.ru_msgsnd  - last.ru_msgsnd ;
	rus.ru_msgrcv  = curr.ru_msgrcv  - last.ru_msgrcv ;
	rus.ru_nsignals= curr.ru_nsignals- last.ru_nsignals;
	rus.ru_nvcsw   = curr.ru_nvcsw	-  last.ru_nvcsw  ;
	rus.ru_nivcsw  = curr.ru_nivcsw  - last.ru_nivcsw ;

	/* last = curr */ 
	last.ru_utime.tv_sec  = curr.ru_utime.tv_sec;
	last.ru_utime.tv_usec = curr.ru_utime.tv_usec;
	last.ru_stime.tv_sec  = curr.ru_stime.tv_sec;
	last.ru_stime.tv_usec = curr.ru_stime.tv_usec;
	last.ru_maxrss =  curr.ru_maxrss;
	last.ru_ixrss  =  curr.ru_ixrss;
	last.ru_idrss  =  curr.ru_idrss;
	last.ru_isrss  =  curr.ru_isrss;
	last.ru_minflt =  curr.ru_minflt;
	last.ru_majflt =  curr.ru_majflt;
	last.ru_nswap  =  curr.ru_nswap;
	last.ru_inblock = curr.ru_inblock;
	last.ru_oublock = curr.ru_oublock;
	last.ru_msgsnd  = curr.ru_msgsnd;
	last.ru_msgrcv  = curr.ru_msgrcv;
	last.ru_nsignals= curr.ru_nsignals;
	last.ru_nvcsw	= curr.ru_nvcsw;
	last.ru_nivcsw  = curr.ru_nivcsw;
	 
	/* send result */
	*ut_se = rus.ru_utime.tv_sec;
	*ut_us = rus.ru_utime.tv_usec;
	*st_se = rus.ru_stime.tv_sec;
	*st_us = rus.ru_stime.tv_usec;

	#ifdef DEBUG
	printf("gus_cpu -- cpu %d.%0.2d user %d.%0.2d syst\n", *ut_se, *ut_us/10000, *st_se, *st_us/10000);
	#endif
}

/*--------------------------------------------------- gus_mem --*/
void  gus_mem (long* maxrss, long* ixrss, long* idrss, long* isrss, long* minflt, long* majflt, long* nswap) {
/* les valeurs sont extraites de la structure remplie par
 * l'appel précédent à gus_cpu                             */
	*maxrss = rus.ru_maxrss;
	*ixrss  = rus.ru_ixrss;
	*idrss  = rus.ru_idrss;
	*isrss  = rus.ru_isrss;
	*minflt = rus.ru_minflt;
	*majflt = rus.ru_majflt;
	*nswap  = rus.ru_nswap;
}

/*--------------------------------------------------- gus_io  --*/
void gus_io (long* inblock, long* oublock,long* msgsnd, long* msgrcv,long* nsignals, long* nvcsw, long* nivcsw) {
/* les valeurs sont extraites de la structure remplie par
 * l'appel précédent à gus_cpu                             */
	*inblock = rus.ru_inblock;
	*oublock = rus.ru_oublock;
	*msgsnd  = rus.ru_msgsnd; 
	*msgrcv  = rus.ru_msgrcv;
	*nsignals= rus.ru_nsignals;
	*nvcsw   = rus.ru_nvcsw; 
	*nivcsw  = rus.ru_nivcsw;
}
