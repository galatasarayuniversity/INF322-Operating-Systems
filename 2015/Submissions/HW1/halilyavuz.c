/*
 * Odev1_HalilYavuz_Cevik_10401569.c
 *
 *  Created on: Mar 19, 2015
 *      Author: yc
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>

/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>

pid_t waitpid(pid_t pid, int *status, int options);

int main(int argc, const char *argv[])
{

	int *status;
	int random;
	int wifexited;
	int status_val;
	status = &status_val;

	pid_t child_pid, wait_pid;

	srand(time(NULL));

	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */
		break;

	case 0:
		/* This is where child continues */

		printf("Fork() Successful!\nParent pid : %d , Child pid : %d\n", getppid(), getpid());
		random = rand() % 10;
		sleep(3);

		exit(random);
		break;

	default:

		wait_pid = waitpid(-1, status, 0);
		wifexited = WIFEXITED(status_val);

		if (wifexited) {
			printf("Child Process Exited\n");
			//Learning Child Process's return value with using WEXITSTATUS()
			status_val = WEXITSTATUS(status_val);
		}

		printf("Changed Child Process's Pid %d , Child Return Value : %d\n", wait_pid, status_val);

		/* This is where parent continues */

		break;
	}

	/* Both parent and child reaches here */

	exit(EXIT_SUCCESS);
}
