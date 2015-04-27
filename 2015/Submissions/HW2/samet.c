#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <ctype.h>
#define BUF_SIZE 16

int main(int argc, char *argv[])
{
	int cstatus;		/* cocuğun çıkış durumu */
	pid_t c;		/* beklenen çocuğun pid si */
	pid_t child;
	int fdps[2];		//pipe için bir dizi
	/* TODO: Boru hatti dosya tanimlayicilari icin bir dizi bildirimi yapin. */
	char buf[BUF_SIZE];
	char new[BUF_SIZE + 1];
	/* TODO: BUF_SIZE boyutlu bir karakter dizisi tanimlayin. */
	ssize_t num_read;
	ssize_t n;
	/* TODO: read()'in donus degerini tutacak bir num_read degiskeni tanimlayin. */

	/* Komut satirindan parametre gecilmemis ise veya --help gecildiyse programisonlandir. */
	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(1);
	}

	/* TODO: Boru hattini yaratin. Basarisiz olursa program 1 degeri ile sonlansin. */
	if (pipe(fdps) < 0) {

		fprintf(stderr, "Boruda hata oluştu.\n");
		exit(1);
	}

	/* Yeni bir surec yaratmak icin fork'layin. */
	switch (child = fork()) {
	case -1:
		fprintf(stderr, "Alt süreç oluşturulamadı.\n");
		close(fdps[0]);
		close(fdps[1]);
		exit(1);
		/* TODO: fork basarisiz oldu, 1 degeriyle cikin. Cikmadan once
		 * acilmis olan boru hatti uclarini kapatin. */
	case 0:

		/* Cocuk surec boru hattinin okuyucusu olacak. */
		/* TODO: Boru hattinin cocuk tarafindan kullanilmayacak yazma
		 * ucunu kapatin. */
		close(fdps[1]);
		/* TODO: Surec oncelikle ekrana kendi PID'ini sonuna '\n' koymadan
		   yazsin  * Burada printf kullanabilirsiniz. */
		printf("PID: %d", getpid());

		/* TODO: Boru hattindan BUF_SIZE bayt bloklar halinde okuma
		 * yapin. */
		while ((num_read = read(fdps[0], buf, BUF_SIZE)) > 0)
			if (write(STDOUT_FILENO, buf, num_read) != num_read)
				return 0;

		/* TODO: Okuma hatasi olduysa surec sonlansin. Acik boru hatti
		 * ucunu da kapatmayi unutmayin. */
		if (num_read < 0) {
			close(fdps[0]);

			exit(1);
		}

		/*
		   TODO: read()'in donus degerini degerlendirerek borunun
		   yazma ucunun kapatildigi ya da daha fazla yazilacak bir sey
		   olmadigi sonucuna variliyorsa ona gore donguden cikin. */

		if (num_read == 0) {

			return 1;
		}

		/* TODO: Borudan okunup tampona atilan veriyi BUYUK HARFE
		 * CEVIRIN. Bunun icin tamponu karakter karakter gezip*
		 * alfabetik ve kucuk karakterse buyuk harfe cevirmeniz
		 gerekmektedir. Daha sonra tamponu ekrana yani
		 stdout akisina write() ile yazin.
		 Bu esnada write()'in donus degerini de kontrol edin, hata olursa
		 sureci 1 değeriyle sonlandırın .*/
		int i;

		for (i = 0; buf[i] != '\0'; i++) {
			if (buf[i] == ' ') {
				new[i] = buf[i];
			}
			new[i] = toupper(buf[i]);
		}

		new[BUF_SIZE] = '\n';
		if (write(1, new, num_read) != num_read)
			exit(1);

		/* TODO: write() ile stdout akisina yeni-satir (\n) karakteri yazin. */

		/* TODO: Son olarak borunun okuma ucunu kapatin. */
		close(fdps[0]);
		/* Her sey basariyla bu noktaya geldiyse cocuk surec 0 ile cikarak
		 * basarili bir sekilde bittigini bildiriyor. */
		exit(0);

	default:
		/* Ebeveyn surec boruya yazacak */
		/* TODO: Boru hattinin ebeveyn tarafindan kullanilmayacak okuma
		 * ucunu kapatin. */

		close(fdps[0]);

		while ((num_read = read(fdps[1], buf, BUF_SIZE)) > 0)
			if (write(fdps[1], buf, num_read) < 0)
				exit(1);
		if (num_read < 0)
			return 1;

		if ((n = (write(fdps[1], argv[1], strlen(argv[1])))) != num_read) {
			exit(1);
		}
/* TODO: Komut satiri argumanini boru hattina yazin. Yazmaniz gereken
veri miktari
*
* write() tarafindan yazilmis olandan farkliysa hata olmus
* demektir, ona gore programi 1 ile sonlandirin.
*/

		if (1) {
			close(fdps[1]);
		}
/* TODO: Yazma ucunu kapatin */
		if (1) {
		}
/* TODO: Cocugun bitisini bekleyin (bkz ilk odev) Cocuk 0'dan
* farkli bir degerle sonlandiysa ekrana bunu belirten bir mesaj
* yazdirin. */
		c = waitpid(child, &cstatus, 0);	/* çocuk sürecin tamamlanmasını bekle . */
		if (WIFEXITED(cstatus)) {
			printf("Parent: Child %d exited with status = %d\n", c, WEXITSTATUS(cstatus));
		}

/* Artik ebeveyn surec basariyla sonlanabilir*/
		exit(0);
	}
}
