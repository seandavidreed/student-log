#ifndef __TOOLS_H__
#define __TOOLS_H__

typedef struct _student {
    char name[30];
    char start_date[15];
    char instrument[10];
    char time[10];
    int element;
} student;

extern student *students[21];

int callback_students(void *not_used, int count, char **data, char **columns);
int callback_assignments(void *not_used, int count, char **data, char **columns);

int time_hash(char *t);

void destroy_array();

int initial_menu(sqlite3 *database, int *day);
int main_menu(sqlite3 *database, int *day);
int student_menu(sqlite3 *database, int index);

int add_student(sqlite3 *database, int day);
int delete_student(sqlite3 *database);

int str_to_int(char *number);

#endif