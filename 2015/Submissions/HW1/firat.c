/*
 * exec4.c
 *
 *  Created on: Mar 16, 2015
 *      Author: firatlepirate
 */

#include <stdio.h>

/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>

/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>
#include<time.h>

int main(int argc, const char *argv[])
{
	pid_t child_pid;
	int x;

	switch ((child_pid = fork())) {

	case -1:
		/* Error creating child process */
		break;

	case 0:
		/* This is where child continues */

		printf("i am child with PID=%d and PPID=%d\n", getpid(), getppid());

		sleep(3);
		srand(time(NULL));
		x = rand() % 10;
		printf("[%d], x=%d\n", getpid(), x);
		exit(EXIT_SUCCESS);
		break;

	default:
		/* This is where parent continues */
		printf("i am the parent with with PID=%d\n", getpid());
		waitpid(0, EXIT_SUCCESS, 0);
		printf("i am the parent with PID: %d and this is the status of my child: %d, signal control: %d\n", getpid(), WIFEXITED(EXIT_SUCCESS),
		       WTERMSIG(EXIT_SUCCESS));

		break;
	}

	exit(EXIT_SUCCESS);
}
