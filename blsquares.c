#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BILLION 1000000000L

int found_solution;

void
create_board(int N, double S, int board[N][N]) {
    int i, j;
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            board[i][j] = 0;
        }
    }
    
    int n_filled = (int)(S * N * N);
    
    int count = 0;
    while (count < n_filled) {
        int color = (rand() % N) + 1;
        int xcoor = rand() % N;
        int ycoor = rand() % N;
        
        int valid = 1;
        
        if (board[xcoor][ycoor] > 0) valid = 0;
        
        for (j = 0; j < N; j++) {
            if (board[xcoor][j] == color) valid = 0;
            if (board[j][ycoor] == color) valid = 0;
        }
        
        if (valid) {
          board[xcoor][ycoor] = color;
          count++;
        }

    }
}



void
blsquares(int N, int x, int y, int board[N][N]) {
    register int valid;
    
    /* starting at (x, 0), look for any empty squares*/

    int i, j, c;
    int found_valid_sq = 0;
    if(found_solution == 0){
		for(i = x; i < N; i++){
			for(j = 0; j < N; j++){
			  if (board[i][j] == 0) {
				found_valid_sq = 1;
				break;
			  }
			}
			if (found_valid_sq) break;
		}
			
		if (found_valid_sq) {
		  // recurse;
		  for (c = 1; c < N + 1; c++) {
			valid = 1;
			int l;
			for (l = 0; l < N; l++) {
			  if (board[i][l] == c) valid = 0;
			  if (board[l][j] == c) valid = 0;
			}
			//printf("x: %d, y: %d, c: %d, valid: %d \n", i, j, c, valid);
			if (valid) {
			  int new_board[N][N];
			  memcpy(new_board, board, N*N*sizeof(int));
			  new_board[i][j] = c;
			  blsquares(N, i, j, new_board);
			}
		  }
		}
		else {
			for (i = 0; i < N; i++) {
			  for (j = 0; j < N; j++) {
				printf("%d  ", board[i][j]);
			  }
			  printf("\n");
			}
			printf("\n");
      found_solution = 1;
		}
	}
    return;
}

int
main(int argc, char **argv) {
    struct timespec start, end;
    double timet;
    time_t t;

    int N;
    double S;
    
    if(argc != 3) {
        printf("Usage: bqueen n s\nAborting...\n");
        exit(0);
    }
    N = atoi(argv[1]);
    
    char *ptr;
    S = strtod(argv[2], &ptr);
    
    int board_stack[N][N];
    int i,j;
    
    //srand((unsigned) time(&t));
    srand(10);    
    create_board(N, S, board_stack);
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        printf("%d  ", board_stack[i][j]);
      }
      printf("\n");
    }
    printf("\n");
	
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    blsquares(N, 0, 0, board_stack);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    timet =
    BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    timet = timet / BILLION;
    
    printf("Elapsed: %lf seconds\n", timet);
    
    printf("Solution found: %d\n", found_solution);

    return 0;
}

