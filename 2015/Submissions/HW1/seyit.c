/*
 * ex4.c
 *
 *  Created on: Mar 19, 2015
 *      Author: seyit
 */

#include <stdio.h>

/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>

/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>

#include <time.h>

int main(int argc, const char *argv[])
{
	pid_t child_pid;
	int status;

	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */

		printf("Error\n");
		break;

	case 0:
		/* This is where child continues */
		printf("Çocuk süreç oluşturuldu.PID = %d\n", getpid());

		srand(time(NULL));
		int rastgele = rand() % 10;

		sleep(3);

		exit(rastgele);
		break;

	default:
		/* This is where parent continues */

		if (waitpid(-1, &status, 0) == child_pid)
			printf("Çocuk süreç sonlandı.\n");;

		if (WIFEXITED(status))
			printf("Çocuk süreçte üretilen rastgele değer = %d\n", WEXITSTATUS(status));

		break;
	}

	/* Both parent and child reaches here */
//    sleep(6);

	exit(EXIT_SUCCESS);
}
