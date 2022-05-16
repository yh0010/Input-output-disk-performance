#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

double read_file(char* fname, int block_size) {
    int input_fd;
    double file_size = 0.0;
    unsigned int *buffer;
    ssize_t ret_in;
    int xor_result = 0;

    input_fd = open(fname, O_RDONLY);
    if (input_fd == -1) {
        perror("open file error");
        exit(EXIT_FAILURE);
    }
    
    buffer = (unsigned int*) malloc((size_t) block_size); 

    while((ret_in = read (input_fd, buffer, block_size)) > 0){
        file_size += (ret_in / 1024.0 / 1024);
        xor_result ^= xorbuf(buffer, ret_in);
    }

    printf("The result of the xor calculation: %x\n", xor_result);

    close(input_fd);

    return file_size;
}

int main(int argc, char* argv[]) {
    char* fname = argv[1];
    unsigned int block_size = strtol(argv[2], NULL, 10);
    double file_size;
    clock_t begin, end;
    double time_spent;
    begin = clock();
    file_size = read_file(fname, block_size);
    end = clock();
    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("Done reading.\nfile size: %f MB\ntotal time spent: %f seconds\nspeed: %f MB/S\n", file_size, time_spent, file_size/time_spent);
    return (EXIT_SUCCESS);
}