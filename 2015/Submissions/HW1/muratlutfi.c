#include <stdio.h>

/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>

/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>
#include <time.h>

int main(int argc, const char *argv[])
{
	pid_t child_pid;
	int x = 111;
	int status;
	int n;
	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */
		break;

	case 0:

		srand(time(NULL));
		n = rand() % 10;
		sleep(3);
		exit(n);
		break;

	default:
		/* This is where parent continues */
		waitpid(child_pid, &status, 0);
		if (WIFEXITED(status)) {
			printf("%d", WEXITSTATUS(status));
		}
		break;
	}

	/* Both parent and child reaches here */
	sleep(1);

	exit(EXIT_SUCCESS);
}
