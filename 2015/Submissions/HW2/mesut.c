#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 16

int main(int argc, char *argv[])
{
	/* TODO:Boru hatti dosya tanimlayicilari icin bir dizi bildirimi yapin. */

	int pipe_fd[2];

	/* TODO: BUF_SIZE boyutlu bir karakter dizisi tanimlayin. */

	char buffer[BUF_SIZE];

	/* TODO: read()'in donus degerini tutacak bir num_read degiskeni tanimlayin. */

	int num_read;

	/* Komut satirindan parametre gecilmemis ise veya --help gecildiyse programi sonlandir. */
	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(1);
	}

	/* TODO: Boru hattini yaratin. Basarisiz olursa program 1 degeri ile sonlansin. */

	if (pipe(pipe_fd) == -1) {

		exit(1);
	}
	/* Yeni bir surec yaratmak icin fork'layin. */

	switch (fork()) {
	case -1:
		/* TODO: fork basarisiz oldu, 1 degeriyle cikin. Cikmadan once
		 * acilmis olan boru hatti uclarini kapatin. */

		fprintf(stderr, "Fork yaparken hata olustu. ");
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		exit(1);

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
			num_read = read(pipe_fd[0], buffer, BUF_SIZE);

			/* TODO: Okuma hatasi olduysa surec sonlansin. Acik boru hatti
			 * ucunu da kapatmayi unutmayin. */

			if (num_read == -1) {
				close(pipe_fd[0]);
				exit(1);

			}

			/* TODO: read()'in donus degerini degerlendirerek borunun
			 * yazma ucunun kapatildigi ya da daha fazla yazilacak bir sey
			 * olmadigi sonucuna variliyorsa ona gore donguden cikin. */
			if (num_read == 0 || close(pipe_fd[1]) == 0) {

				exit(1);

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
			for (i = 0; buffer[i] != '\0'; i++) {
				buffer[i] = toupper(buffer[i]);
				write(STDOUT_FILENO, buffer, num_read);
			}

			if (write(STDOUT_FILENO, buffer, num_read) != num_read) {
				fprintf(stderr, "Can't write whole buffer!\n");
				exit(1);
			}
		}

		/* TODO: write() ile stdout akisina yeni-satir (\n) karakteri yazin. */

		write(STDOUT_FILENO, "\n", strlen("\n"));

		/* TODO: Son olarak borunun okuma ucunu kapatin. */
		if (close(pipe_fd[0]) == -1) {

			fprintf(stderr, "Okuma ucu kapatma hatası oluştu!\n");

		}

		/* Her sey basariyla bu noktaya geldiyse cocuk surec 0 ile cikarak
		 * basarili bir sekilde bittigini bildiriyor. */
		exit(0);

	default:
		/* Ebeveyn surec boruya yazacak */

		/* TODO: Boru hattinin ebeveyn tarafindan kullanilmayacak okuma
		 * ucunu kapatin. */
		if (close(pipe_fd[0]) == -1) {

			fprintf(stderr, "Okuma ucu kapatma hatası oluştu!\n");

		}

		/* TODO: Komut satiri argumanini boru hattina yazin. Yazmaniz gereken veri miktari
		 * write() tarafindan yazilmis olandan farkliysa hata olmus
		 * demektir, ona gore programi 1 ile sonlandirin.
		 */

		if (write(pipe_fd[1], buffer, num_read) != num_read) {
			fprintf(stderr, "Yazma hatası oluştu anam! :)\n");
			exit(1);
		}

		/* TODO: Yazma ucunu kapatin */
		if (close(pipe_fd[1]) == -1) {

			fprintf(stderr, "Yazma ucu kapatma hatası oluştu!\n");

		}
		/* TODO: Cocugun bitisini bekleyin (bkz ilk odev) Cocuk 0'dan
		 * farkli bir degerle sonlandiysa ekrana bunu belirten bir mesaj
		 * yazdirin. */

		int returnStatus;
		waitpid(-1, &returnStatus, 0);	// Parent process waits here for child to terminate.

		if (returnStatus != 0)	// Verify child process terminated without error.  

		{
			fprintf(stderr, " Çocuk süreç O'dan farklı bir değer ile son buldu.\n");

		}

		exit(0);

		/* Artik ebeveyn surec basariyla sonlanabilir */

	}
}
