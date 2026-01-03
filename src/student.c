#include "student.h"
#include <stdio.h>
#include <string.h>

void classroom_init(Classroom *c) {
    c->count = 0;
}

float student_average(const Student *s) {
    if (!s || s->noteCount <= 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < s->noteCount; i++) sum += s->notes[i];
    return sum / (float)s->noteCount;
}

float class_average(const Classroom *c) {
    if (!c || c->count == 0) return 0.0f;
    float sum = 0.0f;
    for (int i = 0; i < c->count; i++) {
        sum += student_average(&c->arr[i]);
    }
    return sum / (float)c->count; /* moyenne des moyennes */
}

int find_student_index_by_id(const Classroom *c, const char *id) {
    if (!c || !id) return -1;
    for (int i = 0; i < c->count; i++) {
        if (strcmp(c->arr[i].id, id) == 0) return i;
    }
    return -1;
}

int add_student(Classroom *c, const Student *s) {
    if (!c || !s) return 0;
    if (c->count >= MAX_STUDENTS) return 0;

    if (find_student_index_by_id(c, s->id) != -1) {
        return 0; /* matricule déjà existant */
    }

    c->arr[c->count] = *s;
    c->count++;
    return 1;
}

int remove_student_by_id(Classroom *c, const char *id) {
    if (!c || !id) return 0;

    int idx = find_student_index_by_id(c, id);
    if (idx == -1) return 0;

    for (int i = idx; i < c->count - 1; i++) {
        c->arr[i] = c->arr[i + 1];
    }
    c->count--;
    return 1;
}

void print_student_summary(const Student *s, int index) {
    if (!s) return;
    printf("%d) %s | %s | Moyenne: %.2f | Notes: %d\n",
           index, s->id, s->name, student_average(s), s->noteCount);
}

void print_student_details(const Student *s) {
    if (!s) return;

    printf("Matricule : %s\n", s->id);
    printf("Nom       : %s\n", s->name);
    printf("Notes     : ");
    for (int i = 0; i < s->noteCount; i++) {
        printf("%.1f", s->notes[i]);
        if (i < s->noteCount - 1) printf(", ");
    }
    printf("\nMoyenne   : %.2f\n", student_average(s));
}

void print_class_list(const Classroom *c) {
    if (!c) return;
    printf("\n=== Liste des étudiants (%d) ===\n", c->count);
    if (c->count == 0) {
        printf("Aucun étudiant enregistré.\n");
        return;
    }
    for (int i = 0; i < c->count; i++) {
        print_student_summary(&c->arr[i], i + 1);
    }
}

int best_student_index(const Classroom *c) {
    if (!c || c->count == 0) return -1;
    int best = 0;
    float bestAvg = student_average(&c->arr[0]);
    for (int i = 1; i < c->count; i++) {
        float a = student_average(&c->arr[i]);
        if (a > bestAvg) {
            bestAvg = a;
            best = i;
        }
    }
    return best;
}

int worst_student_index(const Classroom *c) {
    if (!c || c->count == 0) return -1;
    int worst = 0;
    float worstAvg = student_average(&c->arr[0]);
    for (int i = 1; i < c->count; i++) {
        float a = student_average(&c->arr[i]);
        if (a < worstAvg) {
            worstAvg = a;
            worst = i;
        }
    }
    return worst;
}
