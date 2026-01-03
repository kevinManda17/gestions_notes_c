#ifndef STORAGE_H
#define STORAGE_H

#include "student.h"

/* CSV format (one line per student):
   id;name;noteCount;note1;note2;...;noteN
*/

int load_classroom_csv(Classroom *c, const char *filename);
int save_classroom_csv(const Classroom *c, const char *filename);

#endif
