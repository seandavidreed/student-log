#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "local_headers/in_out.h"

void reformat_date(char *date) {
    char month[3] = {}, day[3] = {}, year[5] = {};
    month[0] = date[0];
    month[1] = date[1];
    day[0] = date[3];
    day[1] = (date[4] == '-') ? day[1] : date[4];
    int length = strlen(date), j = 0;
    for (int i = length - 4; i < length; i++) {
        year[j++] = date[i];
    }
    for (int k = 0; k < length; k++) {
        date[k] = '\0';
    }
    sprintf(date, "%d-%.2d-%.2d", str_to_int(year), str_to_int(month), str_to_int(day));
}

int main() {

    for (int i = 1; i < 57; i++) {
        char filename[50] = {};
        sprintf(filename, "students/student_ (%d).txt", i);

        // START 1: load student file into buffer and close file
        FILE *f = fopen(filename, "r");
        if (f == NULL) {
            printf("File failed to load. Exiting...\n");
            return -1;
        }
        fseek(f, 0, SEEK_END);
        int file_size = ftell(f);
        rewind(f);

        char buffer[file_size];
        fread(buffer, 1, file_size, f);
        fclose(f);
        char *end_address = &buffer[strlen(buffer) - 1];
        // END 1

        char *delimiter = "\n";
        int absent = 0;
        char *token = strtok(buffer, delimiter);
        while (token + 100 < end_address) {

            token = strtok(NULL, delimiter);
            char date[15] = {}, name[30] = {}, assignment[2000] = {}, tiny_string[] = " ";
            strcpy(date, token);
            token = strtok(NULL, delimiter);
            strcpy(name, token);
            token = strtok(NULL, delimiter);

            strcpy(assignment, token);
            token = strtok(NULL, delimiter);
            while (!(strcmp(token, "Present") == 0 || strcmp(token, "Prsnt") == 0 || strcmp(token, "Absent") == 0)) {
                strcat(assignment, tiny_string);
                strcat(assignment, token);
                token = strtok(NULL, delimiter);
            }
            char attendance[10];
            strcpy(attendance, token);
            token = strtok(NULL, delimiter);
            if (strcmp(attendance, "Absent") == 0) continue;

            // START 2: open sql file, load queries from buffer, and execute file
            FILE *sql = fopen("queries.sql", "a");
            char *query = (char *)malloc(2200 * sizeof(char));
            reformat_date(date);
            sprintf(query, "INSERT INTO assignments (date, name, assignment) VALUES (\"%s\", \"%s\", \"%s\");\n", date, name, assignment);
            fwrite(query, 1, strlen(query), sql);
            fclose(sql);
            free(query);
            // END 2

            printf("%s: %d -- ", name, end_address);
            printf("%d\n", token);
        }
        char delete[60] = {};
        sprintf(delete, "rm -f %s", filename);
        system(delete);
    }
    char choice;
    printf("Load into database? [Y/N]: ");
    scanf(" %c", &choice);
    if (toupper(choice) == 'Y') {
        system("sqlite3 studentlog.db < queries.sql");
    }
    return 0;
}