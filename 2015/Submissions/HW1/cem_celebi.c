/*
 * main.c
 *
 *  Created on: Mar 18, 2015
 *      Author: cemcelebi
 */

#include <stdio.h>

/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>

/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>
#include <time.h>
#include "side_functions.h"
/*
 *
int main(int argc, const char *argv[])
{
  ///+/+/++//+/+/+/+/+//+/+//+//Alistirma 1
    pid_t child_pid;
    pid_t parent_pid;
    int x = 111;

    switch ((child_pid = fork())) {
        case -1:
            // Error creating child process

            break;

        case 0:
            // This is where child continues

            child_pid=getpid();
            printf("I am the child with PID= %d and PPID= %d \n",child_pid,getppid());
            x=x*3;
            exit(EXIT_SUCCESS);
            break;

        default:
            // This is where parent continues
            parent_pid=getpid();
            printf("I am the parent with PID= %d \n",parent_pid);
            break;
    }*/

int randr()
{
	srand(time(NULL));
	return rand() % 10;
}

int main(int argc, const char *argv[])
{
	pid_t child_pid;
	int x = 111, a = 5;
	int xstatus;
	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */
		//printf("ttt \n");
		break;

	case 0:
		/* This is where child continues */
		//printf("ttt \n");
		a = randr();
		sleep(3);
		//
		printf("xstatus: %d \n", xstatus);
		exit(a);
		break;

	default:
		printf("xstatus: %d \n", xstatus);
		WEXITSTATUS(xstatus);
		/* This is where parent continues */
		if (waitpid(child_pid, &xstatus, 0) == child_pid && WIFEXITED(xstatus) != 0) {
			// child exited or interrupted; now you can do something with status
			printf("child pid= %d ; degisen cocugun value'su= %d", waitpid(child_pid, &xstatus, 0), WIFEXITED(xstatus));

		} else {
			// error etc
		}
		break;
	}

	/* Both parent and child reaches here */
	// sleep(6);
	exit(EXIT_SUCCESS);
}

   // printf("I am the (parent?) process with PID= %d and x= %d ",getpid() ,x);
    // Both parent and child reaches here
   //deneme_fork_three();
	//ex3();
