#include "utils.h"
#include <stdio.h>
#include <string.h>

void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

void read_line(const char *prompt, char *buf, size_t size) {
    if (prompt) printf("%s", prompt);
    if (!fgets(buf, (int)size, stdin)) {
        buf[0] = '\0';
        return;
    }
    trim_newline(buf);
}

int read_int_range(const char *prompt, int min, int max) {
    char line[128];
    int value;

    while (1) {
        if (prompt) printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) continue;

        if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Entrée invalide. Valeur attendue entre %d et %d.\n", min, max);
    }
}

float read_float_range(const char *prompt, float min, float max) {
    char line[128];
    float value;

    while (1) {
        if (prompt) printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) continue;

        if (sscanf(line, "%f", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Entrée invalide. Valeur attendue entre %.1f et %.1f.\n", min, max);
    }
}
