#include <stdio.h>

/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>

/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>

int main(int argc, const char *argv[])
{
  pid_t child_pid;

  switch ((child_pid = fork())) {
  case -1:
    /* Error creating child process */
    break;

  case 0: ;
    /* This is where child continues */
    srand((unsigned) time(NULL));
    int r = rand() % 10;
    sleep(3);
    exit(r);
    //exit(EXIT_SUCCESS);
    break;

  default:;
    int status;
    /* This is where parent continues */
    pid_t pid;
    pid = waitpid(-1, &status, 0);
    printf("Child Pid = %d\n\
Status = %d\n\
WIFEXITED = %d\n\
WEXITSTATUS = %d\n",pid,status,WIFEXITED(status),WEXITSTATUS(status));
    
    break;
  }

  /* Both parent and child reaches here */
  //sleep(3);

    exit(EXIT_SUCCESS);
}
