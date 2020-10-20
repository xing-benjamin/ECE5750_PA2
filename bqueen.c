#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BILLION 1000000000L

void
bqueen(int *qxcoor, int *qycoor, int n, int N) {
    register int valid;
    valid = 1;
    /* For the n-1 row, the for loop checks possible queen 
     * positions in the column */
    for(int x = 0; x < N; x++){
		/*check that there is nothing in this column */
        for(int i = 0; i < N; i++){
            if(qxcoor[i] == x){
                valid = 0;
            }
        }
        
        /* check that there is nothing in the diagonal */
		/*create the new grid */
		qcoor = (int **) malloc(n * sizeof(int *));
        for(i = 0; i < 2; i++) {
		    qcoor[i] = (int *) malloc(n * sizeof(int));
        }

		/*make sure that n-1 is >= 0 */
		bqueen(int **grid, int (n-1));
	}
    
}

int
main(int argc, char **argv) {
    struct timespec start, end;
    double time;
    int n, i, j;
    double **a, *b, count = 1.0;
    
    if(argc != 2) {
        printf("Usage: bqueen n\nAborting...\n");
        exit(0);
    }
    n = atoi(argv[1]);
    
    qxcoor = (int *) malloc(n * sizeof(int));
    qycoor = (int *) malloc(n * sizeof(int));

    clock_gettime(CLOCK_MONOTONIC, &start);
    bqueen(qxcoor,qycoor, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    time =
    BILLION *(end.tv_sec - start.tv_sec) +(end.tv_nsec - start.tv_nsec);
    time = time / BILLION;
    
    printf("Elapsed: %lf seconds\n", time);
    for(i = 0; i < n; i++)
        printf("%lf ", b[i]);
    printf("\n");
    return 0;
}
