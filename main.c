#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "local_headers/sqlite3.h"
#include "local_headers/in_out.h"
#include "local_headers/tools.h"

// Don't forget to change the system() argument depending on what terminal you use!

int main() {

    // open studentlog database
    // if it does not exist, create database and tables
    sqlite3 *test, *database;
    int db_status = sqlite3_open_v2("studentlog.db", &test, SQLITE_OPEN_READONLY, "unix-none");
    sqlite3_close_v2(test);
    sqlite3_open("studentlog.db", &database);
    if (db_status != 0) {
        char *query = "CREATE TABLE student_base (\n\
            student_id INTEGER PRIMARY KEY,\n\
            name TEXT,\n\
            instrument TEXT,\n\
            day INTEGER,\n\
            time TEXT,\n\
            start_date TEXT,\n\
            end_date TEXT);\n\
            CREATE TABLE assignments (\n\
            assignment_id INTEGER PRIMARY KEY,\n\
            date TEXT,\n\
            name TEXT,\n\
            assignment TEXT);";
        char *err_msg;
        sqlite3_exec(database, query, callback_students, 0, &err_msg);
    }

    // PROGRAM LOOP
    while (1) {

        // initial menu: select day of the week and retrieve students from database
        int day;
        initial_menu(database, &day);
        system("clear");
        
        while (1) {
            // main menu: display students based on weekday
            int choice = main_menu(database, &day);
            if (choice == -1) break;
            // individual student menu: assign and view assignments
            student_menu(database, choice);
        }

        destroy_array();
    }

    return 0;
}