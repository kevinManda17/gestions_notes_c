#ifndef STUDENT_H
#define STUDENT_H

#include <stddef.h>

#define MAX_STUDENTS  200
#define MAX_ID        24
#define MAX_NAME      64
#define MAX_SUBJECTS  30

typedef struct {
    char  id[MAX_ID];
    char  name[MAX_NAME];
    float notes[MAX_SUBJECTS];
    int   noteCount;
} Student;

typedef struct {
    Student arr[MAX_STUDENTS];
    int count;
} Classroom;

/* Core operations */
void classroom_init(Classroom *c);

float student_average(const Student *s);
float class_average(const Classroom *c);

int  find_student_index_by_id(const Classroom *c, const char *id);

int  add_student(Classroom *c, const Student *s);      /* returns 1 success, 0 fail */
int  remove_student_by_id(Classroom *c, const char *id);/* returns 1 success, 0 fail */

/* Display */
void print_student_summary(const Student *s, int index);
void print_student_details(const Student *s);
void print_class_list(const Classroom *c);

/* Stats */
int  best_student_index(const Classroom *c);
int  worst_student_index(const Classroom *c);

#endif
