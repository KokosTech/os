#pragma once

#include <stdint.h>

#define MAX_QUESTIONS 25
#define MAX_B 21
#define MAX_C 18
#define MAX_D 15
#define MAX_F 12

#define MAX_STUDENTS_DIGITS 4  // 2 digits + '\0' + '/
#define MAX_STUDENTS 25

void grader(int argc, char *argv[]);
