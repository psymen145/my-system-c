#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int my_system(const char* string){
   pid_t childpid, ret_val;
   int num_tokens;
   int status;
   char** argv;  

   if((childpid = fork()) == 0){   //make argv delimited by whitespace
      if((num_tokens = makeargv(string," ",&argv)) == -1){
         perror("Makeargv function failed");
      }
      if(execvp(argv[0], argv) == -1){
         perror("Error with exec:"); 
         _exit(1);
      }
      exit(EXIT_SUCCESS);
   }
   else if(childpid == -1){
      perror("The fork failed");
      exit(EXIT_FAILURE);
   }
   else{
      ret_val = waitpid(childpid,&status,0);
      if(WIFEXITED(status)){
         return WEXITSTATUS(status); 
      }
      else if(WIFSIGNALED(status)){
         printf("%s killed by signal %d\n",argv[0],WTERMSIG(status));
         if(WCOREDUMP(status)){
            printf("Core file was produced\n");
            return -1;
         }
         return -1; 
      }
      else if(WIFSTOPPED(status)){
         printf("%s stopped by signal %d\n",argv[0],WIFSTOPPED(status));
         return -1;
      }
      exit(EXIT_SUCCESS);
   }
}

/*
 * Make argv array (*arvp) for tokens in s which are separated by
 * delimiters.   Return -1 on error or the number of tokens otherwise.
 */
int makeargv(char *s, char *delimiters, char ***argvp)
{
   char *t;
   char *snew;
   int numtokens;
   int i;
    /* snew is real start of string after skipping leading delimiters */
   snew = s + strspn(s, delimiters);
                              /* create space for a copy of snew in t */
   if ((t = calloc(strlen(snew) + 1, sizeof(char))) == NULL) {
      *argvp = NULL;
      numtokens = -1;
   } else {                     /* count the number of tokens in snew */
      strcpy(t, snew);
      if (strtok(t, delimiters) == NULL)
         numtokens = 0;
      else
         for (numtokens = 1; strtok(NULL, delimiters) != NULL;
              numtokens++)
              ;  
                /* create an argument array to contain ptrs to tokens */
      if ((*argvp = calloc(numtokens + 1, sizeof(char *))) == NULL) {
         free(t);
         numtokens = -1;
      } else {            /* insert pointers to tokens into the array */
         if (numtokens > 0) {
            strcpy(t, snew);
            **argvp = strtok(t, delimiters);
            for (i = 1; i < numtokens + 1; i++)
               *((*argvp) + i) = strtok(NULL, delimiters);
         } else {
           **argvp = NULL;
           free(t);
         }
      }
   }   
   return numtokens;
}
