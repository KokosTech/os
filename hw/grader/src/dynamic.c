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
#include "utils/path.h"

uint8_t countDigits(size_t integer) {
    uint8_t count = 0;
    while (integer != 0) {
        ++count;
        integer /= 10;
    }

    return count;
}

uint8_t *get_correct_answers(char *correct_answers_file) {
    if (NULL == correct_answers_file)
        errx(EINVAL,
             "[get_correct_answers] invalid correct answers filename\n");

    int afid = open(correct_answers_file, O_RDONLY);

    if (-1 == afid)
        err(EBADF,
            "[get_correct_answers] file open failed, or file doesn't exist\n");

    uint8_t *correct_answers = malloc(MAX_QUESTIONS + 1);

    if (NULL == correct_answers) {
        close(afid);
        err(ENOMEM, "[get_correct_answers] malloc failed\n");
    }

    for (size_t i = 0; i < MAX_QUESTIONS; ++i) {
        int read_bytes = read(afid, &correct_answers[i], 1);

        if (-1 == read_bytes) {
            close(afid);
            free(correct_answers);
            err(EBADF, "[get_correct_answers] read failed\n");
        }

        if (1 != read_bytes) {
            close(afid);
            free(correct_answers);
            errx(EINVAL,
                 "[get_correct_answers] invalid file or insufficient data\n");
        }

        if (-1 == lseek(afid, 1, SEEK_CUR)) {
            close(afid);
            free(correct_answers);
            err(EBADF, "[get_correct_answers] lseek failed\n");
        }
    }

    correct_answers[MAX_QUESTIONS] = '\0';

    close(afid);

    return correct_answers;
}

size_t get_points(char *students_answers_file, uint8_t *correct_answers) {
    if (NULL == students_answers_file) {
        if (NULL != correct_answers) free(correct_answers);
        errx(EINVAL, "[get_points] invalid students answers filename\n");
    }

    if (NULL == correct_answers) {
        if (NULL != students_answers_file) free(students_answers_file);
        errx(EINVAL, "[get_points] invalid correct answers\n");
    }

    size_t ca_size = strlen((char *)correct_answers);

    if (0 == ca_size) {
        free(students_answers_file);
        free(correct_answers);
        errx(EINVAL, "[get_points] empty correct answers\n");
    }

    int sfid = open(students_answers_file, O_RDONLY);

    if (-1 == sfid) {
        free(students_answers_file);
        free(correct_answers);
        err(EBADF, "[get_points] file open failed, or file doesn't exist\n");
    }

    size_t points = 0;
    uint8_t student_answer = 0;

    for (size_t i = 0; i < ca_size; ++i) {
        int read_bytes = read(sfid, &student_answer, 1);

        if (-1 == read_bytes) {
            free(students_answers_file);
            free(correct_answers);
            close(sfid);
            err(EBADF, "[get_points] read failed\n");
        }

        if (1 != read_bytes) {
            free(students_answers_file);
            free(correct_answers);
            close(sfid);
            errx(EINVAL, "[get_points] invalid file or insufficient data\n");
        }

        if (correct_answers[i] == student_answer) ++points;

        if (-1 == lseek(sfid, 1, SEEK_CUR)) {
            free(students_answers_file);
            free(correct_answers);
            close(sfid);
            err(EBADF, "[get_points] lseek failed\n");
        }
    }

    close(sfid);

    return points;
}

uint8_t get_grade(size_t points) {
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

void write_grade(char *path_to_result, size_t points) {
    if (NULL == path_to_result)
        errx(EINVAL, "[write_grade] invalid result filename\n");

    int rfid = open(path_to_result, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (-1 == rfid) {
        free(path_to_result);
        err(EBADF, "[write_grade] file open failed, or file doesn't exist\n");
    }

    uint8_t grade = get_grade(points);

    if (1 != write(rfid, &grade, 1)) {
        free(path_to_result);
        close(rfid);
        err(EBADF, "[write_grade] write failed\n");
    }

    close(rfid);
}

void grade_student(char *path_to_student, char *path_to_result,
                   uint8_t *correct_answers) {
    if (NULL == path_to_student) {
        if (NULL != path_to_result) free(path_to_result);
        if (NULL != correct_answers) free(correct_answers);
        errx(EINVAL, "[grade_student] invalid student answers filename\n");
    }

    if (NULL == path_to_result) {
        if (NULL != path_to_student) free(path_to_student);
        if (NULL != correct_answers) free(correct_answers);
        errx(EINVAL, "[grade_student] invalid result filename\n");
    }

    if (NULL == correct_answers) {
        if (NULL != path_to_student) free(path_to_student);
        if (NULL != path_to_result) free(path_to_result);
        errx(EINVAL, "[grade_student] invalid correct answers\n");
    }

    uint8_t points = get_points(path_to_student, correct_answers);

    write_grade(path_to_result, points);
}

void grader(int argc, char *argv[]) {
    char *students_answers_dir = get_path(argv[1]);
    char *correct_answers_file = argv[2];
    char *results_dir = get_path(argv[3]);

    uint8_t *correct_answers = get_correct_answers(correct_answers_file);

    for (size_t i = 1; i <= MAX_STUDENTS; ++i) {
        char *students_answers_file = malloc(
            (strlen(students_answers_dir) + (countDigits(i) + 1)) * sizeof(char));

        if (NULL == students_answers_file) {
            free(students_answers_dir);
            free(correct_answers);
            free(results_dir);
            errx(ENOMEM, "[grader] malloc failed\n");
        }

        if (1 >
            sprintf(students_answers_file, "%s%zu", students_answers_dir, i)) {
            free(students_answers_file);
            free(students_answers_dir);
            free(correct_answers);
            free(results_dir);
            errx(EINVAL, "[grader] sprintf failed\n");
        }

        char *results_file =
            malloc((strlen(results_dir) + (countDigits(i) + 1)) * sizeof(char));

        if (NULL == results_file) {
            free(students_answers_file);
            free(students_answers_dir);
            free(correct_answers);
            free(results_dir);
            errx(ENOMEM, "[grader] malloc failed\n");
        }

        if (1 > sprintf(results_file, "%s%zu", results_dir, i)) {
            free(results_file);
            free(students_answers_file);
            free(students_answers_dir);
            free(correct_answers);
            free(results_dir);
            errx(EINVAL, "[grader] sprintf failed\n");
        }


        printf("Grading student %s\n", students_answers_file);
        printf("Grading student %s\n", results_file);
        grade_student(students_answers_file, results_file, correct_answers);

        free(students_answers_file);
        free(results_file);
    }

    free(students_answers_dir);
    free(correct_answers);
    free(results_dir);
}
