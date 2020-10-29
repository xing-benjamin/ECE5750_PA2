#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BILLION 1000000000L

typedef struct {
    int **board;
    int N, P, pid, x, y, level;
} GM;

int found_solution;
pthread_mutex_t lock;

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
plsquares(void *varg) {
    GM *arg = varg;
    int P, N, x, y, level, pid;
    int **board;
    P = arg->P;
    N = arg->N;
    board = arg->board;
    x = arg->x;
    y = arg->y;
    pid = arg->pid;
    level = arg->level;
    
    register int valid;
    
    /* starting at (x, 0), look for any empty squares */
    
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
		  if(level){
			  for (c = 1; c < N + 1; c++) {
				valid = 1;
				int l;
				for (l = 0; l < N; l++) {
				  if (board[i][l] == c) valid = 0;
				  if (board[l][j] == c) valid = 0;
				}
				printf("x: %d, y: %d, c: %d, valid: %d \n", i, j, c, valid);
				if (valid) {
				  GM arg1;
				  arg1.P = P;
				  arg1.N = N;
				  arg1.pid = pid;
				  arg1.x = i;
				  arg1.y = j;
				  arg1.level= 1;
				  int **new_board;
				  memcpy(&new_board, &board, N*N*sizeof(int));
				  new_board[i][j] = c;
				  arg1.board = new_board;
				  plsquares(&arg1);
				}
			  }
		    }
		  else{
			   for (c = pid +1; c < N + 1; c = c+P) {
				valid = 1;
				int l;
				for (l = 0; l < N; l++) {
				  if (board[i][l] == c) valid = 0;
				  if (board[l][j] == c) valid = 0;
				}
				printf("x: %d, y: %d, c: %d, valid: %d \n", i, j, c, valid);
				if (valid) {
				  GM arg1;
				  arg1.P = P;
				  arg1.N = N;
				  arg1.pid = pid;
				  arg1.x = i;
				  arg1.y = j;
				  arg1.level= 1;
				  int **new_board;
				  memcpy(&new_board, &board, N*N*sizeof(int));
				  new_board[i][j] = c;
				  arg1.board = new_board;
				  plsquares(&arg1);
				  
				}
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
		    pthread_mutex_lock(&lock);
		    found_solution = 1;
		    pthread_mutex_unlock(&lock);
		}
	}
    return;
}


int
main(int argc, char **argv) {
    struct timespec start, end;
    double timet;
    time_t t;

    int N,P;
    double S;
    
    if(argc != 4) {
        printf("Usage: plsquares n s p\nAborting...\n");
        exit(0);
    }
    N = atoi(argv[1]);
    P = atoi(argv[3]);
    char *ptr;
    S = strtod(argv[2], &ptr);
   
    found_solution = 0;
    int board_stack[N][N];
    int i,j;
    int level = 0;
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
    
    int **board = (int **) malloc(N * sizeof(int *));
    for(i = 0; i < N; i++) {
        board[i] = (int *) malloc(N * sizeof(int));
        for(j = 0; j < N; j++) {
            board[i][j] = board_stack[i][j];
        }
    }
    
    //lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&lock,NULL);

    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_t *threads = malloc(P * sizeof(threads));
    for(i = 0; i < P; i++) {
        GM *arg = malloc(sizeof(*arg));
        arg->P = P;
        arg->N = N;
        arg->board = board;
        arg->x = 0;
        arg->y = 0;
        arg->pid = i;
        arg->level = level;
        pthread_create(&threads[i], NULL, plsquares, arg);
    }
    
    for(i = 0; i < P; i++) {
      pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&lock);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    timet =
    BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    timet = timet / BILLION;
    
    printf("Elapsed: %lf seconds\n", timet);
    
    printf("Solution found: %d\n", found_solution);

    return 0;
}

