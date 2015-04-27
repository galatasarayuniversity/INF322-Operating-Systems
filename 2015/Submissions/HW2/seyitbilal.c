/*
 * hw2_skeleton.c
 *
 *  Created on: Apr 12, 2015
 *      Author: seyit
 */

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 16

int main(int argc, char *argv[])
{

	/* TODO: Boru hatti dosya tanimlayicilari icin bir dizi bildirimi yapin. */
	int pipe_fd[2];

	/* TODO: BUF_SIZE boyutlu bir karakter dizisi tanimlayin. */
	char text[BUF_SIZE];

	/* TODO: read()'in donus degerini tutacak bir num_read degiskeni tanimlayin. */
	int num_read, num_write;

	/* Komut satirindan parametre gecilmemis ise veya --help gecildiyse programi sonlandir. */
	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(1);
	}

	/* TODO: Boru hattini yaratin. Basarisiz olursa program 1 degeri ile sonlansin. */
	if (pipe(pipe_fd) == -1) {
		perror("boru hattı yaratılamadı\n");
		exit(1);
	}

	/* Yeni bir surec yaratmak icin fork'layin. */
	switch (fork()) {
	case -1:
		/* TODO: fork basarisiz oldu, 1 degeriyle cikin. Cikmadan once
		 * acilmis olan boru hatti uclarini kapatin. */
		perror("fork başarısız oldu.\n");

		if (close(pipe_fd[0]) == -1)
			perror("okuma ucu kapatılamadı.1\n");

		if (close(pipe_fd[1]) == -1)
			perror("yazma ucu kapatılamadı.\n");

		exit(1);
	case 0:
		/* Cocuk surec boru hattinin okuyucusu olacak. */

		/* TODO: Boru hattinin cocuk tarafindan kullanilmayacak yazma
		 * ucunu kapatin. */
		if (close(pipe_fd[1]) == -1)
			perror("yazma ucu kapatılamadı.\n");

		/* TODO: Surec oncelikle ekrana kendi PID'ini sonuna '\n' koymadan yazsin.
		 * Burada printf kullanabilirsiniz. */
		printf("child's PID : %d\n", getpid());

		while (1) {
			/* TODO: Boru hattindan BUF_SIZE bayt bloklar halinde okuma
			 * yapin. */
			num_read = read(pipe_fd[0], text, BUF_SIZE);

			/* TODO: Okuma hatasi olduysa surec sonlansin. Acik boru hatti
			 * ucunu da kapatmayi unutmayin. */
			if (num_read == -1) {
				perror("okuma başarısız.\n");

				if (close(pipe_fd[0]) == -1)
					perror("okuma ucu kapatılamadı.2\n");

				exit(1);
			}

			/* TODO: read()'in donus degerini degerlendirerek borunun
			 * yazma ucunun kapatildigi ya da daha fazla yazilacak bir sey
			 * olmadigi sonucuna variliyorsa ona gore donguden cikin. */
			if (num_read == 0)
				break;

			/* TODO: Borudan okunup tampona atilan veriyi BUYUK HARFE
			 * CEVIRIN. Bunun icin tamponu karakter karakter gezip
			 * alfabetik ve kucuk karakterse buyuk harfe cevirmeniz
			 * gerekmektedir. Daha sonra tamponu ekrana yani
			 * stdout akisina write() ile yazin.
			 *
			 * Bu esnada write()'in donus degerini de kontrol edin, hata olursa sureci 1
			 * degeriyle sonlandirin. */
			int i;
			for (i = 0; i < strlen(text); i++)
				if (text[i] >= 97 && text[i] <= 122)
					text[i] = text[i] - 32;

			num_write = write(STDOUT_FILENO, text, num_read);
			if (num_write == -1) {
				perror("yazma başarısız.\n");

				if (close(pipe_fd[0]) == -1)
					perror("okuma ucu kapatılamadı.4\n");

				exit(1);
			}
		}

		/* TODO: write() ile stdout akisina yeni-satir (\n) karakteri yazin. */
		num_write = write(STDOUT_FILENO, "\n", strlen("\n"));

		if (num_write == -1) {
			perror("yazma başarısız.\n");

			if (close(pipe_fd[0]) == -1)
				perror("okuma ucu kapatılamadı.5\n");

			exit(1);
		}
		/* TODO: Son olarak borunun okuma ucunu kapatin. */
		if (close(pipe_fd[0]) == -1)
			perror("okuma ucu kapatılamadı.6\n");

		/* Her sey basariyla bu noktaya geldiyse cocuk surec 0 ile cikarak
		 * basarili bir sekilde bittigini bildiriyor. */
		exit(0);

	default:
		/* Ebeveyn surec boruya yazacak */

		/* TODO: Boru hattinin ebeveyn tarafindan kullanilmayacak okuma
		 * ucunu kapatin. */
		if (close(pipe_fd[0]) == -1)
			perror("okuma ucu kapatılamadı.7\n");

		/* TODO: Komut satiri argumanini boru hattina yazin. Yazmaniz gereken veri miktari
		 * write() tarafindan yazilmis olandan farkliysa hata olmus
		 * demektir, ona gore programi 1 ile sonlandirin.
		 */

		num_write = write(pipe_fd[1], argv[1], strlen(argv[1]));

		if (num_write == -1) {
			perror("yazma başarısız.\n");

			if (close(pipe_fd[1]) == -1)
				perror("yazma ucu kapatılamadı.\n");

			exit(1);
		}

		/* TODO: Yazma ucunu kapatin */
		if (close(pipe_fd[1]) == -1)
			perror("yazma ucu kapatılamadı.\n");

		/* TODO: Cocugun bitisini bekleyin (bkz ilk odev) Cocuk 0'dan
		 * farkli bir degerle sonlandiysa ekrana bunu belirten bir mesaj
		 * yazdirin. */
		int status;
		waitpid(-1, &status, 0);

		if (WIFEXITED(status))
			if (WEXITSTATUS(status) != 0)
				printf("hata oluştu.\n");

		/* Artik ebeveyn surec basariyla sonlanabilir */
		exit(0);
	}
}
