#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sqlite3.h"
#include "in_out.h"
#include "tools.h"

// Uncomment according to OS compilation target
#define SYS_CLR() system("clear") // Linux
//#define SYS_CLR() system("cls") // Windows

student *students[21] = {NULL};
student *all_students[105] = {NULL};
char *assignments[105] = {NULL};
int val = 0;
int *ptr = &val;

const char* const time[21] = {
    "10:00am", "10:30am", "11:00am", "11:30am", "12:00pm", "12:30pm", "1:00pm", 
    "1:30pm", "2:00pm", "2:30pm", "3:00pm", "3:30pm", "4:00pm", "4:30pm", 
    "5:00pm", "5:30pm", "6:00pm", "6:30pm", "7:00pm", "7:30pm", "8:00pm", 
};

const char* const instrument[3] = {"Guitar", "Ukulele", "Bass"};

int callback_students(void *not_used, int count, char **data, char **columns) {
    student *temp = (student *)malloc(sizeof(student));
    strcpy(temp->name, data[0]);
    strcpy(temp->start_date, data[1]);
    strcpy(temp->instrument, data[2]);
    strcpy(temp->time, data[3]);
    students[time_hash(temp->time)] = temp;
    return 0;
}

int callback_allstudents(void *not_used, int count, char **data, char **columns) {
    student *temp = (student *)malloc(sizeof(student));
    strcpy(temp->name, data[0]);
    strcpy(temp->start_date, data[1]);
    strcpy(temp->instrument, data[2]);
    strcpy(temp->time, data[3]);
    all_students[*ptr] = temp;
    (*ptr)++;
    return 0;
}

int callback_assignments(void *not_used, int count, char **data, char **columns) {
    printf("%s - ", data[0]);
    text_wrap(data[1], 50);
    printf("%s\n\n", data[1]);
    return 0;
}

int callback_archive(void *not_used, int count, char **data, char **columns) {
    for (int i = 0; i < 4; i++) {
        printf("%s  ", data[i]);
    }
    printf("\n");
    return 0;
}

int time_hash(char *t) {
    for (int i = 0; i < 21; i++) {
        if (strcmp(t, time[i]) == 0) return i;
    }
}

unsigned int get_time() {
    int time_choice;

    for (int i = 0; i < 21; i++) {
        printf("%d - %s\n", i + 1, time[i]);
    }
    printf("Select Time: ");
    scanf("%d", &time_choice);

    return time_choice;
}

void destroy_array() {
    for (int i = 0; i < 21; i++) {
        if (students[i]) {
            free(students[i]);
            students[i] = NULL;
        }
    }
}

int initial_menu(sqlite3 *database, int *day) {
    SYS_CLR();
    printf("Student Log version 4.0\n----------------------\n");
    printf("1 - Tuesday\n"
            "2 - Wednesday\n"
            "3 - Thursday\n"
            "4 - Friday\n"
            "5 - Saturday\n"
            "6 - Display All Students\n"
            "E - EXIT\n");
    char c;
    char *err_msg = 0;
    while (1) {
        printf("Enter Selection: ");
        scanf(" %c", &c);
        clear_stdin();
        int n = (int)(c - '0');
        switch (n) {
            case 21:
            case 53:
                destroy_array();
                sqlite3_close(database);
                exit(0);
            case 1 ... 5:
                char query[100];
                *day = n + 1;
                sprintf(query, "SELECT name, start_date, instrument, time FROM student_base "\
                               "WHERE day = %d ORDER BY time ASC;", *day);
                int rc = sqlite3_exec(database, query, callback_students, students, &err_msg);
                return 0;
            case 6:
                sqlite3_exec(
                    database, 
                    "SELECT name, start_date, instrument, time FROM student_base ORDER BY day ASC, time ASC;",
                    callback_allstudents,
                    all_students,
                    &err_msg
                );
                int i = 0;
                while (all_students[i]) {
                    printf("%d - %s\n", i + 1, all_students[i]->name);
                    char *new_query = (char *)malloc(150 * sizeof(char));
                    sprintf(new_query, "SELECT date, assignment FROM assignments "\
                                "WHERE assignment_id = (SELECT MAX(assignment_id) FROM assignments WHERE "\
                                "name = '%s');", all_students[i]->name);
                    printf("Latest Assignment:\n");
                    sqlite3_exec(database, new_query, callback_assignments, 0, &err_msg);
                    free(new_query);
                    i++;
                }
                *ptr = 0;
                break;
            case 0:
                sqlite3_exec(database, "SELECT name, start_date, end_date, instrument FROM student_base "\
                                       "WHERE day = -1 ORDER BY name ASC;", callback_archive, 0, &err_msg);
                break;
            default:
                printf("Incorrect input. Try again.\n");    
        }
    }
}

// returns student choice
int main_menu(sqlite3 *database, int *day) {
    SYS_CLR();
    char *days[] = {
        "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"
    };
    printf("%s\n---------\n", days[*day - 2]);
    while (1) {
        int j = 1;
        for (int i = 0; i < 21; i++) {
            if (students[i]) {
                printf("%d - %s\n", j++, students[i]->name);
            }
        }
        printf("A - ADD STUDENT\n");
        printf("D - DELETE STUDENT\n");
        printf("R - RETURN\n");

        char str[2] = {};
        int confirm = 0;
        while (1) {
            printf("Enter Selection: ");
            scanf("%s", str);
            clear_stdin();
            int n = str_to_int(str);
        
            switch (n) {
                case 17:
                case 49:
                    add_student(database, *day);
                    confirm++;
                    break;
                case 20:
                case 52:
                    delete_student(database);
                    confirm++;
                    break;
                case 34:
                case 66:
                    return -1;
                default:
                    if (n <= 0 || n >= j) {
                        printf("Incorrect input. Try again.\n");
                        break;
                    }
                    return n;
            }
            if (confirm == 1) break;
        }
    }  
}

int student_menu(sqlite3 *database, int choice) {
    SYS_CLR();
    int index, count;
    for (index = 0, count = 0; index < 21; index++) {
        if (students[index]) count++;
        if (count == choice) break;
    }

    while (1) {
        char *query, *assignment;
        int len;
        char *err_msg = 0;

        // display student information
        printf("Name: %s\n", students[index]->name);
        printf("Start Date: %s\n", students[index]->start_date);
        printf("Instrument: %s\n", students[index]->instrument);
        printf("Time: %s\n\n", students[index]->time);
        // get latest assignment
        query = (char *)malloc(150 * sizeof(char));
        sprintf(query, "SELECT date, assignment FROM assignments "\
                       "WHERE assignment_id = (SELECT MAX(assignment_id) FROM assignments WHERE "\
                       "name = '%s');", students[index]->name);
        printf("Latest Assignment:\n");
        sqlite3_exec(database, query, callback_assignments, 0, &err_msg);
        free(query);

        printf("A - ASSIGN\n");
        printf("V - VIEW ALL ASSIGNMENTS\n");
        printf("E - EDIT PROFILE\n");
        printf("R - RETURN\n");
        printf("Enter Selection: ");

        char choice;
        scanf(" %c", &choice);
        clear_stdin();
        choice = toupper(choice);
        //char *use = "use";
        switch (choice) {
            case 'A':
                printf("Enter Assignment (C to Cancel): ");
                len = get_var_text(&assignment);
                // if user wants to cancel operation
                if (toupper(assignment[0]) == 'C' && len <= 1) return 0;
                // otherwise proceed
                query = (char *)malloc((len + 120) * sizeof(char));
                sprintf(query, "INSERT INTO assignments (date, name, assignment) "\
                               "VALUES (date(\"now\", \"localtime\"), \"%s\", \"%s\");",
                                students[index]->name, assignment);
                free(assignment);
                sqlite3_exec(database, query, callback_assignments, 0, &err_msg);
                free(query);
                return 0;
            case 'E':
                edit_student(database, students[index]->name);
                break;
            case 'V': // consider using the full method of preparing statements here, not the exec convenience wrapper
                query = (char *)malloc(110 * sizeof(char));
                sprintf(query, "SELECT date, assignment FROM assignments "\
                               "WHERE name = \"%s\" ORDER BY date ASC;", students[index]->name);
                int rc = sqlite3_exec(database, query, callback_assignments, 0, &err_msg);
                if (rc != SQLITE_ROW) {
                    printf("End of List.\n\n"); // change this when the prepare statement process is changed
                }
                free(query);
                printf("Press ENTER to return.\n\n");
                clear_stdin();
                SYS_CLR();
                break;
            case 'R':
                return 0;
            default:
                SYS_CLR();
                printf("Incorrect input. Try again.\n");
        }
    }
}

int add_student(sqlite3 *database, int day) {
    // declare all necessary variables
    char *err_msg = 0;
    int inst_choice;
    char name[30]; 
    char query[300];

    // get student name
    printf("Enter Name (C to Cancel): ");
    int len = get_text(name, 30);

    // if user wants to cancel operation
    if (toupper(name[0]) == 'C' && len == 1) return 0;

    // get instrument
    printf("1 - Guitar\n2 - Ukulele\n3 - Bass\n"\
           "Select Instrument: ");
    scanf("%d", &inst_choice);

    // get time of lesson
    int time_choice = get_time();

    // create and execute query
    sprintf(query, "INSERT INTO student_base (name, instrument, day, time, start_date)\
                    VALUES (\"%s\", \"%s\", \"%d\", \"%s\", date(\"now\", \"localtime\"));", 
                    name, instrument[inst_choice - 1], day, time[time_choice - 1]);
    sqlite3_exec(database, query, callback_students, 0, &err_msg);

    // insert student into students array
    sprintf(query, "SELECT name, start_date, instrument, time "\
                   "FROM student_base WHERE name = \"%s\";", name);
    sqlite3_exec(database, query, callback_students, students, &err_msg);
    SYS_CLR();
    return 0;
}

int edit_student(sqlite3 *database, char *name) {
    SYS_CLR();
    char *err_msg = 0;
    char query[300];

    printf("Editing %s\n----------------------\n", name);
    printf("N - NAME\n");
    printf("D - DAY\n");    
    printf("T - TIME\n");
    printf("I - INSTRUMENT\n");
    printf("R - RETURN\n");
    printf("Enter Selection: ");

    char choice;
    scanf(" %c", &choice);
    clear_stdin();
    choice = toupper(choice);

    switch (choice) {
        case 'N':
            char new_name[30];
            printf("Enter New Name: ");
            get_text(new_name, 30);
            sprintf(query, "UPDATE student_base SET name = \"%s\" WHERE name = \"%s\";", new_name, name);
            sqlite3_exec(database, query, callback_students, 0, &err_msg);
            break;
        case 'D':
            printf("1 - Tuesday\n"\
            "2 - Wednesday\n"\
            "3 - Thursday\n"\
            "4 - Friday\n"\
            "5 - Saturday\n"
            "R - RETURN\n");
            char c;
            unsigned int success = 0;
            while (1) {
                printf("Enter Selection: ");
                scanf(" %c", &c);
                clear_stdin();
                int n = (int)(c - '0');
                switch (n) {
                    case 34:
                    case 66:
                        return 0;
                    case 1 ... 5:
                        sprintf(query, "UPDATE student_base SET day = %d WHERE name = \"%s\";", n + 1, name);
                        sqlite3_exec(database, query, callback_students, 0, &err_msg);
                        success = 1;
                        break;
                    default:
                        printf("Incorrect input. Try again.\n");
                }
                if (success == 1) break;
            }
        case 'T':
            int time_choice = get_time();
            sprintf(query, "UPDATE student_base SET time = \"%s\" WHERE name = \"%s\";", time[time_choice - 1], name);
            sqlite3_exec(database, query, callback_students, 0, &err_msg);
            break;
        case 'I':
            printf("1 - Guitar\n2 - Ukulele\n3 - Bass\n"\
                   "Select Instrument: ");
            int inst_choice;
            scanf("%d", &inst_choice);
            sprintf(query, "UPDATE student_base SET instrument = \"%s\" WHERE name = \"%s\";", instrument[inst_choice - 1], name);
            sqlite3_exec(database, query, callback_students, 0, &err_msg);
            break;
        case 'R':
            return 0;
        default:
            SYS_CLR();
            printf("Incorrect input. Try again.\n");
    }

    return 0;

}

int delete_student(sqlite3 *database) {
    char query[120];
    char *err_msg = 0;
    char name[30];
    printf("Enter Name (C to Cancel): ");
    int len = get_text(name, 30);

    // if user wants to cancel operation
    if (toupper(name[0]) == 'C' && len == 1) return 0;

    if (name[0] == '-' && name[1] == 'd') { // if user wants to permanently delete the student
        char temp[30];
        strcpy(temp, &name[3]);
        
        sprintf(query, "DELETE FROM student_base WHERE name = \"%s\";", temp);

        // Remove student from student array
        for (int i = 0; i < 21; i++) {
            if (students[i] == NULL) continue;
            if (strcmp(students[i]->name, temp) == 0) {
                free(students[i]);
                students[i] = NULL;
            }
        }
    }
    else { // if user wants only to remove student from display
        sprintf(query, "UPDATE student_base SET day = -1, end_date = date(\"now\") "\
                       "WHERE name = \"%s\";", name);
        // Remove student from student array
        for (int i = 0; i < 21; i++) {
            if (students[i] == NULL) continue;
            if (strcmp(students[i]->name, name) == 0) {
                free(students[i]);
                students[i] = NULL;
            }
        }
    }
    sqlite3_exec(database, query, callback_students, 0, &err_msg);
    SYS_CLR();
    return 0;
}