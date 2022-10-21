#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (2 > argc) {
        write(2, "give a file\n", 12);
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);

    if (-1 == fd) {
        write(2, "file open failed, or file doesn't exist\n", 12);
        exit(1);
    }

    size_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char buffer[file_size];

    for(size_t i = 0; i < file_size; ++i) {
        read(fd, &buffer[i], 1);
    }

    close(fd);

    for(size_t i = 0; i < file_size; ++i) {
        for(size_t j = 0; j < file_size - 1; ++j) {
            if (buffer[j] > buffer[j+1]) {
                char temp = buffer[j];
                buffer[j] = buffer[j+1];
                buffer[j+1] = temp;
            }
        }
    }

    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (-1 == fd) {
        write(2, "file open failed, or file doesn't exist\n", 12);
        exit(1);
    }

    for(size_t i = 0; i < file_size; ++i) {
        write(fd, &buffer[i], 1);
    }

    close(fd);

    return 0; 
}