#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BILLION 1000000000L

typedef struct {
    int *board;
    int N, P, pid, x, y, level;
} GM;

int found_solution;
pthread_mutex_t lock;
pthread_barrier_t  barrier;

struct timespec ts1, ts2, ts3, ts4;

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
    int *board;
    P = arg->P;
    N = arg->N;
    board = arg->board;
    x = arg->x;
    y = arg->y;
    pid = arg->pid;
    level = arg->level;
    
    if (level == 0) {
      //pthread_barrier_wait(&barrier);
      if(pid == 0){
        clock_gettime(CLOCK_MONOTONIC, &ts2);
      }
    }
    
    register int valid;
    
    /* starting at (x, 0), look for any empty squares */
    
    int i, j, c;
    int found_valid_sq = 0;
    if(found_solution == 0){
      for(i = x; i < N; ++i){
        for(j = 0; j < N; ++j){
          //if (board[i][j] == 0) {
          if (board[i*N + j] == 0) {
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
            if (found_solution) break;
            valid = 1;
            int l;
            for (l = 0; l < N; l++) {
              if (board[i*N + l] == c) valid = 0;
              if (board[l*N + j] == c) valid = 0;
            }
            //printf("level: %d, x: %d, y: %d, c: %d, valid: %d \n", level, i, j, c, valid);
            if (valid) {
              GM arg1;
              arg1.P = P;
              arg1.N = N;
              arg1.pid = pid;
              arg1.x = i;
              arg1.y = j;
              arg1.level= 1;
              int new_board[N][N];
              memcpy(new_board, board, N*N*sizeof(int));
              new_board[i][j] = c;
          
              arg1.board = (int *) new_board;
              plsquares(&arg1);
            }
          }
        }
        else{
          
          for (c = pid +1; c < N + 1; c = c+P) {
            if (found_solution) break;
            valid = 1;
            int l;
            for (l = 0; l < N; l++) {
              if (board[i*N + l] == c) valid = 0;
              if (board[l*N + j] == c) valid = 0;
            }
            //printf("x: %d, y: %d, c: %d, valid: %d \n", i, j, c, valid);
            if (valid) {
              GM arg1;
              arg1.P = P;
              arg1.N = N;
              arg1.pid = pid;
              arg1.x = i;
              arg1.y = j;
              arg1.level= 1;
              int new_board[N][N];
              memcpy(new_board, board, N*N*sizeof(int));
              new_board[i][j] = c;
              arg1.board = (int *) new_board;
              plsquares(&arg1);
            }
          }
        }
      }
      else {
        for (i = 0; i < N; i++) {
          for (j = 0; j < N; j++) {
          printf("%d  ", board[i*N + j]);
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
    double time_init, time_exec, time_fin;
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
    
  pthread_mutex_init(&lock,NULL);

  clock_gettime(CLOCK_MONOTONIC, &ts1);
  pthread_t *threads = malloc(P * sizeof(threads));
  for(i = 0; i < P; i++) {
      GM *arg = malloc(sizeof(*arg));
      arg->P = P;
      arg->N = N;
      arg->board = (int *) board_stack;
      arg->x = 0;
      arg->y = 0;
      arg->pid = i;
      arg->level = level;
      pthread_barrier_init(&barrier, NULL, P);
      pthread_create(&threads[i], NULL, plsquares, arg);
      
  }
  
  for(i = 0; i < P; i++) {
    pthread_join(threads[i], NULL);
  }
  
  clock_gettime(CLOCK_MONOTONIC, &ts3);
  
  pthread_mutex_destroy(&lock);

  clock_gettime(CLOCK_MONOTONIC, &ts4);
  
  
  time_init =
  BILLION *(ts2.tv_sec - ts1.tv_sec) +(ts2.tv_nsec - ts1.tv_nsec);
  time_init = time_init / BILLION;
  
  time_exec =
  BILLION *(ts3.tv_sec - ts2.tv_sec) +(ts3.tv_nsec - ts2.tv_nsec);
  time_exec = time_exec / BILLION;
  
  time_fin =
  BILLION *(ts4.tv_sec - ts3.tv_sec) +(ts4.tv_nsec - ts3.tv_nsec);
  time_fin = time_fin / BILLION;
  
  printf("Elapsed init: %lf seconds\n", time_init);
  printf("Elapsed exec: %lf seconds\n", time_exec);
  printf("Elapsed fin: %lf seconds\n", time_fin);
  
  printf("Solution found: %d\n", found_solution);

  return 0;
}


