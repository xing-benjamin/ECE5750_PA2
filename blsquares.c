#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BILLION 1000000000L



int found_solution;
int num_solutions;

typedef struct {
    int *qxcoor_max, *qycoor_max;
    int max_profit, num_solutions;
} Sol;


//int 
//find_profit(int *qxcoor, int *qycoor, int N) {
    //int profit = 0;
    
    //int i;
    //for (i = 0; i < N; i++) {
      //profit = profit + abs(qxcoor[i] - qycoor[i]);
    //}
    //return profit; 
//}


void
create_board(int N, double S, int **board) {
    board = (int **) malloc(N * sizeof(int *));
    
    int i, j;
    for(i = 0; i < N; i++) {
        board[i] = (int *) malloc(N * sizeof(int));
        for(j = 0; j < N; j++) {
            board[i][j] = 0;
        }
    }
    
    srand(1);
    
    
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
    
    /* For the n-1 row, the for loop checks possible queen 
     * positions in the column */
    

    
    int i, j, c;
    int found_valid_sq = 0;
    
    for(i = x; i < N; i++){
        /*check that there is nothing in this column */
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
        if (valid) {
          
          int new_board[N][N];
          memcpy(new_board, board, N*N*sizeof(int));
          new_board[i][j] = c;
          
          blsquares(N, i, j, new_board);
          
        }
      }
      
    }
    
    else {
       found_solution = 1;
       num_solutions++;
    }
    
    return;

        

    
}







int
main(int argc, char **argv) {
    struct timespec start, end;
    double time;

    int N;
    double S;
    
    if(argc != 3) {
        printf("Usage: bqueen n s\nAborting...\n");
        exit(0);
    }
    N = atoi(argv[1]);
    
    char *ptr;
    S = strtod(argv[2], &ptr);
    

    
    int **board;
    create_board(N, S, board);
    

    int board_stack[N][N];
    memcpy(board_stack[N][N], board, N*N*sizeof(int));
    
    printf("got here");
    fflush(stdout);
    
    int i, j;
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        printf("%d  ", board_stack[i][j]);
      }
      printf("\n");
    }
    fflush(stdout);
    

    clock_gettime(CLOCK_MONOTONIC, &start);
    
    blsquares(N, 0, 0, board_stack);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    time =
    BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    time = time / BILLION;
    
    printf("Elapsed: %lf seconds\n", time);
    
    printf("Solution found: %d\n", found_solution);
    printf("Number of solutions: %d\n", num_solutions);

    return 0;
}

