#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 16

int main(int argc, char *argv[])
{
	ssize_t num_written;
	int i, child_pid, status;
	char upper[BUF_SIZE];

	/* TODO: Boru hatti dosya tanimlayicilari icin bir dizi bildirimi yapin. */
	int pipe_fd[2];

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
		exit(1);

	case 0:
		/* Cocuk surec boru hattinin okuyucusu olacak. */

		/* TODO: Boru hattinin cocuk tarafindan kullanilmayacak yazma
		 * ucunu kapatin. */
		if (close(pipe_fd[1]) == -1) {
			perror("close");
		}

		/* TODO: Surec oncelikle ekrana kendi PID'ini sonuna '\n' koymadan yazsin.
		 * Burada printf kullanabilirsiniz. */
		child_pid = getpid();
//          printf("I am the child with PID=%d\n",getpid());

		while (1) {
			/* TODO: Boru hattindan BUF_SIZE bayt bloklar halinde okuma
			 * yapin. */
			num_read = read(pipe_fd[0], buf, BUF_SIZE);
//printf("buf dan okudum = %s\n",buf);
//printf("ilk harf =%c\n",buf[0]);
//printf("num_read degeri = %zd",num_read);

			/* TODO: Okuma hatasi olduysa surec sonlansin. Acik boru hatti
			 * ucunu da kapatmayi unutmayin. */
			if (num_read == -1) {
				close(pipe_fd[0]);
				exit(-1);
			}

			/* TODO: read()'in donus degerini degerlendirerek borunun
			 * yazma ucunun kapatildigi ya da daha fazla yazilacak bir sey
			 * olmadigi sonucuna variliyorsa ona gore donguden cikin. */

//If the current file offset is at or past the end of file, no bytes are read, and read() returns zero.
			if (num_read == 0 || close(pipe_fd[1]) == 0) {
				exit(0);
			}

			/* TODO: Borudan okunup tampona atilan veriyi BUYUK HARFE
			 * CEVIRIN. Bunun icin tamponu karakter karakter gezip
			 * alfabetik ve kucuk karakterse buyuk harfe cevirmeniz
			 * gerekmektedir. Daha sonra tamponu ekrana yani
			 * stdout akisina write() ile yazin.
			 *
			 * Bu esnada write()'in donus degerini de kontrol edin, hata olursa sureci 1
			 * degeriyle sonlandirin. */

			for (i = 0; i < num_read; i++) {
				if (buf[i] >= 'a' && buf[i] <= 'z') {
					upper[i] = 'A' + buf[i] - 'a';
					//printf("uppera cevirme ifine girdi.");
				} else {
					upper[i] = buf[i];
				}
			}

			num_written = write(STDOUT_FILENO, upper, num_read);
			if (num_written < 0) {
				perror("write");
				exit(1);
			}
		}

		/* TODO: write() ile stdout akisina yeni-satir (\n) karakteri yazin. */
		write(STDOUT_FILENO, "\n", 1);

		/* TODO: Son olarak borunun okuma ucunu kapatin. */
		if (close(pipe_fd[0]) == -1) {
			perror("close");
		}

		/* Her sey basariyla bu noktaya geldiyse cocuk surec 0 ile cikarak
		 * basarili bir sekilde bittigini bildiriyor. */
		exit(0);

	default:
		/* Ebeveyn surec boruya yazacak */

		/* TODO: Boru hattinin ebeveyn tarafindan kullanilmayacak okuma
		 * ucunu kapatin. */
		if (close(pipe_fd[0]) == -1) {
			perror("close");
		}

		/* TODO: Komut satiri argumanini boru hattina yazin. Yazmaniz gereken veri miktari
		 * write() tarafindan yazilmis olandan farkliysa hata olmus
		 * demektir, ona gore programi 1 ile sonlandirin.
		 */

		num_written = write(pipe_fd[1], argv[1], (strlen(argv[1]) + 1));
//printf("%s\n",pipe_fd[1]);

		if (num_written != strlen(argv[1]) + 1) {
			exit(1);
		}
		/* TODO: Yazma ucunu kapatin */
		if (close(pipe_fd[1]) == -1) {
			perror("close");
		}

		/* TODO: Cocugun bitisini bekleyin (bkz ilk odev) Cocuk 0'dan
		 * farkli bir degerle sonlandiysa ekrana bunu belirten bir mesaj
		 * yazdirin. */
		waitpid(child_pid, &status, 0);
		if (WIFEXITED(status)) {
			printf("exit değeri = %d\n", WEXITSTATUS(status));
		} else if (WIFSIGNALED(status)) {
			printf("abnormal termination!!, signal number= %d\n", WTERMSIG(status));
		}

		/* Artik ebeveyn surec basariyla sonlanabilir */
		exit(0);
	}
}
