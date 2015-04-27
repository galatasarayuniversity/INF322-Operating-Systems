/*
 * gulce.c
 *
 *  Created on: Mar 19, 2015
 *      Author: gulce
 */

#include <stdio.h>
/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>
/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <conio.h>

int main(int argc, const char *argv[])
{
	pid_t child_pid;

	int rastgele;
	int status = -1;

	/* rastgele case 0
	 * status defaultun içindeydi ama hata verdi buraya tanımlayınca oldu ?
	 */

	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */
		printf("HATA SÜREÇ YARATILAMADI");
		break;

	case 0:
		/* This is where child continues */
		//int rastgele;
		rastgele = rand() % 10;
		printf("child says %d\n", rastgele);
		sleep(3);
		exit(rastgele);
		break;

	default:
		/* This is where parent continues */
		//int status = -1;
		waitpid(-1, &status, 0);
		printf("parent says:%d", WEXITSTATUS(status));
		break;
	}

	/* Both parent and child reaches here */
	sleep(6);

	exit(EXIT_SUCCESS);
}
