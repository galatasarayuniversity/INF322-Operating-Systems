#include <stdio.h>

/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>

/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>
#include <time.h>		// rastgele sayı üretmek için

int main(int argc, const char *argv[])
{
	pid_t child_pid;
	int status;

	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */
		break;

	case 0:
		/* This is where child continues */

		srand(time(NULL));
		int rastgele = rand() % 10;	//0-9 arasında sayı üretir.

		/* printf("rasgele sayı %d\n",rastgele);                                          KONTROL
		   printf("I am the child with PID=%d and PPID=%d\n",getpid(),getppid());          KONTROL     */

		sleep(3);
		exit(rastgele);

		break;

	default:
		/* This is where parent continues */

		child_pid = waitpid(-1, &status, 0);

		/* waitpid() geriye durumu değişen çocuğun pid'ini dondurur
		   printf("durumu değişen çocuğun pidi = %d",child_pid);                            KONTROL
		   printf("I am the parent with PID=%d\n", getpid());                               KONTROL     */

		if (WIFEXITED(status)) {
			printf("normal termination, exit status child = %d\n", WEXITSTATUS(status));
		}

		else if (WIFSIGNALED(status)) {
			printf("abnormal termination, signal number= %d\n", WTERMSIG(status));
		}

		break;
	}

	exit(EXIT_SUCCESS);
}
