#include <stdio.h>
#include <setjmp.h>
#include <sys/types.h>
#include <signal.h>

jmp_buf jb; 
/**
- fonction de protection pour gerer les erreurs
- variable ou sera sauvegarde le contexte de pile et d'environnement
- sera utilise ulterieurement par longjmp
- setjmp() et sigsetjmp() renvoient 0 lorsqu'elles reviennent directement, 
et une valeur non nulle si elles reviennent à travers un appel à longjmp() 
utilisant le contexte sauvegardé
*/
struct sigaction sigact_segv;

void handler_segv ()
{
  int retour_de_longjmp = 1; // permet de montrer qu'il y a une erreur 

  printf ("\nSIGSEGV recu\n");
  fflush (stdout);

/* la valeur de retour de lonjmp doit etre differente de  0;
 si on la met quand meme a 0, longjmp retourne 1    */
  longjmp (jb, retour_de_longjmp); // renvoie au dernier point de sauvegarde avec une valeur "retour_de_longjmp"
}

int probe (int *adr)
{
  int status;

/* status correspond en fait au retour de longjmp en cas de probleme,
 ou la valeur 0 sinon */

  if ((status = setjmp (jb)) != 0) // fixe un point de sauvegarde, sauvegarde le contexte
    return (0);			/* si l'instruction suspecte a echoue, on revient ici  */
  // adresse inaccessible en lecture
  else
    {
  // status=0 tout va bien
      printf ("\nvaleur=%d", *adr);	/* instruction suspecte */  // c'est ici que le signal est emi !!!
      return (1);		/* si l'instruction a pu etre executee, on continue ici  */
    // adresse accessible en lecture
    }
}

int main ()
{
  int i, index, tab[10], *ia1;
  sigact_segv.sa_handler=handler_segv;
  sigaction(SIGSEGV, &sigact_segv,NULL);
// SIGSEGV correspond a une tentative d'acces a une adresse memoire non definie
  index = 0;
  while (1)
    {
      ia1 = &tab[index]; // quand on arrivera a une adresse non definie, SIGSEGV sera 
      i = probe (ia1);
      if (i == 0)
      {
        printf ("probe fail at index=%d ia1=%d\n", index, ia1);
        break;
      }
      index++;
    }
    return 0;
}










