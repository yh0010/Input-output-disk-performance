// This code doesn't work, but shows out idea of implementing multithread

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define BLOCK_SIZE 16384

double file_size =  0.0;
int xor_result =  0;
pthread_mutex_t lock;

unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

void * read_thread(void *vargp) {
    unsigned int *buffer;
    ssize_t ret_in;
    pthread_t id = pthread_self();
    buffer = (unsigned int*) malloc((size_t) BLOCK_SIZE); 
    int *input_fd = (int *)vargp;
    ret_in = read (*input_fd, buffer, BLOCK_SIZE);
    if (ret_in > 0) {
        pthread_mutex_lock(&lock);
        file_size += (ret_in / 1024.0 / 1024);
        xor_result ^= xorbuf(buffer, ret_in);
        pthread_mutex_unlock(&lock);
    }
    printf("Thread ID: %lu\nCurrent file size: %f MB\nCurrent xor result: %x\n", id, file_size, xor_result);
    return NULL;
}

int main(int argc, char* argv[]) {
    char* fname = argv[1];
    double file_size;
    clock_t begin, end;
    double time_spent;
    int input_fd, i;
    pthread_t thread[20];

    file_size =  0.0;
    xor_result =  0;

    begin = clock();

    input_fd = open(fname, O_RDONLY);
    if (input_fd == -1) {
        perror("open file error");
        exit(EXIT_FAILURE);
    }

    for (i=0; i<20; i++) {
        pthread_create(&thread[i], NULL, &read_thread, (void *) &input_fd);
    }

    for (i=0; i<20; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("The result of the xor calculation: %x\n", xor_result);

    close(input_fd);
    end = clock();
    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Done reading.\ntotal time spent: %f seconds\n", time_spent);

    return (EXIT_SUCCESS);
}