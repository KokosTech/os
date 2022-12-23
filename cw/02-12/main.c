#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/* Зад. 44 2016-SE-02 Двоичните файлове f1 и f2 сьдьржат 32 битови числа без знак (wint32_t). Файльт f1
е съдьржа п двойки числа, нека і-тата двойка е < Хі, У >. Напишете програма на С, която извлича интервалите с начало хі и дьлжина У от файла 12 и ги записва залепени в изходен файл £3.
Пример:
• f1 сьдьржа 4 числа (2 двойки): 30000, 20, 19000, 10
• програмата записва в f3 две поредици 32-битови числа, взети от 12 както следва:
• най-напред се записват числата, които са на позиции 30000, 30001, 30002, ... 30019.
• след тях се записват числата от позиции 19000, 19001, ... 19009.
Забележка: С пылен брой точки ще се оценяват решения, които работят стс скорост, пропорцио-нална на размера на изходния файл £3. */

int main() {
    int fd1 = open("f1.txt", O_RDONLY);

    if (-1 == fd1) {
        close(fd1);
        perror("open");
        exit(1);
    }

    int fd2 = open("f2.txt", O_RDONLY);

    if (-1 == fd2) {
        close(fd2);
        perror("open");
        exit(1);
    }

    int fd3 = open("f3.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (-1 == fd3) {
        close(fd3);
        perror("open");
        exit(1);
    }

    
    int32_t pos, len;

    while (read(fd1, &pos, sizeof(pos)) > 0) {
        read(fd1, &len, sizeof(len));

        lseek(fd2, pos * sizeof(int32_t), SEEK_SET);

        int32_t buf[len];
        read(fd2, buf, len * sizeof(int32_t));

        write(fd3, buf, len * sizeof(int32_t));
    }

    return 0;
}