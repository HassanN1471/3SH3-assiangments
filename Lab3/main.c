#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int n;
int **a = NULL;
int count = 0;
pthread_t thread[4];

int row_count=0;

// Declaration of thread condition variable 
pthread_cond_t c = PTHREAD_COND_INITIALIZER; 
  
// declaring mutex 
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; 

void printArray() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("------------\n");
}

void rowsort(int i) {
    for (int j = 0; j < n - 1; j++) {
        for (int k = 0; k < n - j - 1; k++) {
            if (i % 2 == 0) {
                //even rows
                if (a[i][k] > a[i][k + 1]) {
                    int temp = a[i][k];
                    a[i][k] = a[i][k + 1];
                    a[i][k + 1] = temp;
                }
            } else {
                //odd rows
                if (a[i][k] < a[i][k + 1]) {
                    int temp = a[i][k];
                    a[i][k] = a[i][k + 1];
                    a[i][k + 1] = temp;
                }
            }
        }
    }  
    if (row_count>3)
        row_count=0;
    row_count++;

}

void colsort(int i) {

    for (int j = 0; j < n - 1; j++) {
        for (int k = 0; k < n - j - 1; k++) {
            if (a[k][i] > a[k + 1][i]) {
                int temp = a[k][i];
                a[k][i] = a[k + 1][i];
                a[k + 1][i] = temp;
            }
        }
    }
    if (row_count > 3){
        row_count = 0;
    }
    row_count++;
}

void getArray() {
    FILE *input;
    input = fopen("input.txt", "r");
    
    //count lines in text file, value in n
    for (char c = getc(input); c != EOF; c = getc(input)) {
        if (c == '\n') { // Increment n if character is newline 
            n++;
        }
    }
    
    //reset pointer to top of file
    rewind(input);
    
    //allocate memory for n rows and n columns
    a = (int **) malloc(n * sizeof (int*));
    for (int i = 0; i < n; i++) {
        a[i] = (int *) malloc(n * sizeof (int));
    }
    
    //store values from text file in 2d array
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!fscanf(input, "%d", &a[i][j]))
                break;
        }
    }
    fclose(input);
    printArray();
}


void thr_join() {
    pthread_mutex_lock(&m);
    if (count < 3) {
        count++;
        pthread_cond_wait(&c, &m);
    }
    count=0;
    if (row_count>3){
        printArray();        
    }
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}


void dosort(int i){   
    rowsort(i);
    thr_join();
    colsort(i);
    thr_join();    
    rowsort(i);
    thr_join();
    colsort(i);
    thr_join();
    rowsort(i);  
}


int main() {

    getArray();
    int i = 0;
  
        for (i = 0; i < 4; i++){
            pthread_create(&thread[i], NULL, &dosort, i);
        }
        for (i = 0; i < 4; i++){
            pthread_join(thread[i], NULL);
        }
    
    printArray();

    return 0;
}