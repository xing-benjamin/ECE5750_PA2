#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define BILLION 1000000000L

typedef struct {
    int *qxcoor_max, *qycoor_max;
    int max_profit, num_solutions;
} Sol;

//global variables to be set by the recursive function with final values
int num_solutions;
int max_profit;

//function to determine the profit based on the placement of all N queens
int 
find_profit(int *qxcoor, int *qycoor, int N) {
    int profit = 0;
    
    int i;
    for (i = 0; i < N; i++) {
      profit = profit + abs(qxcoor[i] - qycoor[i]);
    }
    return profit; 
}

/*function to determine the placement of the N queens so that the
*constraints about 1 queen per diagonal, row and column is maintained
*/
void
bqueen(int *qxcoor, int *qycoor, int y, int N, int *qxcoor_max, int *qycoor_max) {
    register int valid;
    
    /* For the n-1 row, the for loop checks possible queen 
     * positions in the column */
    
    int x;
    for(x = 0; x < N; x++){
        valid = 1;
        /*check that there are no other queens in this column */
        int i;
        for(i = 0; i < N; i++){
          if (qxcoor[i] != -1) {
            if(qxcoor[i] == x){
                valid = 0;
            }
            /* check that there are no other queens in the diagonal */
            int dist_x = abs(qxcoor[i] - x);
            int dist_y = abs(qycoor[i] - y);
            if(dist_x == dist_y) {
                valid = 0;
            }
          }
        }

        if (valid) {            
            int qxcoor_new[N];
            int qycoor_new[N];
            
            memcpy(qxcoor_new, qxcoor, N*sizeof(int));
            memcpy(qycoor_new, qycoor, N*sizeof(int));
            
            qycoor_new[N-y-1] = y;
            qxcoor_new[N-y-1] = x; 
            
            if (y > 0) {
            /*make sure that n-1 is >= 0 */
              bqueen(qxcoor_new, qycoor_new, y-1, N, qxcoor_max, qycoor_max);
            }
            else {
              num_solutions++;
              
              int profit = find_profit(qxcoor_new, qycoor_new, N);
              
              if (profit > max_profit) {
                max_profit = profit;
                
                memcpy(qxcoor_max, qxcoor_new, N*sizeof(int));
                memcpy(qycoor_max, qycoor_new, N*sizeof(int));
              }
            }   
        }
    }
    return;
  }






int
main(int argc, char **argv) {
    struct timespec start, end;
    double time;

    int N;
    
    if(argc != 2) {
        printf("Usage: bqueen n\nAborting...\n");
        exit(0);
    }
    N = atoi(argv[1]);
    
    int qxcoor[N];
    int qycoor[N];
    
    int i;
    for (i = 0; i < N; i++) {
      qxcoor[i] = -1;
      qycoor[i] = -1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    int * qxcoor_max = (int *) malloc(N * sizeof(int));
    int * qycoor_max = (int *) malloc(N * sizeof(int));
    //call the recursive function
    bqueen(qxcoor,qycoor, N-1, N, qxcoor_max, qycoor_max);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    time =
    BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    time = time / BILLION;
    
    printf("Elapsed: %lf seconds\n", time);
    
    printf("Number of solutions: %d\n", num_solutions);
    printf("Max profit: %d\n", max_profit);
    
    for (i = 0; i < N; i++) {
      
      printf("x: %d, y: %d \n", qxcoor_max[i], qycoor_max[i]);
    }

    return 0;
}

