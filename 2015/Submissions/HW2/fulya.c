#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 16

int main(int argc, char *argv[])
{
	int status;
	/* TODO: Boru hatti dosya tanimlayicilari icin bir dizi bildirimi yapin. */
	int pipefd[2];
	/* TODO: BUF_SIZE boyutlu bir karakter dizisi tanimlayin. */
	char buf[BUF_SIZE];
	/* TODO: read()'in donus degerini tutacak bir num_read degiskeni tanimlayin. */
	ssize_t num_read;

	/* Komut satirindan parametre gecilmemis ise veya --help gecildiyse programi sonlandir. */
	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(1);
	}

	/* TODO: Boru hattini yaratin. Basarisiz olursa program 1 degeri ile sonlansin. */
	if (pipe(pipefd) == -1) {
		exit(1);
	}

	/* Yeni bir surec yaratmak icin fork'layin. */
	switch (fork()) {
	case -1:
		/* TODO: fork basarisiz oldu, 1 degeriyle cikin. Cikmadan once
		 * acilmis olan boru hatti uclarini kapatin. */
		printf("Fork failed");
		exit(1);
		close(pipefd[1]);
		close(pipefd[0]);
	case 0:
		/* Cocuk surec boru hattinin okuyucusu olacak. */

		/* TODO: Boru hattinin cocuk tarafindan kullanilmayacak yazma
		 * ucunu kapatin. */
		close(pipefd[1]);
		/* TODO: Surec oncelikle ekrana kendi PID'ini sonuna '\n' koymadan yazsin.
		 * Burada printf kullanabilirsiniz. */
		printf("I am a child process pid %d, and will read from pipe\n", getpid());

		while (1) {
			/* TODO: Boru hattindan BUF_SIZE bayt bloklar halinde okuma
			 * yapin. */
			num_read = read(pipefd[0], buf, BUF_SIZE);

			/* TODO: Okuma hatasi olduysa surec sonlansin. Acik boru hatti
			 * ucunu da kapatmayi unutmayin. */

			if (num_read == -1) {
				exit(1);
				close(pipefd[0]);
			}

			/* TODO: read()'in donus degerini degerlendirerek borunun
			 * yazma ucunun kapatildigi ya da daha fazla yazilacak bir sey
			 * olmadigi sonucuna variliyorsa ona gore donguden cikin. */
			if (num_read == 0) {
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
			char yenimesaj[100];

			for (i = 0; buf[i] != '\0'; i++) {
				yenimesaj[i] = toupper(buf[i]);
			}
			// write(STDOUT_FILENO, yenimesaj, strlen(yenimesaj)); 
			if (write(pipefd[1], yenimesaj, num_read) != num_read) {
				fprintf(stderr, "Can't write whole buffer!\n");
				exit(1);
			}
		}

		/* TODO: write() ile stdout akisina yeni-satir (\n) karakteri yazin. */
		write(STDOUT_FILENO, "\n", strlen("\n"));

		/* TODO: Son olarak borunun okuma ucunu kapatin. */
		if (close(pipefd[0]) < 0) {
			perror("close");
		}

		/* Her sey basariyla bu noktaya geldiyse cocuk surec 0 ile cikarak
		 * basarili bir sekilde bittigini bildiriyor. */
		exit(0);

	default:
		/* Ebeveyn surec boruya yazacak */

		/* TODO: Boru hattinin ebeveyn tarafindan kullanilmayacak okuma
		 * ucunu kapatin. */

		if (close(pipefd[0]) < 0) {
			perror("close");
		}

		/* TODO: Komut satiri argumanini boru hattina yazin. Yazmaniz gereken veri miktari
		 * write() tarafindan yazilmis olandan farkliysa hata olmus
		 * demektir, ona gore programi 1 ile sonlandirin.
		 */
		if (write(pipefd[1], buf, num_read) != num_read) {
			printf("hata");
			exit(1);

		}

		/* TODO: Yazma ucunu kapatin */
		if (close(pipefd[1]) < 0) {
			perror("close");
		}

		/* TODO: Cocugun bitisini bekleyin (bkz ilk odev) Cocuk 0'dan
		 * farkli bir degerle sonlandiysa ekrana bunu belirten bir mesaj
		 * yazdirin. */

		waitpid(-1, &status, 0);
		printf("Exit code: %d\n", WEXITSTATUS(status));

		/* Artik ebeveyn surec basariyla sonlanabilir */
		exit(0);
	}
}
