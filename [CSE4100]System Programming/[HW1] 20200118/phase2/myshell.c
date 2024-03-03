/* $begin shellmain */
#include "myshell.h"
//#include<errno.h>
#define MAXARGS   128
//global
int is_pipe;
int pipe_arr[MAXARGS];
int pipe_num;
int idx = 0;
/* Function prototypes */
void    eval(char *cmdline);
int parseline(char *buf, char **argv);//, int *pipe_idx);
int builtin_command(char **argv); 
void    running_pipe(char **argv, int *pipe_idx);
void    pipe_tok(char **argv, char *token1[], char *token2[]);
void    exec_pipe(char **argv, char *token1[], char *token2[]);
void    unix_error(char *msg) /* Unix-style error */
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
void    eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	    return;   /* Ignore empty lines */

    if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
        if((pid = fork()) == 0){ //mycode
        if(is_pipe) //pipe 존재 여부
    {
        char *token1[MAXARGS] = {0,};
        char *token2[MAXARGS] = {0,};
        pipe_tok(argv, token1, token2);
        exec_pipe(argv, token1, token2);
    } 
         else if (execvp(argv[0], argv) < 0) {	//ex) /bin/ls ls -al &
           printf("%s: Command not found.\n", argv[0]);
            exit(0); 
            }
        }

	/* Parent waits for foreground job to terminate */
	if (!bg){ 
	    int status;
       if(waitpid(pid, &status, 0) < 0)
           unix_error("waitfg: watipid error");
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

int parseline(char *buf, char **argv) //, int *pipe_idx) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */
    int token_num = 0;   //token 개수
   
    is_pipe = 0; //init
    pipe_num = 0;
    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	    buf++;
    /* Build the argv list */
    argc = 0;
    for(int i = 1; i < strlen(buf); i++)
    {
        if(buf[i] == '|')
	{
         pipe_arr[pipe_num] = token_num + 1;
	 pipe_num++;    
	 is_pipe = 1; //pipe 존재
	 buf[i] = ' ';
	}
        else if(buf[i] != ' ' && buf[i-1] == ' ')
            token_num++; //실질적인 토큰 개수
    }
    pipe_arr[pipe_num] = 0;
    while ((delim = strchr(buf, ' ')) || (delim = strchr(buf, '|'))){
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
void    pipe_tok(char **argv, char *token1[], char *token2[])
{
     int pipe_idx;
     int i; 
     int s_idx = 0;    
 	if (idx == 0) 
	pipe_idx = pipe_arr[idx];
	else
	 pipe_idx = pipe_arr[idx] - pipe_arr[idx - 1];
	for(i = s_idx; i < pipe_idx; i++)
		token1[i] = argv[i];
	token1[i] = 0;
	for(i = pipe_idx; argv[i] != 0; i++)
	   token2[i-pipe_idx] = argv[i];
	token2[i] = 0;
	return ; 
}
void    exec_pipe(char **argv, char *token1[], char *token2[])
{
    pid_t pid;
    char *tmp1[MAXARGS] = {0,};
    char *tmp2[MAXARGS] = {0, };
    int fd[2];
    //fd[0] -> input
    //fd[1] -> output
    //STDIN_FILENO : 0
    //STDOUT_FILENO : 1
    if(pipe(fd) < 0)
        exit(-1);
    if((pid = Fork()) == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[STDIN_FILENO]); 
	close(fd[STDOUT_FILENO]);
        if(execvp(token1[0], token1) < 0){
           printf("Error. pipe failed\n");
            exit(0); 
        }
        
    }
    else
    {
        dup2(fd[0], STDIN_FILENO);
	close(fd[STDOUT_FILENO]);        
	close(fd[STDIN_FILENO]);
	idx++;
	if(pipe_arr[idx] != 0)
	{
        pipe_tok(token2, tmp1, tmp2);
    	exec_pipe(argv, tmp1, tmp2);
	}
        else{
        if (execvp(token2[0], token2) < 0){
            printf("Error. pipe failed\n");
            exit(1);} 
            }
    return ;
    }
}
