## my-system-c

## Assignment 3 - Using fork(), exec() and waitpid()

In this assignment you are going to write your own version of the library function, system(). An excerpt from the man pages on how to use system() is: 
 SYSTEM(3)                  Linux Programmer’s Manual                 SYSTEM(3)

NAME
       system - execute a shell command

SYNOPSIS
       '''
       #include <stdlib.h>

       int system(const char *command);
       '''

DESCRIPTION
system() executes a command specified in command by calling /bin/sh -c command, and returns after the command has been completed.  During execution of the command, SIGCHLD will be blocked, and SIGINT and SIGQUIT will be ignored.

RETURN VALUE
The value returned is -1 on error (e.g.  fork(2) failed), and the return status of the command otherwise.  This latter return status is in the format specified in wait(2).  Thus, the exit code of the command will be WEXITSTATUS(status).  In case /bin/sh could not be executed, the exit status will be that of a command that does exit(127). If the value of command is NULL, system() returns non-zero if the shell is available, and zero if not. system() does not affect the wait status of any other children.


An example of using system() is:

#include        <stdio.h>
#include        <stdlib.h>

int main()
{

   int  status;

        status = system("date");
        printf("The exit status of system() is %d\n",status);

        exit(0);
}
The output of this program is:

[whitlock@websol01 hw]$ ./a.out
Wed Oct  4 14:50:29 EDT 2017
The exit status of system() is 0
ASSIGNMENT
You will be writing the function my_system(const char *string); however, your function will not invoke the shell. Instead, your function will carry out the following actions listed below. (By not invoking the shell, we are limiting the versatility of my_system()).
The function will fork() a child which will execute the passed parameter string. The original function (parent) will use waitpid() to wait for this particular child. Don't use the WNOHANG option.
The child process will use the supplied function makeargv() to create the argv[] array from the string. That is, if the string is "date", makeargv() will create argv[0] = "date", argv[1] = 0. You may create your own version of makeargv() if you wish.
The child process will use execvp() to carry out the command. (See page pages 470 - 472 of our text or the man pages.)
If the execvp() system call fails, the child process should use perror() to write out the errno value and use _exit() to return an error status (like 1).
Upon returning from waitpid(), the parent process uses the wstat macros (page 475 in text or man pages) to evaulate whether the child process exited normally.
If WIFEXITED() is true, my_system() returns the WEXITSTATUS() value.
If WIFSIGNALED() is true, my_system() prints the string: "argv[0] killed by signal x" and returns -1. Of course, your function fills in the actual values of argv[0] and x.
If WIFSTOPPED() is true, my_system() prints the string: "argv[0] stopped by signal x" and returns -1. Of course your function fills in the actual values of argv[0] and x.
If WCOREDUMP() is true, my_system() prints the string: "Core file was produced" and returns -1. This can only be true if WIFSIGNALED() returned true.
