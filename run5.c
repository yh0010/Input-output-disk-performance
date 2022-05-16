#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BLOCK_SIZE 1
#define BLOCK_COUNT 102400

unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

double read_file(char* fname, int block_size, int block_count) {
    int input_fd, i;
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

    for (i=0; i<block_count; i++){
        ret_in = read (input_fd, buffer, block_size);
        if (ret_in < 0) {
            perror("read error");
            exit(EXIT_FAILURE);
        }
        file_size += (ret_in / 1024.0 / 1024);
        xor_result ^= xorbuf(buffer, ret_in);
    }

    printf("The result of the xor calculation: %x\n", xor_result);

    close(input_fd);

    return file_size;
}

double lseek_file(char* fname, int block_size, int block_count) {
    int input_fd, i;
    double file_size = 0.0;
    off_t off;

    input_fd = open(fname, O_RDONLY);
        if (input_fd == -1) {
        perror("open file error");
        exit(EXIT_FAILURE);
    }

    for (i=0; i<block_count; i++) {
        off = lseek(input_fd, block_size, SEEK_CUR);
        if (off < 0) {
            perror("seek error");
            exit(EXIT_FAILURE);
        }
        file_size += (block_size / 1024.0 / 1024);
    }

    close(input_fd);

    return file_size;
}

int main(int argc, char* argv[]) {
    char* fname = argv[1];
    char* option = argv[2];
    double file_size;
    clock_t begin, end;
    double time_spent;

    if (strcmp(option, "-r") == 0) {
        begin = clock();
        file_size = read_file(fname, BLOCK_SIZE, BLOCK_COUNT);
        end = clock();
        time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("Done reading.\nfile size: %f MB\ntotal time spent: %f seconds\nspeed: %f MB/S\nsystem call speed: %f B/S\n",
                file_size, time_spent, file_size/time_spent, file_size/time_spent*1024*1024);
    } else if (strcmp(option, "-s") == 0) {
        begin = clock();
        file_size = lseek_file(fname, BLOCK_SIZE, BLOCK_COUNT);
        end = clock();
        time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
        printf("Done seeking.\nfile size: %f MB\ntotal time spent: %f seconds\nspeed: %f MB/S\nsystem call speed: %f B/S\n",
                file_size, time_spent, file_size/time_spent, file_size/time_spent*1024*1024);
    } else {
        perror("option error");
        exit(EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}