#!/bin/bash

set -xe

gcc snake.c graphique.c -o snake -Wextra -Wall -g3

gcc snake_cap.c graphique.c -o snake_cap -Wextra -Wall -g3

gcc main2.c -o main2 -Wextra -Wall

gcc main3.c -o main3 -Wextra -Wall

gcc main4.c -o main4 -Wextra -Wall

# ./main4
