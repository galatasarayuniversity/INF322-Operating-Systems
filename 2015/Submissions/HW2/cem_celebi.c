#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#define BUF_SIZE 16

int main(int argc, char *argv[]) {
	int src_fd, dest_fd;

	/* TODO: Boru hatti dosya tanimlayicilari icin bir dizi bildirimi yapin. */
	int pipe_fd[2];
	/* TODO: BUF_SIZE boyutlu bir karakter dizisi tanimlayin. */
	char kar_dizi[BUF_SIZE];
	/* TODO: read()'in donus degerini tutacak bir num_read degiskeni tanimlayin. */
	int num_read;
	/* Komut satirindan parametre gecilmemis ise veya --help gecildiyse programi
	 sonlandir. */
	if (argc != 2 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(1);
	}

	/* TODO: Boru hattini yaratin. Basarisiz olursa program 1 degeri ile sonlansin.*/
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
		close(pipe_fd[1]);
	/* TODO: Surec oncelikle ekrana kbuendi PID'ini sonuna '\n' koymadan yazsin.
	 * Burada printf kullanabilirsiniz. */
		printf("%d", getpid());
	while (1) {
		/* TODO: Boru hattindan BUF_SIZE bayt bloklar halinde okuma
		 * yapin. */
		num_read = read(src_fd, kar_dizi, BUF_SIZE); // n byte okuma
		/* TODO: Okuma hatasi olduysa surec sonlansin. Acik boru hatti
		 * ucunu da kapatmayi unutmayin. */
		if (num_read ==-1) {
				perror("read");
				close(pipe_fd[0]);
				close(pipe_fd[1]);
		}
		/*
		 *
		 *

		 TODO: read()'in donus degerini degerlendirerek borunun
		 yazma ucunun kapatildigi ya da daha fazla yazilacak bir sey
		 olmadigi sonucuna variliyorsa ona gore donguden cikin. */
		 if(num_read==BUF_SIZE) {
			 // demek ki BUF_SIZE tane byte BASARIYLA okundu
			 exit(1);
			 break;

		 }

		/* TODO: Borudan okunup tampona atilan veriyi BUYUK HARFE
		 * CEVIRIN. Bunun icin tamponu karakter karakter gezip*
		 *
		 *
		 *
		 *
		 *
		 sureci 1
		 }*/

		 //BUYUK HARFE DONUSTURME::::
		 //A-->ASCII(32)  //to upper..??
		 int i=0;
		 for(i;kar_dizi[i];i++)
			 {
			 	 kar_dizi[i] -= 32;
			 }

		 /*
		 alfabetik ve kucuk karakterse buyuk harfe cevirmeniz
		 gerekmektedir. Daha sonra tamponu ekrana yani
		 stdout akisina write() ile yazin.
		 Bu esnada write()'in donus degerini de kontrol edin, hata olursa
		 * degeriyle sonlandirin. */
		int num_write=write(stdout, *kar_dizi, BUF_SIZE);
		if (num_write !=0){//n the absence of errors, or if
			//error detection is not performed, the write() function shall return zero
			exit(1);
		}

		/* TODO: write() ile stdout akisina yeni-satir (\n) karakteri yazin. */
		/* TODO: Son olarak borunun okuma ucunu kapatin. */
		if (num_write=0) {
			write(stdout, '\n', BUF_SIZE);
		}
		/* Her sey basariyla bu noktaya geldiyse cocuk surec 0 ile cikarak
		 * basarili bir sekilde bittigini bildiriyor. */
		exit(0);
	}
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

/* Artik ebeveyn surec basariyla sonlanabilir */
exit(0);
}
}
