#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    int haut = 0;
    int val = 0;
    for (int i = 0; i < 1000; i++) {
	val = rand()%(1080);
	// printf("%f\n", (float)rand()/(float)RAND_MAX);
	if (haut < val)
	    haut = val;
	// printf("%d\n", i);
    }
    printf("val max %d\n", haut);

    // for (int j = 10; j > 0; j--) {
    // 	printf("%d, %d\n", j, j-1);
    // }

    // for (int j = 0; j < 10; j++) {
    // 	printf("%d\n", j);
    // }

    return 0;
}
