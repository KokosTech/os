#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (3 > argc) {
        write(2, "give file(s)\n", 12);
        exit(1);
    }

    int src = open(argv[1], O_RDONLY);

    if (-1 == src) {
        write(2, "file open failed, of file doesn't exist\n", 12);
        exit(1);
    }

    int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (-1 == dest) {
        write(2, "can't open dest file\n", 12);
        exit(1);
    }

    char n;
    while (0 != read(src, &n, 1)) {
        write(dest, &n, 1);
    }

    close(src);
    close(dest);
    return 0;
}