/*
 * Odev2_HalilYavuz_Cevik_10401569.c
 *
 *  Created on: Apr 13, 2015
 *      Author: yc
 */

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<stdio.h>
#define BUF_SIZE 16
#define ERR_READ 16
int main(int argc, char *argv[])
{

	int *status;
	char *a_ptr;

	int sayac int wifexited;
	int status_val;
	status = &status_val;
	pid_t wait_pid;
	int src_fd, dest_fd;

	/* TODO: Boru hatti dosya tanimlayicilari icin bir dizi bildirimi yapin. */
	int pipe_fd[2];

	/* TODO: BUF_SIZE boyutlu bir karakter dizisi tanimlayin. */
	char a[BUF_SIZE];
	a_ptr = a;

	/* TODO: read()'in donus degerini tutacak bir num_read degiskeni tanimlayin. */
	ssize_t num_read;

	/* Komut satirindan parametre gecilmemis ise veya --help gecildiyse programi
	   sonlandir. */
	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(1);
	}

	/* TODO: Boru hattini yaratin. Basarisiz olursa program 1 degeri ile sonlansin. */
	if (pipe(pipe_fd) == -1) {

		perror("pipe");

		exit(1);
	}
	/* Yeni bir surec yaratmak icin fork'layin. */
	switch (fork()) {
	case -1:
		/* TODO: fork basarisiz oldu, 1 degeriyle cikin. Cikmadan once
		 * acilmis olan boru hatti uclarini kapatin. */
		close(pipe_fd[0]);

		exit(1);
		break;
	case 0:
		/* Cocuk surec boru hattinin okuyucusu olacak. */

		/* TODO: Boru hattinin cocuk tarafindan kullanilmayacak yazma
		 * ucunu kapatin. */
		close(pipe_fd[1]);

		/* TODO: Surec oncelikle ekrana kendi PID'ini sonuna '\n' koymadan yazsin.
		 * Burada printf kullanabilirsiniz. */

		printf("%d", getpid());

		while (1) {
			/* TODO: Boru hattindan BUF_SIZE bayt bloklar halinde okuma
			 * yapin. */
			num_read = read(src_fd, a, BUF_SIZE);

			/* TODO: Okuma hatasi olduysa surec sonlansin. Acik boru hatti
			 * ucunu da kapatmayi unutmayin. */

			if (num_read == -1) {
				perror("read");
				close(pipe_fd[0]);
				exit(ERR_READ);

			}
			/*
			 *

			 TODO: read()'in donus degerini degerlendirerek borunun
			 yazma ucunun kapatildigi ya da daha fazla yazilacak bir sey
			 olmadigi sonucuna variliyorsa ona gore donguden cikin. */

			if (num_read > 0 || close(pipe_fd[0]) == 0) {
				perror("read");
				exit(ERR_READ);
			}

			/* TODO: Borudan okunup tampona atilan veriyi BUYUK HARFE
			 * CEVIRIN. Bunun icin tamponu karakter karakter gezip
			 *
			 *
			 *
			 *
			 alfabetik ve kucuk karakterse buyuk harfe cevirmeniz
			 gerekmektedir. Daha sonra tamponu ekrana yani
			 stdout akisina write() ile yazin.
			 Bu esnada write()'in donus degerini de kontrol edin, hata olursa
			 sureci 1 degeriyle sonlandirin. */

			for (sayac = 0; sayac < BUF_SIZE; sayac++) {

				a[sayac] = a[sayac] - 32;
			}

			if (write(stdout, a_ptr, BUF_SIZE) != BUF_SIZE) {

				exit(1);
			}

		}
		/* TODO: write() ile stdout akisina yeni-satir (\n) karakteri yazin. */

		/* TODO: Son olarak borunun okuma ucunu kapatin. */
		close(pipe_fd[0]);

		/* Her sey basariyla bu noktaya geldiyse cocuk surec 0 ile cikarak
		 * basarili bir sekilde bittigini bildiriyor. */
		exit(0);

	default:
		/* Ebeveyn surec boruya yazacak */

		/* TODO: Boru hattinin ebeveyn tarafindan kullanilmayacak okuma
		 * ucunu kapatin. */
		close(pipe_fd[0]);

		/* TODO: Komut satiri argumanini boru hattina yazin. Yazmaniz gereken
		   veri miktari
		   * write() tarafindan yazilmis olandan farkliysa hata olmus
		   * demektir, ona gore programi 1 ile sonlandirin.
		 */
		if () {
		}

		/* TODO: Yazma ucunu kapatin */
		close(pipe_fd[1]);

		/* TODO: Cocugun bitisini bekleyin (bkz ilk odev) Cocuk 0'dan
		 * farkli bir degerle sonlandiysa ekrana bunu belirten bir mesaj
		 * yazdirin. */

		wait_pid = waitpid(-1, status, 0);
		wifexited = WIFEXITED(status_val);

		if (wifexited) {
			printf("Child Process Exited\n");
			//Learning Child Process's return value with using WEXITSTATUS()
			status_val = WEXITSTATUS(status_val);
		}

		printf("Changed Child Process's Pid %d , Child Return Value : %d\n", wait_pid, status_val);

		/* Artik ebeveyn surec basariyla sonlanabilir */
		exit(0);
	}
}
