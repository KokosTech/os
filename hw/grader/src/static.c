#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "main.h"
#include "utils/file.h"

uint8_t get_points(int sfid, uint8_t *correct_answers) {
    if (-1 == sfid || NULL == correct_answers)
        err(EBADF, "[get_points] file open failed, or file doesn't exist\n");

    size_t ca_size = strlen(correct_answers);

    if (0 == ca_size) errx(EINVAL, "[get_points] empty correct answers\n");

    size_t points = 0;
    uint8_t student_answer = NULL;

    for (size_t i = 0; i < ca_size; ++i) {
        read(sfid, &student_answer, 1);

        if (correct_answers[i] == student_answer) ++points;

        lseek(sfid, 1, SEEK_CUR);
    }

    return points;
}

uint8_t get_grade(uint8_t points) {
    if (points <= MAX_F) {
        return '2';
    } else if (points <= MAX_D) {
        return '3';
    } else if (points <= MAX_C) {
        return '4';
    } else if (points <= MAX_B) {
        return '5';
    } else if (points <= MAX_QUESTIONS) {
        return '6';
    } else {
        errx(EINVAL, "[get_grade] invalid points\n");
    }
}

void write_grade(int rfid, uint8_t points) {
    if (-1 == rfid)
        err(EBADF, "[write_grade] file open failed, or file doesn't exist\n");

    uint8_t grade = get_grade(points);

    if (1 != write(rfid, &grade, 1)) err(EBADF, "[write_grade] write failed\n");
}

void grade_student(char *path_to_student, char *path_to_result,
                   char *correct_answers) {
    int sfid = open(path_to_student, O_RDONLY);
    if (-1 == sfid)
        err(EBADF, "[grade_student] file open failed, or file doesn't exist\n");

    if (NULL == correct_answers)
        err(EBADF, "[grade_student] get_correct_answers failed\n");

    uint8_t points = get_points(sfid, correct_answers);
    close(sfid);

    int rfid = open(path_to_result, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (-1 == rfid)
        err(EBADF, "[grade_student] file open failed, or file doesn't exist\n");

    write_grade(rfid, points);

    close(rfid);
}

void grader(int argc, char *argv[]) {
    if (argc < 4) errx(EINVAL, "[grader] invalid number of arguments\n");

    char path_to_student[strlen(argv[1]) + MAX_STUDENTS_DIGITS];
    char path_to_result[strlen(argv[3]) + MAX_STUDENTS_DIGITS];
    char path_to_correct_answers[strlen(argv[2]) + 1];

    strcpy(path_to_correct_answers, argv[2]);

    int afid = open(path_to_correct_answers, O_RDONLY);

    if (-1 == afid) {
        err(EBADF, "[grader] file open failed, or file doesn't exist\n");
    }

    uint8_t correct_answers[MAX_QUESTIONS + 1];

    char c = 0;
    for (size_t i = 0; i < MAX_QUESTIONS; ++i) {
        if (1 != read(afid, &c, 1)) err(EBADF, "[grader] read failed\n");

        correct_answers[i] = c;

        lseek(afid, 1, SEEK_CUR);
    }

    correct_answers[MAX_QUESTIONS] = '\0';
    close(afid);

    for (size_t i = 1; i <= MAX_STUDENTS; ++i) {
        if (argv[1][strlen(argv[1])] == '/')
            sprintf(path_to_student, "%s%zu\0", argv[1], i);
        else
            sprintf(path_to_student, "%s/%zu\0", argv[1], i);

        if (argv[3][strlen(argv[3])] == '/')
            sprintf(path_to_result, "%s%zu\0", argv[3], i);
        else
            sprintf(path_to_result, "%s/%zu\0", argv[3], i);

        grade_student(path_to_student, path_to_result, correct_answers);
    }
}