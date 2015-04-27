/*12.03.2015*/
/*Sumeyye KONAK - 14401676*/

#include <stdio.h>

/* For exit() and EXIT_SUCCESS */
#include <stdlib.h>

/* For fork(), getpid(), getppid(), sleep() functions */
#include <unistd.h>

/* For waitpid() functions */
#include <sys/wait.h>

int main(int argc, const char *argv[])
{
	pid_t child_pid;
	pid_t pid;
	int status, random;
	int x = 111;

	//ebeveyn surec cocuk surec yaratiyor
	switch ((child_pid = fork())) {
	case -1:
		/* Error creating child process */
		break;

	case 0:		//cocuk surec
		/* This is where child continues */

		//      x *= 3;

		printf("I am the child with PID=%d and PPID=%d\n", getpid(), getppid());
		srand(time(NULL));
		random = rand() % 10;	//0 ile 9 arasi random sayi uretiliyor
		printf("Cocuk surec: RND = %d\n", random);	//cocuk surecin urettigi random sayinin ekrana basilmasi
		sleep(3);	//cocuk surec 3 sn uyusun.
		exit(random);	//rastgele sayiyi cikis degerine vererek sonlanma

		break;

	default:		//ebeveyn surec
		/* This is where parent continues */
		printf("I am the parent with PID=%d\n", getpid());

		//child'in durum degismesini bekliyoruz
		waitpid(-1, &status, 0);

		do {
			if (WIFEXITED(status) != 0) {	//cocuk surec normal yolla sonlandıysa, WIFEXITED 0'dan farkli deger dondurur
				printf("Exited, status=%d\n", WEXITSTATUS(status));	//Cocuk surecin cikis degerini bildirme
			} else if (WIFSIGNALED(status) != 0) {	//cocuk surec  bir sinyal yuzunden sonlandiysa, 0'dan farkli deger dondurur
				printf("Killed by signal %d\n", WTERMSIG(status));	//hangi sinyal yuzunden sonlandigini bildirme
			}

		} while ((WIFEXITED(status) != 0) && (WIFSIGNALED(status) != 0));
		exit(EXIT_SUCCESS);

		break;
	}

//      printf("[%d], x = %d\n", getpid(), x);
	/* Both parent and child reaches here */
	sleep(6);

	exit(EXIT_SUCCESS);

}
