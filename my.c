#include <stdlib.h>
#include <m4ri/mzd.h>
#include <m4ri/config.h>
#include <m4ri/misc.h>
#include <x86intrin.h>
#include <time.h>

#define VECTOR_SIZE 1024
#define NUMBER_OF_SAMPLES 10000
#define clear 1

int main(){
    srand(time(NULL));
    mzd_t* A;                          // predefined matrix for multiplication 
    A = mzd_init(VECTOR_SIZE, VECTOR_SIZE);
    mzd_randomize(A);
    
    // Random bits= total VECTOR_SIZE * NUMBER_OF_SAMPLES many
    uint8_t* data = (uint8_t*)malloc(NUMBER_OF_SAMPLES * VECTOR_SIZE * sizeof(uint8_t));
    if (data == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
        }

    // Fill the array with random values 
    for (int i = 0; i < NUMBER_OF_SAMPLES * VECTOR_SIZE; i++) {
        data[i] = rand() % 2; 
    }
    

    /* ________________ Warmup computation ________________ */
    for (int i = 0; i < NUMBER_OF_SAMPLES/4; i++)
    {
        mzd_t *v = mzd_init(VECTOR_SIZE,1);
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            mzd_write_bit(v, j, 0, data[i*VECTOR_SIZE + j]);   // loading random vits into the sample vector
        }
        
        mzd_t *C = mzd_init(VECTOR_SIZE, 1);                  // matrix for storing multiplication result
        mzd_t *D = _mzd_mul_va(C, v, A, clear);               // v*A multiplication function from mzd.c
        
    }
    printf("Warmup done\n");




    /* ________________ Actual benchmark________________ */
    uint64_t start, end;
    unsigned int dummy;
    printf("Start rdtscp\n");
    start = _rdtscp(&dummy);  

    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        mzd_t *v = mzd_init(VECTOR_SIZE,1);
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            mzd_write_bit(v, j, 0, data[i*VECTOR_SIZE + j]);
        }
        
        mzd_t *C = mzd_init(VECTOR_SIZE, 1);             // matrix for storing multiplication result
        mzd_t *D = _mzd_mul_va(C, v, A, clear);          // v*A multiplication function from mzd.c

        mzd_free(v);
        
    }
    printf("Multiplication done\n");
    end = _rdtscp(&dummy);
    double diff = (double) (end-start)/ NUMBER_OF_SAMPLES;
    printf("Average number of CPU cycles: %f\n", diff);

    return 0;

}