/*Sumeyye KONAK*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 16

int main(int argc, char *argv[])
{
	/* TODO: Boru hatti dosya tanimlayicilari icin bir dizi bildirimi yapin. */
	int filedes[2];

	/* TODO: BUF_SIZE boyutlu bir karakter dizisi tanimlayin. */
	char buf[BUF_SIZE];

	/* TODO: read()'in donus degerini tutacak bir num_read degiskeni tanimlayin. */
	ssize_t numread;

	ssize_t numwrite;
	int status;
	int i;
	char bcumle[BUF_SIZE];

	/* Komut satirindan parametre gecilmemis ise veya --help gecildiyse programi sonlandir. */
	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(1);
	}

	/* TODO: Boru hattini yaratin. Basarisiz olursa program 1 degeri ile sonlansin. */
	if (pipe(filedes) < 0) {
		exit(1);
	}

	/* Yeni bir surec yaratmak icin fork'layin. */
	switch (fork()) {
	case -1:
		/* TODO: fork basarisiz oldu, 1 degeriyle cikin. Cikmadan once
		 * acilmis olan boru hatti uclarini kapatin. */
		close(filedes[0]);	//Boru hattinin okuma ucunu kapama
		close(filedes[1]);	//Boru hattinin yazma ucunu kapama
		exit(1);	//1 degeriyle cikma

	case 0:
		/* Cocuk surec boru hattinin okuyucusu olacak. */

		/* TODO: Boru hattinin cocuk tarafindan kullanilmayacak yazma
		 * ucunu kapatin. */
		close(filedes[1]);	//Yazma ucunu kapama

		/* TODO: Surec oncelikle ekrana kendi PID'ini sonuna '\n' koymadan yazsin.
		 * Burada printf kullanabilirsiniz. */
		printf("%d", getpid());	//Cocuk surecin pid'isini ekrana yazma

		while (1) {
			/* TODO: Boru hattindan BUF_SIZE bayt bloklar halinde okuma
			 * yapin. */
			numread = read(filedes[0], buf, BUF_SIZE);

			/* TODO: Okuma hatasi olduysa surec sonlansin. Acik boru hatti
			 * ucunu da kapatmayi unutmayin. */
			if (numread < 0) {
				close(filedes[0]);	//Acik okuma ucunu kapama
				exit(1);
			}

			/* TODO: read()'in donus degerini degerlendirerek borunun
			 * yazma ucunun kapatildigi ya da daha fazla yazilacak bir sey
			 * olmadigi sonucuna variliyorsa ona gore donguden cikin. */
			if (numread == 0) {
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

			for (i = 0; buf[i] != '\0'; i++) {
				bcumle[i] = toupper(buf[i]);
			}
			printf("\n");

			numwrite = write(STDOUT_FILENO, bcumle, strlen(bcumle));
			if (numwrite < 0) {
				exit(1);
			}
		}

		/* TODO: write() ile stdout akisina yeni-satir (\n) karakteri yazin. */
		numwrite = write(STDOUT_FILENO, "\n", strlen("\n"));

		/* TODO: Son olarak borunun okuma ucunu kapatin. */
		if (close(filedes[0]) < 0) {
			fprintf(stderr, "Hata 1!\n");
		}

		/* Her sey basariyla bu noktaya geldiyse cocuk surec 0 ile cikarak
		 * basarili bir sekilde bittigini bildiriyor. */
		exit(0);

	default:
		/* Ebeveyn surec boruya yazacak */

		/* TODO: Boru hattinin ebeveyn tarafindan kullanilmayacak okuma
		 * ucunu kapatin. */
		if (close(filedes[0]) < 0) {
			fprintf(stderr, "Hata 2!\n");
		}

		/* TODO: Komut satiri argumanini boru hattina yazin. Yazmaniz gereken veri miktari
		 * write() tarafindan yazilmis olandan farkliysa hata olmus
		 * demektir, ona gore programi 1 ile sonlandirin.
		 */
		numwrite = write(filedes[1], buf, numwrite);
		if (numwrite != numread) {
			fprintf(stderr, "Hata 3!\n");
			exit(1);
		}

		/* TODO: Yazma ucunu kapatin */
		if (close(filedes[1]) < 0) {
			fprintf(stderr, "Hata 4!\n");
		}

		/* TODO: Cocugun bitisini bekleyin (bkz ilk odev) Cocuk 0'dan
		 * farkli bir degerle sonlandiysa ekrana bunu belirten bir mesaj
		 * yazdirin. */
		waitpid(-1, &status, 0);

		do {
			if (WIFEXITED(status) != 0) {	//cocuk surec normal yolla sonlandıysa, WIFEXITED 0'dan farkli deger dondurur
				printf("Exited, status=%d\n", WEXITSTATUS(status));	//Cocuk surecin cikis degerini bildirme
			} else if (WIFSIGNALED(status) != 0) {	//cocuk surec  bir sinyal yuzunden sonlandiysa, 0'dan farkli deger dondurur
				printf("Killed by signal %d\n", WTERMSIG(status));	//hangi sinyal yuzunden sonlandigini bildirme
			}

		} while ((WIFEXITED(status) != 0) && (WIFSIGNALED(status) != 0));

		exit(EXIT_SUCCESS);

		/* Artik ebeveyn surec basariyla sonlanabilir */
		exit(0);
	}
}
