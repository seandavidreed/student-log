#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "logtools.h"
#include "queries.h" // can be removed: only used to seed new SQLITE database

int main() {
	// Declare variables
	char c = 'A';
	int day;
	bool end = 0;
	node *head = NULL;
	// Initialize date variable with current date
	char date[15];
	currDate(date);
	// Head of old struct node
	node_n *head_n = NULL;
	node *tmp;
	node *n;
	// FULLY COMMENTED
	// Prompt user (this feature might be obsolete)
	printf("Load Roster? [Y/N]: ");
	scanf("\n%c", &c);
	c = toupper(c);
	// flush standard input (clear input buffer)
	clear_stdin();
	// Select which roster to load
	printf("\n1 - Tuesday\n"\
				 "2 - Wednesday\n"\
				 "3 - Thursday\n"\
				 "4 - Friday\n"\
				 "5 - Saturday\n\n");
	printf("Input Number: ");
	scanf("\n%d", &day);
	clear_stdin();
	// Day 6 is for testing purposes
	while (day < 1 || day > 6) {
		printf("Invalid Input.\n");
		printf("Input Number: ");
		scanf("%d", &day);
		clear_stdin();
	}
	if (c == 'Y') {
		head = readRoster(day);
	}
	// Backward compatibility with old struct node
	if (c == 'Z') {
		head_n = readRoster_n(day);
		transfer(head_n, &head);
		deleteList_n(&head_n);
	}
	system("cls");
	// Enter into menu loop
	while (1) {
		// List is unpopulated: display "List is empty."
		if (head == NULL) {
			system("cls");
			printf("------------------------------\n"\
						 "List is empty.\n"\
						 "------------------------------\n");
		}
		// List is populated: display student names
		else {
			n = head;
			int i = 1;
			printf("------------------------------\n");
			while (n) {
				int j = 0;
				// Buffer to remove newline char from n->student
				char buffer[50];
				while ((n->student)[j] != '\n') {
					buffer[j] = n->student[j];
					j++;
				}
				buffer[j] = '\0';
				printf("%d: [%c] %s - %s", i++,
							(n->attend)[0], buffer, n->lessonTime);
				// Clear buffer
				for (j = j - 1; j > 0; j--) buffer[j] = '\0';
				n = n->next;
			}
			printf("------------------------------\n");
		}
		/* Prompt User for menu selection,
			 pass through switch statement to appropriate function */
		printf("D - Display Expanded\n"\
					 "S - Select Student\n"\
					 "A - Add Student\n"\
				 	 "R - Remove Student\n"\
					 "E - Export Students\n"\
					 "U - Update Roster\n"\
				 	 "Q - Quit\n\n");
		printf("Enter Choice: ");
		scanf("\n%c", &c);
		c = toupper(c);
		clear_stdin();
		switch (c) {
			case 'D':
				displayExpand(head);
				break;
			case 'S':
				selectStudent(head, date);
				break;
			case 'A':
				tmp = createStudent(date);
				insertStudent(&head, tmp);
				system("cls");
				break;
			case 'R':
				removeStudent(&head, date);
				break;
			case 'E':
				exportStudents(head, date);
				break;
			case 'U':
				writeRoster(head, day);
				break;
			case '$':
				write_SQL_queries(head); // can be removed: only used to seed new SQLITE database
				break;
			case 'Q':
				printf("Do you want to update the roster? [Y/N]: ");
				scanf("\n%c", &c);
				c = toupper(c);
				clear_stdin();
				if (c == 'Y') {
					writeRoster(head, day);
				}
				deleteList(&head);
				end = 1;
				break;
			default:
				// Error handling
				system("cls");
				printf("Error: Invalid Input.\n");
		}
		if (end == 1) break;
	}
}
