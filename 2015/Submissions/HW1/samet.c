
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	pid_t child;
	int cstatus;		/* cocuğun çıkış durumu */
	pid_t c;		/* beklenen çocuğun pid si */

	if ((child = fork()) == 0) {
		/* çocuk süreç */
		printf("Child: PID of Child = %d and Parent: PPID =%d \n", getpid(), getppid());
		srand(time(NULL));
		cstatus = rand() % 10;
		sleep(3);
		exit(cstatus);
	} else {		/* Ana süreç */
		if (child == (pid_t) (-1)) {
			fprintf(stderr, "Fork failed.\n");
			exit(1);
		} else {
			c = waitpid(child, &cstatus, 0);	/* çocuk sürecin tamamlanmasını bekle . */
			if (WIFEXITED(cstatus)) {
				printf("Parent: Child %d exited with status = %d\n", c, WEXITSTATUS(cstatus));
			}
			return 0;
		}
	}
}
