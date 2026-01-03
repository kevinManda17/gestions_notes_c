#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void clear_stdin(void);
void read_line(const char *prompt, char *buf, size_t size);
int  read_int_range(const char *prompt, int min, int max);
float read_float_range(const char *prompt, float min, float max);

#endif
