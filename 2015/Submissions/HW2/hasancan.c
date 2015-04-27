#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 16

int main(int argc, char *argv[])
{
	/* TODO: Boru hatti dosya tanimlayicilari icin bir dizi bildirimi yapin. */
	int pipe_fd[2];
	pid_t pid;
	/* TODO: BUF_SIZE boyutlu bir karakter dizisi tanimlayin. */
	char buffer[BUF_SIZE];
	/* TODO: read()'in donus degerini tutacak bir num_read degiskeni tanimlayin. */
	ssize_t num_read;
	/* Komut satirindan parametre gecilmemis ise veya --help gecildiyse programi sonlandir. */
	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(1);
	}

	/* TODO: Boru hattini yaratin. Basarisiz olursa program 1 degeri ile sonlansin. */
	if (pipe(pipe_fd)) {
		perror("Pipe couldn't be created!");
		return 1;
	}

	/* Yeni bir surec yaratmak icin fork'layin. */
	switch (pid = fork()) {
	case -1:
		/* TODO: fork basarisiz oldu, 1 degeriyle cikin. Cikmadan once
		 * acilmis olan boru hatti uclarini kapatin. */
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return 1;
	case 0:
		/* Cocuk surec boru hattinin okuyucusu olacak. */
		/* TODO: Boru hattinin cocuk tarafindan kullanilmayacak yazma
		 * ucunu kapatin. */
		if (close(pipe_fd[1]) == -1)
			perror("child couldnt close pipe");
		/* TODO: Surec oncelikle ekrana kendi PID'ini sonuna '\n' koymadan yazsin.
		 * Burada printf kullanabilirsiniz. */
		printf("%d", getpid());
		while (1) {
			/* TODO: Boru hattindan BUF_SIZE bayt bloklar halinde okuma
			 * yapin. */
			num_read = read(pipe_fd[0], buffer, BUF_SIZE);
			/* TODO: Okuma hatasi olduysa surec sonlansin. Acik boru hatti
			 * ucunu da kapatmayi unutmayin. */
			if (num_read == -1) {
				perror("Read Error in Child");
				close(pipe_fd[0]);
			}

			/* TODO: read()'in donus degerini degerlendirerek borunun
			 * yazma ucunun kapatildigi ya da daha fazla yazilacak bir sey
			 * olmadigi sonucuna variliyorsa ona gore donguden cikin. */
			if (num_read == 0 || pipe_fd[0] == 0) {
				break;
			}

			/* TODO: Borudan okunup tampona atilan veriyi BUYUK HARFE
			 * CEVIRIN. Bunun icin tamponu karakter karakter gezip
			 * alfabetik ve kucuk karakterse buyuk harfe cevirmeniz
			 * gerekmektedir. Daha sonra tamponu ekrana yani
			 * stdout akisina write() ile yazin.
			 *
			 * Bu esnada write()'in donus degerini de kontrol edin, hata olursa sureci 1
			 * degeriyle sonlandirin. */
			int i;
			for (i = 0; i < BUF_SIZE; i++) {
				if (buffer[i] < 123 && buffer[i] > 96)
					buffer[i] = buffer[i] - ' ';

			}
			if (write(STDOUT_FILENO, buffer, num_read) != num_read) {
				perror("child write failure stdout");
			}
		}

		/* TODO: write() ile stdout akisina yeni-satir (\n) karakteri yazin. */
		write(STDOUT_FILENO, "\n", 1);
		/* TODO: Son olarak borunun okuma ucunu kapatin. */
		if (close(pipe_fd[0] == -1)) {
			perror("child okuma ucunu kapatamadi");
		}

		/* Her sey basariyla bu noktaya geldiyse cocuk surec 0 ile cikarak
		 * basarili bir sekilde bittigini bildiriyor. */
		exit(0);

	default:
		/* Ebeveyn surec boruya yazacak */

		/* TODO: Boru hattinin ebeveyn tarafindan kullanilmayacak okuma
		 * ucunu kapatin. */
		if (close(pipe_fd[0]) == -1) {
			perror("parent diger pipe ucunu kapatamadi");
		}

		/* TODO: Komut satiri argumanini boru hattina yazin. Yazmaniz gereken veri miktari
		 * write() tarafindan yazilmis olandan farkliysa hata olmus
		 * demektir, ona gore programi 1 ile sonlandirin.
		 */
		if (write(pipe_fd[1], argv[1], strlen(argv[1])) != strlen(argv[1])) {
			perror("parent pipe write fault");
		}

		/* TODO: Yazma ucunu kapatin */
		if (close(pipe_fd[1]) == -1) {
			perror("error closing write pipe");
		}

		/* TODO: Cocugun bitisini bekleyin (bkz ilk odev) Cocuk 0'dan
		 * farkli bir degerle sonlandiysa ekrana bunu belirten bir mesaj
		 * yazdirin. */
		int status;
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) != 0) {
			printf("child process 0 ile sonlanmadi");
		}
		/* Artik ebeveyn surec basariyla sonlanabilir */
		exit(0);
	}
}
