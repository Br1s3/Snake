#!/bin/bash

set -xe

gcc snake.c graphique.c -o snake -Wextra -Wall -g3

gcc snake_cap.c graphique.c -o snake_cap -Wextra -Wall -g3

gcc essai/main2.c -o essai/main2 -Wextra -Wall

gcc essai/main3.c -o essai/main3 -Wextra -Wall

gcc essai/main4.c -o essai/main4 -Wextra -Wall

# ./main4
