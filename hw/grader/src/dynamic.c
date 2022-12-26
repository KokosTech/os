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
#include "utils/path.h"

uint8_t *get_correct_answers(char *correct_answers_file) {
    int afid = open(correct_answers_file, O_RDONLY);

    if (-1 == afid)
        err(EBADF,
            "[get_correct_answers] file open failed, or file doesn't exist\n");

    uint8_t *correct_answers = malloc(MAX_QUESTIONS + 1);

    if (NULL == correct_answers)
        err(ENOMEM, "[get_correct_answers] malloc failed\n");

    for (size_t i = 0; i < MAX_QUESTIONS; ++i) {
        read(afid, &correct_answers[i], 1);
        lseek(afid, 1, SEEK_CUR);
    }

    correct_answers[MAX_QUESTIONS] = '\0';

    close(afid);

    return correct_answers;
}

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
    char *students_answers_dir = get_path(argv[1]);
    char *correct_answers_file = argv[2];
    char *results_dir = get_path(argv[3]);

    uint8_t *correct_answers = get_correct_answers(correct_answers_file);

    for (size_t i = 0; i < MAX_STUDENTS; ++i) {
        sprintf(students_answers_dir, "%s%zu\0", argv[1], i);
        sprintf(results_dir, "%s%zu\0", argv[3], i);
        grade_student(students_answers_dir, results_dir, correct_answers);
    }

    free(students_answers_dir);
    free(correct_answers);
    free(results_dir);
}