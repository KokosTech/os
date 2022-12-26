#pragma once

#include <stdint.h>

#define MAX_QUESTIONS 25
#define MAX_B 21
#define MAX_C 18
#define MAX_D 15
#define MAX_F 12

#define MAX_STUDENTS_DIGITS 4  // 2 digits + '\0' + '/
#define MAX_STUDENTS 25

// char *get_correct_answers(int afid);

uint8_t get_points(int sfid, uint8_t *correct_answers);
uint8_t get_grade(uint8_t points);
void grade_student(char *path_to_student, char *path_to_result,
                   char *correct_answers);
void grader(int argc, char *argv[]);