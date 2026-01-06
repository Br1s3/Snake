#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <linux/input.h>

const char *chemin_evt = "/dev/input/event3";

void wait(int n)
{
    for (int i = 0; i < n; i++) {
	usleep(1000);	// ms
    }
}


int main()
{
    struct input_event ev;
    // struct termios term_sauv, term_non_echo;

    int fd = open(chemin_evt, O_RDONLY);
    if (fd < 0) {
	fprintf(stderr, "Necessite les droits admin\n");
	exit(-1);
    }
    
    // if (tcgetattr(STDIN_FILENO, &term_sauv) < 0) {
    // 	fprintf(stderr, "Problème tc get attr\n");
    // 	exit(-1);
    // }
    // term_sauv = term_non_echo;
    // term_non_echo.c_lflag &= ~(ICANON | ECHO);
    // // term_non_echo.c_lflag &= ~(ECHO);

    // if (tcsetattr(STDIN_FILENO, TCSANOW, &term_non_echo) < 0) {
    // 	fprintf(stderr, "Problème tc set attr\n");
    // 	exit(-1);
    // }
    
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
    	fprintf(stderr, "Problème fcntl\n");
    	exit(-1);	
    }

    for (int i = 0; i < 100; i++) {
	read(fd, &ev, sizeof(ev));
	wait(100);
	// if (n != sizeof(ev)) {
	//     fprintf(stderr, "Problème taille du read\n");
	//     break;
	// }

	// if (ev.type == EV_KEY) {
	printf("Key %d %s => %d\n", ev.code, ev.value ? (ev.value == 2 ? "held" : "pressed") : "released", i);
	// }
	    fflush(stdout);
	
	if (ev.code == KEY_ESC && ev.value == 1) {
	    printf("→ Sortie du programme.\n");
	    fflush(stdout);
	    break;
	}
    }

    // tcsetattr(STDIN_FILENO, TCSANOW, &term_sauv);
    close(fd);
    return 0;
}
