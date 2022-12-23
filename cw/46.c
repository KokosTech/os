#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct string {
    uint8_t *str;
    size_t len;
};

int main() {
    int fdat1 = open("f1.dat", O_RDONLY);

    if (-1 == fdat1) {
        close(fdat1);
        perror("open");
        exit(1);
    }

    int fidx1 = open("f1.idx", O_RDONLY);

    int fdat2 = open("f2.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (-1 == fidx1) {
        close(fidx1);
        perror("open");
        exit(1);
    }

    uint16_t off;
    uint8_t len;
    uint8_t saved;

    while (read(fidx1, &off, sizeof(off)) > 0) {
        read(fidx1, &len, sizeof(len));

        lseek(fdat1, off * 4, SEEK_SET);

        uint8_t buf[len];
        read(fdat1, buf, len);
        read(fidx1, &saved, sizeof(saved));

        printf("%d %d %d ", off, len, saved);
        printf("%s\n", buf);

        if (isupper(buf[0])) write(fdat2, buf, len);
    }

    return 0;
}