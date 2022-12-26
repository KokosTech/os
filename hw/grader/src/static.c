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

/* uint8_t *get_correct_answers(int afid) {
    if (-1 == afid) {
        err(EBADF,
            "[get_correct_answers] file open failed, or file doesn't exist\n");
    }

    //  size_t ca_size = lseek(afid, 0, SEEK_END);

    // if (0 == ca_size) {
    //     errx(EINVAL, "[get_correct_answers] empty correct answers\n");
    // }

    uint8_t correct_answers[MAX_QUESTIONS + 1];

    for (size_t i = 0; i < MAX_QUESTIONS; ++i) {
        correct_answers[i] = read(afid, i, 1);
        lseek(afid, 1, SEEK_CUR);
    }

    correct_answers[MAX_QUESTIONS] = '\0';

    return correct_answers;
} */

uint8_t get_points(int sfid, uint8_t *correct_answers) {
    if (-1 == sfid || NULL == correct_answers) {
        err(EBADF, "[get_points] file open failed, or file doesn't exist\n");
    }

    size_t ca_size = strlen(correct_answers);

    if (0 == ca_size) {
        errx(EINVAL, "[get_points] empty correct answers\n");
    }

    /*     if (ca_size > lseek(sfid, 0, SEEK_END)) {
            errx(EINVAL,
                 "[get_points] student answers are shorter than correct
       answers\n");
        } */

    size_t points = 0;

    uint8_t student_answer;
    for (size_t i = 0; i < ca_size; ++i) {
        read(sfid, &student_answer, 1);

        // printf("student answer: %c, correct answer: %c\n", student_answer,
        //        correct_answers[i]);
        if (correct_answers[i] == student_answer) {
            ++points;
        }

        lseek(sfid, 1, SEEK_CUR);
    }

    return points;
}

uint8_t get_grade(uint8_t points) {
    if (0 <= points && points <= MAX_F) {
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
    if (-1 == rfid) {
        err(EBADF, "[write_grade] file open failed, or file doesn't exist\n");
    }

    uint8_t grade = get_grade(points);

    if (1 != write(rfid, &grade, 1)) {
        err(EBADF, "[write_grade] write failed\n");
    }
}

void grade_student(char *path_to_student, char *path_to_result,
                   char *correct_answers) {
    int sfid = open(path_to_student, O_RDONLY);
    int rfid = open(path_to_result, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (-1 == sfid || -1 == rfid) {
        err(EBADF, "[grade_student] file open failed, or file doesn't exist\n");
    }

    if (NULL == correct_answers) {
        err(EBADF, "[grade_student] get_correct_answers failed\n");
    }

    uint8_t points = get_points(sfid, correct_answers);
    printf("Points: %d, %c\n", points, get_grade(points));

    write_grade(rfid, points);

    close(sfid);
    close(rfid);
}

void grader(int argc, char *argv[]) {
    if (argc < 4) {
        errx(EINVAL, "[grader] invalid number of arguments\n");
    }

    char path_to_student[strlen(argv[1]) + MAX_STUDENTS_DIGITS];
    char path_to_result[strlen(argv[2]) + MAX_STUDENTS_DIGITS];
    char path_to_correct_answers[strlen(argv[3]) + 1];

    strcpy(path_to_correct_answers, argv[3]);

    int afid = open(path_to_correct_answers, O_RDONLY);

    printf("Correct answers: %s\n", path_to_correct_answers);

    if (-1 == afid) {
        err(EBADF, "[grader] file open failed, or file doesn't exist\n");
    }

    uint8_t correct_answers[MAX_QUESTIONS + 1];

    char c = 0;
    for (size_t i = 0; i < MAX_QUESTIONS; ++i) {
        if (1 != read(afid, &c, 1)) {
            err(EBADF, "[grader] read failed\n");
        }

        correct_answers[i] = c;
        printf("correct answer: %d\n", correct_answers[i]);
        lseek(afid, 1, SEEK_CUR);
    }

    correct_answers[MAX_QUESTIONS] = '\0';

    for (size_t i = 1; i <= MAX_STUDENTS; ++i) {
        if (argv[1][strlen(argv[1])] == '/')
            sprintf(path_to_student, "%s%zu\0", argv[1], i);
        else
            sprintf(path_to_student, "%s/%zu\0", argv[1], i);

        if (argv[2][strlen(argv[2])] == '/')
            sprintf(path_to_result, "%s%zu\0", argv[2], i);
        else
            sprintf(path_to_result, "%s/%zu\0", argv[2], i);

        printf("\n\nGrading student %s\n", path_to_student);
        printf("Writing result to %s\n", path_to_result);

        grade_student(path_to_student, path_to_result, correct_answers);
    }

    close(afid);
}