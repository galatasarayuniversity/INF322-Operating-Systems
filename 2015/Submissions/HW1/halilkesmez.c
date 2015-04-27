/*Halil Kesmez
 *Galatasaray University
 *Computer Science
 *Operating System
 */
#include <stdio.h>
	/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>
	/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>
	/* For rand(),srand() */
#include<curses.h>
	/* For time() */
#include<time.h>
int main(int argc, const char *argv[])
{
	pid_t child_pid, wait_pid;
	int random, status;
	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */
		break;

	case 0:
		/* This is where child continues */
		printf("i am a child\n");
		srand(time(NULL));
		random = rand() % 10;
		sleep(3);
		exit(random);
		break;

	default:
		printf("i am a parent\n");
		do {
			wait_pid = waitpid(child_pid, &status, WUNTRACED | WCONTINUED);
			if (wait_pid == -1) {
				/*error waitpid */
				exit(EXIT_FAILURE);
			}

			if (WIFEXITED(status)) {
				/*Exited child process */
				printf("exit value of child(random number) = %d\n", WEXITSTATUS(status));
			} else if (WIFSIGNALED(status)) {
				/*Stopped child process unnormally */
				printf("process stopped unnormally %d\n", WTERMSIG(status));
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		break;
	}
	sleep(1);

	exit(EXIT_SUCCESS);
}
