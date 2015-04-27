#include <stdio.h>

/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>
#include <time.h>
/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>
int main(int argc, const char *argv[])
{
	pid_t child_pid;
	int x = 111;
	int random = 0;
	int status;

	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */
		break;

	case 0:
		/* This is where child continues */
		srand(time(NULL));
		random = rand() % 10;
		//printf("%d\n",random); kontrol amaclı eklemistim
		sleep(3);
		exit(random);
		break;

	default:
		waitpid(child_pid, &status, 0);
		printf("Exit code: %d\n", WEXITSTATUS(status));
		break;

		/* This is where parent continues */

	}

	/* Both parent and child reaches here */

	exit(EXIT_SUCCESS);
}
