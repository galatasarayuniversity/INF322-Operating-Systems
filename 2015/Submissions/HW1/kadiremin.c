#include <stdio.h>

/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>

/* For generate random value */
#include <time.h>

/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>

int main(int argc, const char *argv[])
{
	pid_t child_pid;
	int random, status;
	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */
		break;

	case 0:
		/* This is where child continues */
		srand(time(NULL));
		random = rand() % 10;
		printf("Child has generated random value %d (this is child)\n", random);
		sleep(3);
		exit(random);
		break;

	default:
		/* This is where parent continues */
		waitpid(-1, &status, 0);
		printf("Child has returned the value %d (this is parent)\n", WEXITSTATUS(status));
		break;
	}

	/* Both parent and child reaches here */

	exit(EXIT_SUCCESS);
}
