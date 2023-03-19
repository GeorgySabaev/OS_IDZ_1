#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "unistd.h"
#include "fcntl.h"
#include "stdbool.h"
#include "stdlib.h"

bool solve(char buffer_sorted[5000], int size, int n) {
    for (int i = size - n; i >= 0; --i) {
        bool ordered = true;
        for (int j = 1; j < n; ++j) {
            if (buffer_sorted[i + j] <= buffer_sorted[i + j - 1]) {
                printf("%c %c\n", buffer_sorted[2], buffer_sorted[3]);
                ordered = false;
                break;
            }
        }
        if (ordered) {
            memmove(buffer_sorted, buffer_sorted + i, n);
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    int fd1[2];
    int fd2[2];

    pipe(fd1);
    pipe(fd2);
    if (argc != 4) {
        printf("Program requires 3 arguments, %d were provided.", argc - 1);
    }
    int len = atoi(argv[3]);
    int id1 = fork();
    if (id1 == 0) {
        int id2 = fork();
        if (id2 == 0) {
            int input = open(argv[1], O_RDONLY);
            char buffer[5000];
            int size = read(input, buffer, 5000);
            write(fd1[1], buffer, size);
        } else {
            char buffer[5000];
            int size = read(fd1[0], buffer, 5000);
            if (!solve(buffer, size, len)) {
                printf("No such string exists.");
                exit(1);
            }

            write(fd2[1], buffer, len);
            close(fd2[1]);
        }
    } else {
        int output = open(argv[2], O_WRONLY | O_TRUNC);
        char buffer[len];

        int size = read(fd2[0], buffer, len);
        write(output, buffer, size);
        close(output);
    }
}
