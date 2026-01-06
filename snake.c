#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <linux/input.h>
#include <time.h>
#include <string.h>

#include "graphique.h"

#define dH 20
#define dL 10

#define HEIGHT (600/dH)
#define WIDTH (800/dL)

#define longueur_serpent_max  ((HEIGHT-2)*(WIDTH-2))
COORD Serp[longueur_serpent_max];

// const char *chemin_evt = "/dev/input/event3";

void wait(int n)
{
    for (int i = 0; i < n; i++) {
	usleep(1000);	// ms
    }
}

void construction_plateau(char **P, int width, int height)
{
    cons_clear(P, width, height, '#');
    cons_rect(P, width, height, -width/2+1, -height/2+1, width-2, height-2, ' ');
}

int saisie(char *ret)
{
    char buf = *ret;
    scanf("%c", &buf);
    if (buf == 'v')
    	return 1;
    	// return 5;
    else if (!(buf == 'z' || buf == 's' || buf == 'q' || buf == 'd')) {
    	moveto(HEIGHT+1, 0);
    	printf("Erreur de saisie");
    	return 300;
    }
    else {
	*ret = buf;
    	moveto(HEIGHT+1, 0);
    	puts("                                            ");
    	if (buf == 'd' || buf == 'q')
    	    // return  500;
	    return  150;
    	else
    	    // return 500;
	    return 300;
    }
    return 1000;
}



COORD set_pomme(char **p, int width, int height)
{
    COORD pom;

    do
    {
    	pom.x = rand()%(width-2)+1;
    	pom.y = rand()%(height-2)+1;
    } while (p[pom.y][pom.x] != ' '); // probleme de lecture de la bonne pom.x pom.y
    pom.x -= width/2;
    pom.y -= height/2;
    cons_rect(p, width, height, pom.x, pom.y, 1, 1, '@');
    
    return pom;
}

COORD spawn_serpent(char **p, int width, int height)
{
    COORD S;

    S.x = rand()%(((width-2)+1)/2); // moitier droite du plateau
    S.y = rand()%(height-2)+1;

    S.y -= height/2;

    cons_rect(p, width, height, S.x, S.y, 1, 1, 'P');
    return S;
}


int main()
{
    struct termios canonique, non_canonique;
    tcgetattr(STDIN_FILENO, &canonique);
    non_canonique = canonique;
    non_canonique.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &non_canonique);

    // int fd = open(chemin_evt, O_RDONLY);
    // if (fd < 0) {
    // 	fprintf(stderr, "Prblème ouverture fichier, le sudo est requis\n");
    // 	exit(EXIT_FAILURE);
    // }
    // fcntl(fd, F_SETFL, O_NONBLOCK);
    
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    
    char **console = mem_alloc(HEIGHT, WIDTH);
    if (console == NULL) {
	fprintf(stderr, "Problème d'allocation %d\n", __LINE__);
	exit(EXIT_FAILURE);
    }
    
    construction_plateau(console, WIDTH, HEIGHT);
    COORD pomme = set_pomme(console, WIDTH, HEIGHT);

    char direction = 'q';
    char direction_buf = direction;
    int longueur_serpent = 5;
    // int longueur_serpent = (HEIGHT-2)*(WIDTH-2);
    int speed;

        
    srand(time(NULL));
    Serp[0] = spawn_serpent(console, WIDTH, HEIGHT);
    for (int j = 0; j < longueur_serpent; j++) {
	Serp[j+1].x = Serp[j].x;
	Serp[j+1].y = Serp[j].y;
    }

    for (;;)
    {
	if (longueur_serpent == longueur_serpent_max) {
	    moveto(HEIGHT+1, 0);
	    puts("GAGNE !!!");
	    break;
	}
	
	for (int j = longueur_serpent; j > 0; j--) {
	    Serp[j].x = Serp[j-1].x;
	    Serp[j].y = Serp[j-1].y;
	}
	
	speed = saisie(&direction);
	direction_buf = direction;

	switch (direction_buf)
	{
	case 'z' :
	    Serp[0].y--;
	    break;
	case 's' :
	    Serp[0].y++;
	    break;
	case 'q' :
	    Serp[0].x--;
	    break;
	case 'd' :
	    Serp[0].x++;
	    break;
	default:
	    moveto(HEIGHT+1, 0);
	    printf("Gros problème");
	    break;
	}
	
	if (console[Serp[0].y+HEIGHT/2][Serp[0].x+WIDTH/2] == '#' || console[Serp[0].y+HEIGHT/2][Serp[0].x+WIDTH/2] == 'o') {
	    moveto(HEIGHT+1, 0);
	    puts("PERDU !!!");
	    break;
	}
	
	if (Serp[0].x == pomme.x && Serp[0].y == pomme.y) {
	    longueur_serpent++;
	    pomme = set_pomme(console, WIDTH, HEIGHT);
	}
	
	for (int j = 0; j < longueur_serpent+1; j++) {
	    if (j == longueur_serpent)
		cons_rect(console, WIDTH, HEIGHT, Serp[j].x, Serp[j].y, 1, 1, ' ');
	    else if (j == 0)
		cons_rect(console, WIDTH, HEIGHT, Serp[j].x, Serp[j].y, 1, 1, 'Q');
	    else
		cons_rect(console, WIDTH, HEIGHT, Serp[j].x, Serp[j].y, 1, 1, 'o');
	}
	
	// int test = 0;
	// for (int j = 0; j < HEIGHT; j++) {
	//     for (int k = 0; k < WIDTH; k++) {
	// 	if (console[j][k] == '@') test++;
	//     }
	// }
	// if (test == 0) {puts("PROBLEME de fin de queu");break;}
	
	print_cons(console, WIDTH, HEIGHT);
	
	wait(speed);
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &canonique);
    mem_free(console, HEIGHT);
    return 0;
}
