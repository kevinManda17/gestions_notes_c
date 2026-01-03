#include "storage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void safe_copy(char *dst, size_t dstSize, const char *src) {
    if (!dst || dstSize == 0) return;
    if (!src) { dst[0] = '\0'; return; }
    strncpy(dst, src, dstSize - 1);
    dst[dstSize - 1] = '\0';
}

int load_classroom_csv(Classroom *c, const char *filename) {
    if (!c || !filename) return 0;

    FILE *f = fopen(filename, "r");
    if (!f) {
        /* fichier absent => OK, on démarre vide */
        return 1;
    }

    char line[1024];
    int loaded = 0;

    while (fgets(line, sizeof(line), f)) {
        /* ignorer lignes vides */
        if (line[0] == '\n' || line[0] == '\0') continue;

        /* enlever \n */
        line[strcspn(line, "\n")] = '\0';

        Student s;
        memset(&s, 0, sizeof(Student));

        /* tokenizer */
        char *token = strtok(line, ";");
        if (!token) continue;
        safe_copy(s.id, sizeof(s.id), token);

        token = strtok(NULL, ";");
        if (!token) continue;
        safe_copy(s.name, sizeof(s.name), token);

        token = strtok(NULL, ";");
        if (!token) continue;
        s.noteCount = atoi(token);

        if (s.noteCount < 0) s.noteCount = 0;
        if (s.noteCount > MAX_SUBJECTS) s.noteCount = MAX_SUBJECTS;

        for (int i = 0; i < s.noteCount; i++) {
            token = strtok(NULL, ";");
            if (!token) {
                s.notes[i] = 0.0f;
            } else {
                s.notes[i] = (float)atof(token);
                /* clamp au cas où */
                if (s.notes[i] < 0.0f) s.notes[i] = 0.0f;
                if (s.notes[i] > 20.0f) s.notes[i] = 20.0f;
            }
        }

        if (c->count < MAX_STUDENTS) {
            /* éviter doublons id */
            if (find_student_index_by_id(c, s.id) == -1) {
                c->arr[c->count++] = s;
                loaded++;
            }
        } else {
            break;
        }
    }

    fclose(f);
    (void)loaded;
    return 1;
}

int save_classroom_csv(const Classroom *c, const char *filename) {
    if (!c || !filename) return 0;

    FILE *f = fopen(filename, "w");
    if (!f) return 0;

    for (int i = 0; i < c->count; i++) {
        const Student *s = &c->arr[i];
        fprintf(f, "%s;%s;%d", s->id, s->name, s->noteCount);
        for (int j = 0; j < s->noteCount; j++) {
            fprintf(f, ";%.2f", s->notes[j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return 1;
}
