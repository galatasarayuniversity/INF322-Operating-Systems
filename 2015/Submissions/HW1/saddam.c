#include <stdio.h>
/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>

/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>

int main(int argc, const char *argv[])
{
	pid_t child_pid;
	pid_t waitpid(pid_t pid, int *status, int options);
	int x = 111;

	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */
		printf("Error");
		break;

	case 0:
		/* This is where child continues */
		x = x * 3;
		printf("I am the child with PID=%d and PPID=%d\n", getpid(), getppid());
		printf("[%d],x=%d\n", getpid(), x);

		srand(time(NULL));
		int nb;
		nb = rand() % 10;
		printf("random number= %d\n", nb);
		int status = WIFEXITED(status) + WIFSIGNALED(status);

		if (waitpid(-1, &status, 0) != 0) {
			sleep(3);
			printf("A child who is status changing,child'sNewStatus=%d\n", waitpid(-1, &status, 0));
			exit(nb);
		}
		//exit(EXIT_SUCCESS);
		break;

	default:
		/* This is where parent continues */
		printf("I am the parent with PID=%d\n", getpid());
		printf("[%d],x=%d\n", getpid(), x);
		break;
	}
	/* Both parent and child reaches here */

	sleep(6);

	exit(EXIT_SUCCESS);
}
