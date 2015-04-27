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
	int child_pid;
	ssize_t num_written;
	int status;

	/* TODO: BUF_SIZE boyutlu bir karakter dizisi tanimlayin. */
	char s[BUF_SIZE];
	char ns[BUF_SIZE];

	/* TODO: read()'in donus degerini tutacak bir num_read degiskeni tanimlayin. */
	ssize_t num_read;

	/* Komut satirindan parametre gecilmemis ise veya --help gecildiyse programi sonlandir. */
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
		close(pipe_fd[1]);

	case 0:
		/* Cocuk surec boru hattinin okuyucusu olacak. */
		//read(pipe_fd[0], s, BUF_SIZE);
		/* TODO: Boru hattinin cocuk tarafindan kullanilmayacak yazma
		 * ucunu kapatin. */
		close(pipe_fd[1]);

		/* TODO: Surec oncelikle ekrana kendi PID'ini sonuna '\n' koymadan yazsin.
		 * Burada printf kullanabilirsiniz. */
		printf("%d", getpid());

		while (1) {
			/* TODO: Boru hattindan BUF_SIZE bayt bloklar halinde okuma
			 * yapin. */
			num_read = read(pipe_fd[0], s, BUF_SIZE);
			/* TODO: Okuma hatasi olduysa surec sonlansin. Acik boru hatti
			 * ucunu da kapatmayi unutmayin. */
			if (num_read == -1) {
				close(pipe_fd[0]);
				exit(0);
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
			for (int i = 0; i < BUF_SIZE; i++) {
				if ((s[i] > 96) && (s[i] < 123))
					ns[i] = s[i] - 'a' + 'A';
				else
					ns[i] = s[i];
			}
			num_written = write(STDOUT_FILENO, ns, BUF_SIZE);
			if (num_written == -1) {
				printf("yazamadi");
				exit(1);
			}
		}

		/* TODO: write() ile stdout akisina yeni-satir (\n) karakteri yazin. */
		num_written = write(STDOUT_FILENO, "\n", BUF_SIZE);
		/* TODO: Son olarak borunun okuma ucunu kapatin. */
		if (close(pipe_fd[0]) == -1) {
			printf("Okuma ucu kapanmadi");
			exit(1);
		}

		/* Her sey basariyla bu noktaya geldiyse cocuk surec 0 ile cikarak
		 * basarili bir sekilde bittigini bildiriyor. */
		exit(0);

	default:
		/* Ebeveyn surec boruya yazacak */
		//write(STDOUT_FILENO, “hello world!\n”, strlen(“hello world!\n”));
		/* TODO: Boru hattinin ebeveyn tarafindan kullanilmayacak okuma
		 * ucunu kapatin. */
		if (close(pipe_fd[0]) == -1) {
			exit(1);
		}

		/* TODO: Komut satiri argumanini boru hattina yazin. Yazmaniz gereken veri miktari
		 * write() tarafindan yazilmis olandan farkliysa hata olmus
		 * demektir, ona gore programi 1 ile sonlandirin.
		 */
		//char buf[BUF_SIZE] = argv[1]
		num_written = write(pipe_fd[1], argv[1], strlen(argv[1]));
		//printf("%s", argv[1]);
		if (num_written != BUF_SIZE) {
			exit(1);
		}

		/* TODO: Yazma ucunu kapatin */
		if (close(pipe_fd[1]) == -1) {
			exit(1);
		}

		/* TODO: Cocugun bitisini bekleyin (bkz ilk odev) Cocuk 0'dan
		 * farkli bir degerle sonlandiysa ekrana bunu belirten bir mesaj
		 * yazdirin. */
		waitpid(child_pid, &status, 0);
		if (WIFEXITED(status)) {
			printf("%d", WEXITSTATUS(status));
		}
		/* Artik ebeveyn surec basariyla sonlanabilir */
		exit(0);
	}
}
