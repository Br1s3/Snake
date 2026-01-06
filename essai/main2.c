#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>
#include <termios.h>

int main()
{
    char tab = 'c';

    struct termios ancien, nouveau;
    tcgetattr(STDIN_FILENO, &ancien);       // sauvegarde des paramètres du terminal
    nouveau = ancien;
    nouveau.c_lflag &= ~(ICANON | ECHO); // ICANON pour mode désactiver le mode canonique et ECHO pour ne pas afficher le carractère tapé (logique inverse)
    tcsetattr(STDIN_FILENO, TCSANOW, &nouveau);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    
    for (int i = 0; i < 100; i++) {

	usleep(200000);
	fscanf(stdin, "%c", &tab);
	
	printf("%d: PHRASE: %c\n", i, tab);
	fflush(stdin);
	if (tab == 's') break;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &ancien);
    return 0;
}
