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
    sqlite3 *database;
    int exit = sqlite3_open("studentlog.db", &database);
    if (exit) {
        printf("Error opening database. Exiting...");
        return -1;
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