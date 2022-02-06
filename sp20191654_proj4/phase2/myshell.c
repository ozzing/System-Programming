/* $begin shellmain */
#include "myshell.h"
#include<errno.h>
#define MAXARGS   128

/* Function prototypes */
void eval(char* cmdline);
void evalpipe(char* cmdline);
int parseline(char* buf, char** argv);
int builtin_command(char** argv);
void unix_error(char* msg);
void app_error(char* msg);
void Remove(char* buf);

int main()
{
    char cmdline[MAXLINE]; /* Command line */
    while (1) {
        /* Read */
        printf("CSE4100-SP-P4> ");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin))
            exit(0);
        /* Evaluate */
        if (!strstr(cmdline, "|")) eval(cmdline);
        else evalpipe(cmdline);
    }
}
/* $end shellmain */

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char* cmdline)
{
    char* argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if (argv[0] == NULL)
        return;   /* Ignore empty lines */

    if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
        pid = fork();
        if (pid < 0) {
            unix_error("fork error");
        }
        else if (pid == 0) { /* child process */
            char* bin = (char*)malloc(sizeof(char) * MAXLINE);
            strcpy(bin, "/bin/");
            strcat(bin, argv[0]);
            if (strcmp("echo", argv[0]) == 0 && strncmp("`", argv[1], 1)==0) { /* echo command with ` */
                argv[1]++;  // delete first `
                argv[1] = strtok(argv[1], "`");  // delete next `
                char* cmd[] = { argv[1], 0 };
                strcpy(bin, "/bin/");
                strcat(bin, argv[1]);
                if (execve(bin, cmd, environ) < 0) {
                    printf("%s: Command not found.\n", cmd[0]);
                    exit(0);
                }
            }
            else if (execve(bin, argv, environ) < 0) {	//ex) /bin/ls ls -al &
                if (execve(argv[0], argv, environ) < 0) {
                    printf("%s: Command not found.\n", argv[0]);
                    exit(0);
                }
            }
        }
        /* Parent waits for foreground job to terminate */
        if (!bg) {
            int status;
            if (waitpid(pid, &status, 0) < 0)
                unix_error("waitfg: waitpid error");
        }
        else {//when there is backgrount process!
            printf("%d %s", pid, cmdline);
        }
    }
    return;
}

/* $begin evalpipe */
/* evalpipe - Evaluate a command line */
void evalpipe(char* cmdline)
{
    char* argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    int fd1[2], fd2[2];
    char* temp[MAXARGS];

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if (argv[0] == NULL)
        return;   /* Ignore empty lines */

    int i = 0, j = 0, p = 0;
    while (1) {
        while (i < MAXARGS) {   // for all argv[i]
            if (argv[i] == NULL) {  // last command
                temp[j] = NULL;
                j = 0;
                break;
            }
            else if (strcmp("|", argv[i]) == 0) {   // pipe found
                temp[j] = NULL;
                j = 0;
                i++;
                break;
            }
            else {  // copy string
                temp[j] = (char*)malloc(sizeof(char) * MAXLINE);
                strcpy(temp[j], argv[i]);
                j++;
            }
            i++;
        }
        if (pipe(fd1) < 0) {    // create pipe
            unix_error("pipe error");
        }
        if (!builtin_command(temp)) { //quit -> exit(0), & -> ignore, other -> run
            pid = fork();
            if (pid < 0) { /* fork */
                unix_error("fork error");
            }
            // child
            else if (pid == 0) {
                dup2(fd2[0], STDIN_FILENO); // get saved input from previous pipe
                if (argv[i] != NULL) dup2(fd1[1], STDOUT_FILENO); // if not last, write to pipe
                close(fd1[0]);
                char* bin = (char*)malloc(sizeof(char) * MAXLINE);
                strcpy(bin, "/bin/");
                strcat(bin, temp[0]);
                if (strcmp("echo", temp[0]) == 0 && strncmp("`", temp[1], 1) == 0) {    /* echo command with ` */
                    temp[1]++; // delete first `
                    temp[1] = strtok(temp[1], "`"); // delete next `
                    char* cmd[] = { temp[1], 0 };
                    strcpy(bin, "/bin/");
                    strcat(bin, temp[1]);
                    if (execvp(bin, cmd) < 0) {
                        printf("%s: Command not found.\n", cmd[0]);
                        exit(0);
                    }
                }
                else if (execvp(bin, temp) < 0) { 	//ex) /bin/ls ls -al &
                    if (execvp(temp[0], temp) < 0) {
                        printf("%s: Command not found.\n", temp[0]);
                        exit(0);
                    }
                }
            }
            // parent
            else {
                /* Parent waits for foreground job to terminate */
                if (!bg) {
                    int status;
                    if (waitpid(pid, &status, 0) < 0)
                        unix_error("waitfg: waitpid error");
                         
                    close(fd1[1]);
                    // save in temporary array
                    fd2[0] = fd1[0];
                    fd2[1] = fd1[1];
                }
                else {//when there is backgrount process!
                    printf("%d %s", pid, cmdline);
                } 
            }
        }
        if (argv[i] == NULL) break;
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char** argv)
{
    if (!strcmp(argv[0], "cd")) {
        if (argv[1] != NULL) {              /* cd to argv[1] command */
            if (chdir(argv[1]) != 0)
                fprintf(stderr, "cd error: %s\n", strerror(errno));
        }
        else {
            if (chdir(getenv("HOME")) != 0) /* cd to home command */
                fprintf(stderr, "cd error: %s\n", strerror(errno));
        }
        return 1;
    }
    if (!strcmp(argv[0], "quit")) /* quit command */
        exit(0);
    if (!strcmp(argv[0], "exit"))
        exit(0);
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
        return 1;
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char* buf, char** argv)
{
    char* delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf) - 1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        Remove(buf);
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
    if ((bg = (*argv[argc - 1] == '&')) != 0)
        argv[--argc] = NULL;

    return bg;
}
/* $end parseline */

/* $begin unixerror */
void unix_error(char* msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
/* $end unixerror */

void app_error(char* msg) /* Application error */
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

/* $begin Fgets */
char* Fgets(char* ptr, int n, FILE* stream)
{
    char* rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
        app_error("Fgets error");

    return rptr;
}
/* $end Fgets */

/* $begin Remove */
void Remove(char* buf)
{
    for (; *buf != '\0'; buf++) {
        if (*buf == '"' || *buf == '\'') {
            strcpy(buf, buf + 1);
            buf--;
        }
    }
}
/* $end Remove */
