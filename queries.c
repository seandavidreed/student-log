/* 
Writes queries to seed new SQLITE database for 
studentlog4.0 -- only needed once! Can be removed
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "logtools.h"
#include "queries.h"

int str_to_int(char *number) {
    int i = -1;
    int buffer[10];
    int result = 0;
    while (number[++i] != '\0') {
        buffer[i] = (int)(number[i] - '0');
    }
    for (int j = 0, k = i; j < k; j++) {
        result += buffer[j] * pow(10, --i);
    }
    return result;
}

void reformat_date(char *date) {
    char month[3] = {}, day[3] = {}, year[5] = {};
    int i = -1, j = 0, k = 0, l = 0, count = 0;
    while (date[++i] != '\0') {
        if (date[i] == '/') {
            count++;
            continue;
        }
            
        switch (count) {
            case 0:
                month[j++] = date[i];
                break;
            case 1:
                day[k++] = date[i];
                break;
            case 2:
                year[l++] = date[i];
        }
        month[j] = '\0';
        day[k] = '\0';
        year[l] = '\0';
    }
    sprintf(date, "%d-%.2d-%.2d", str_to_int(year), str_to_int(month), str_to_int(day));
}

int write_SQL_queries(node *head) {

    if (head == NULL) {
        printf("List empty. Canceling...");
        return 0;
    }

    FILE *sql = fopen("queries.sql", "a");
    if (sql == NULL) {
        printf("Error opening. Canceling...");
        fclose(sql);
        return -1;
    }

    while (head) {
        char *query = (char *)calloc(200, sizeof(char));
        char temp1[50], temp2[50], temp3[50];

        strcpy(temp1, head->student);
        temp1[strlen(head->student) - 1] = '\0';

        strcpy(temp2, head->lessonTime);
        temp2[strlen(head->lessonTime) - 1] = '\0';

        strcpy(temp3, head->dateStart);
        temp3[strlen(head->dateStart) - 1] = '\0';
        reformat_date(temp3);

        sprintf(query, "INSERT INTO student_base (name, instrument, time, start_date) VALUES ('%s', 'Guitar', '%s', '%s');\n",
                temp1, temp2, temp3);
        fwrite(query, 1, strlen(query), sql);
        free(query);
        head = head->next;
    }

    fclose(sql);
    return 0;
}
