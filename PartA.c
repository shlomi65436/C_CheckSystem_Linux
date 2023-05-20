#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd1, fd2, res;
    char buf1[100], buf2[100];

    if (argc != 3) {
        exit(1);
    }

    fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        exit(1);
    }

    fd2 = open(argv[2], O_RDONLY);
    if (fd2 == -1) {
        close(fd1);
        exit(1);
    }

    long endFile = lseek(fd1, 0, SEEK_END);
    if (lseek(fd2, 0, SEEK_END) != endFile) {
        close(fd1);
        close(fd2);
        exit(1);
    }

    lseek(fd1, 0, SEEK_SET);
    lseek(fd2, 0, SEEK_SET);

    while ((res = read(fd1, buf1, 100)) > 0) {
        if (read(fd2, buf2, res) != res) {
            close(fd1);
            close(fd2);
            exit(1);
        }
        for (int i = 0; i < res; i++) {
            if (buf1[i] != buf2[i]) {
                close(fd1);
                close(fd2);
                exit(1);
            }
        }
    }

    if (res < 0) {
        close(fd1);
        close(fd2);
        exit(1);
    }

    close(fd1);
    close(fd2);

    exit(2);
}
