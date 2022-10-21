#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (2 > argc){
        write(2, "give a file\n", 12);
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);

    if (-1 == fd) {
        write(2, "file open failed, or file doesn't exist\n", 12);
        exit(1);
    }

    char n;
    while (0 != read(fd, &n, 1)) {
        write(1, &n, 1);
    } write(1, "\n", 1);

    close(fd);
    return 0;
}