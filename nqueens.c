#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define BILLION 1000000000L



int num_solutions;
int max_profit;

typedef struct {
    int *qxcoor_max, *qycoor_max;
    int *qxcoor, *qycoor;
    int y, N;
    int p;
    int pid;
} GM;


int 
find_profit(int *qxcoor, int *qycoor, int N) {
    int profit = 0;
    
    int i;
    for (i = 0; i < N; i++) {
      profit = profit + abs(qxcoor[i] - qycoor[i]);
    }
    return profit; 
}



void
pqueen(int *qxcoor, int *qycoor, int y, int N, int *qxcoor_max, int *qycoor_max) {
        int * qxcoor_max, qycoor_max;
        int * qxcoor, qycoor;
        int p, pid;
        int y, N;
        
        
        GM *arg = malloc(sizeof(*arg));
        
        qxcoor_max = arg->qxcoor_max;
        qycoor_max = arg->qycoor_max;
        qxcoor = arg->qxcoor;
        qycoor = arg->qycoor;
        y = arg->y;
        N = arg->N;
        p = arg->p;
        pid = arg->pid;
        
        register int valid = 1;

        /*check that there is nothing in this column */
        int i;
        for(i = 0; i < N; i++){
          if (qxcoor[i] != -1) {
            if(qxcoor[i] == x){
                valid = 0;
            }
            
            /* check that there is nothing in the diagonal */
            int dist_x = abs(qxcoor[i] - x);
            int dist_y = abs(qycoor[i] - y);
            if(dist_x == dist_y) {
                valid = 0;
            }
          }
        }
        
        
        if (valid) {
          
            /*create the new lists */
            //qxcoor_new = (int *) malloc(n * sizeof(int));
            //qycoor_new = (int *) malloc(n * sizeof(int));
            
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
                //qxcoor_max = qxcoor_new;
                //qycoor_max = qxcoor_new;
              }
            }
            
        }

}




int
main(int argc, char **argv) {
    struct timespec start, end;
    double time;

    int N, p;
    
    if(argc != 2) {
        printf("Usage: bqueen n\nAborting...\n");
        exit(0);
    }
    N = atoi(argv[1]);
    p = atoi(argv[2]);
    
    int qxcoor[N];
    int qycoor[N];
    
    int i;
    for (i = 0; i < N; i++) {
      qxcoor[i] = -1;
      qycoor[i] = -1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_t *threads = malloc(p * sizeof(threads));
    
    
    int * qxcoor_max = (int *) malloc(N * sizeof(int));
    int * qycoor_max = (int *) malloc(N * sizeof(int));
    
    for(i = 0; i < p; i++) {
        GM *arg = malloc(sizeof(*arg));
        
        arg->qxcoor_max = qxcoor_max;
        arg->qycoor_max = qycoor_max;
        arg->qxcoor = qxcoor;
        arg->qycoor = qycoor;
        arg->p = p;
        arg->pid = i;
        arg->y = N-1;
        arg->N = N;
        pthread_create(&threads[i], NULL, pqueen, arg);
      }
    


    //Sol *arg = malloc(sizeof(*arg));
    //arg->qxcoor_max = qxcoor_max;
    //arg->qycoor_max = qycoor_max;
    //arg->num_solutions = 0;
    //arg->max_profit = 0;
    

        pthread_create(&threads[i], NULL, pqueen, arg);
        
        
        
    bqueen(qxcoor,qycoor, N-1, N, qxcoor_max, qycoor_max);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    for(i = 0; i < p; i++) {
      pthread_join(threads[i], NULL);
    }
    
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


