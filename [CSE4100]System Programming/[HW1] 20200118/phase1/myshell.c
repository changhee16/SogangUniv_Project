/* $begin shellmain */
#include "myshell.h"
//#include<errno.h>
#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 

void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
pid_t Fork(void) 
{
    pid_t pid;

    if ((pid = fork()) < 0)
	unix_error("Fork error");
    return pid;
}
int main() 
{
    char cmdline[MAXLINE]; /* Command line */
   //signal_관련
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    while (1) {
 
	/* Read */
	printf("CSE4100-SP-P#1> ");                   
	fgets(cmdline, MAXLINE, stdin);
    if (feof(stdin))
	    exit(0);
	/* Evaluate */
	eval(cmdline);
    } 
}
/* $end shellmain */
  
/* $begin eval */ 
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{

    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
   // char path[MAXLINE] = "/bin/";
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */
    if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
        if((pid = Fork()) == 0){ //mycode
        if (execvp(argv[0], argv) < 0) {	//ex) /bin/ls ls -al &
           printf("%s: Command not found.\n", argv[0]);
            exit(0); 
            }
        }

	/* Parent waits for foreground job to terminate */
	if (!bg){ 
	    int status;
       if(waitpid(pid, &status, 0) < 0)
       {
           unix_error("waitfg: watipid error");
  /*    fprintf(stderr, "waitfg: watipid error: %s\n", strerror(errno));
       exit(0);*/
       }
	}
	else//when there is backgrount process!
	    printf("%d %s", pid, cmdline);
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "exit")) /* quit command */
	    exit(0);  
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
	    return 1;
    //cd: special case
    if(!strcmp(argv[0], "cd"))
    {
        if(!chdir(argv[1]))
            return 1;
        else
            fprintf(stderr, "%s: no such file or directory: %s\n", argv[0], argv[1]);
    }
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */
    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	    buf++;
    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        if(*buf == '\'' || *buf == '\"')
        {
            char tmp = *buf;
            buf++;
            delim = strchr(buf, tmp);
        }
	    argv[argc++] = buf;
	    *delim = '\0';
	    buf = delim + 1;
	    while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	    return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	    argv[--argc] = NULL;

    return bg;
}
/* $end parseline */


