#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "logtools.h"
#include "queries.h" // can be removed: only used to seed new SQLITE database

// Set path at which to store and retrieve student files
#define PATH "C:\\Users\\seanr\\Documents\\programming_projects\\studentlog3.0\\students\\"
// Set MAX number of chars for "assign" member of struct
#define MAX 1000
// Set line length before text wraps for "assign" member of struct
#define WRAP 50

void displayExpand(node *head) {
	// If list is empty, return
	if (head == NULL) {
		return;
	}
	while (head) {
		printf("Name:        %s"\
				 	 "Start Date:  %s"\
					 "Attendance:  %s"\
					 "Lesson Time: %s"\
				 	 "Assigned:    %s\n\n", head->student,
				 	 head->dateStart, head->attend,
					 head->lessonTime, head->assign);
		head = head->next;
	}

	printf("Press ENTER to continue.");
	clear_stdin();
	system("cls");
}

// Fully commented
void selectStudent(node *head, char date[]) {
	// If list is empty, return
	if (head == NULL) {
		return;
	}
	// Set variables
	int n = -1;
	char buffer[MAX] = {};
	char ans = 'A';

	// Prompt user for student number (n)
	printf("Select a Student [Number]: ");
	scanf("%d", &n);
	clear_stdin();
	while (n < 1) {
		printf("Invalid Input.\n");
		printf("Select a Student [Number]: ");
		scanf("%d", &n);
		clear_stdin();
	}
	/*
	Iterate through list n times and throw error if bound exceeded
	Otherwise, stop at selected student
	*/
	if (n > 1) {
		for (int i = 0; i < n - 1; i++) {
			head = head->next;
			if (head == NULL) {
				printf("No student at that location.\n");
				return;
			}
		}
	}

	// Display Student Information
	system("cls");
	printf("Name:        %s"\
			 	 "Start Date:  %s"\
				 "Attendance:  %s"\
				 "Lesson Time: %s"\
			 	 "Assigned:    %s", head->student,
			 	 head->dateStart, head->attend,
				 head->lessonTime, head->assign);

	// Open menu of options and store selection in ans
	ans = studentMenu();
	switch (ans) {
		// User wants to assign homework
		case 'A':
			// Clear assign array to prepare for new entry
			for (int i = 0; i < MAX; i++) {
				(head->assign)[i] = '\0';
			}
			printf("Enter Assignment: ");
			fgets(buffer, MAX, stdin);
			/*
			Load string from buffer into struct
			Format the string to wrap at 50 chars
			*/
			strcpy(head->assign, textWrap(buffer, WRAP));
			// If homework assigned, set attendance to Present
			strcpy(head->attend, "Present\n");
			break;
		case 'S':
			printf("Student Present? [Y/N]: ");
			scanf("%c", &ans);
			ans = toupper(ans);
			clear_stdin();
			if (ans == 'Y') {
				// Clear array of previous input
				for (int i = 0; i < 8; i++) {
					(head->attend)[i] = '\0';
				}
				strcpy(head->attend, "Present\n");
			}
		 	else {
				// Clear array of previous input
				for (int i = 0; i < 8; i++) {
					(head->attend)[i] = '\0';
				}
				strcpy(head->attend, "Absent\n");
			}
			break;
		case 'O': ;
		// This option so far works on Windows and Linux
		char shellCMD[150] = {"notepad.exe '"PATH""};
		strcat(shellCMD, head->student);
		int i = 0;
		while (shellCMD[i] != '\n') i++;
		shellCMD[i] = '\0';
		strcat(shellCMD, ".txt");
		strcat(shellCMD, "'");
		system(shellCMD);
			break;
		case 'T':
			printf("Set Lesson Time [10:00am]: ");
			// Clear array of previous input
			for (int i = 0; i < 10; i++) {
				(head->lessonTime)[i] = '\0';
			}
			fgets(head->lessonTime, 10, stdin);
			break;
		case 'E':
			export_1_student(head, head->student, date);
			break;
		case 'R':
			break;
	}
	system("cls");
}

// Fully commented
node *createStudent(char date[]) {
	// Allocates memory for a node
	node *n = malloc(sizeof(node));
	char c;
	// Prompt user for student's name
	printf("Add a Student: ");
	fgets(n->student, 50, stdin);
	// Prompt user for date: manual input or ctime function
	printf("Manual Start Date? [Y/N]: ");
	scanf("\n%c", &c);
	c = toupper(c);
	// flush standard input (clear input buffer)
	clear_stdin();
	if (c == 'Y') {
		printf("Enter Date: ");
		fgets(n->dateStart, 30, stdin);
	}
	else {
		strcpy(n->dateStart, date);
	}
	printf("Set Lesson Time [10:00am]: ");
	fgets(n->lessonTime, 10, stdin);

	// Set default values for struct members
	strcpy(n->assign, "Unassigned\n");
	strcpy(n->attend, "Not Set\n");
	n->next = NULL;
	return n;
}

void insertStudent(node **h_ref, node *t) {
	// If list is empty, place student at the head by default
	if (*h_ref == NULL) {
		*h_ref = t;
		return;
	}
	// Prompt the user for placement
	int place = 0;
	node *current = *h_ref;
	while (1) {
		printf("Select place in the list: ");
		scanf("%d", &place);
		if (place <= 0) {
			printf("Only positive integers!\n");
		}
		else break;
	}
	// Place student at the head of the list
	if (place == 1) {
		t->next = current;
		*h_ref = t;
		return;
	}
	// Place student somewhere else in the list
	while (place - 2) {
		if (current->next == NULL) {
			current->next = t;
			return;
		}
		current = current->next;
		place--;
	}
	t->next = current->next;
	current->next = t;
}
// Fully commented
void removeStudent(node **h_ref, char date[]) {
	// Error handling
	if (*h_ref == NULL) {
		return;
	}
	// Receives date and formats it to endDate
	char endDate[20] = "End Date: ";
	strcat(endDate, date);
	/* Dereference reference to head and store address in current,
	 	 Get student name from user, reformat and store in path */
	node *current = *h_ref;
	char name[50] = {};
	char path[150] = {PATH};
	printf("Remove a Student [Enter Full Name]: ");
	fgets(name, 50, stdin);
	strcat(path, name);
	int i = 0;
	while (path[i] != '\n') i++;
	path[i] = '\0';
	strcat(path, ".txt");
	// If student to be removed is at head of the list
	if (strcmp(name, (*h_ref)->student) == 0) {
		// Remove student from linked list
		(*h_ref) = (*h_ref)->next;
		system("cls");
		printf("Student Removed From Schedule.\nEnd Date Appended.\n");
		free(current);
		// Append end date to student file
		FILE *f = fopen(path, "a");
		if (f == NULL) {
			printf("Error opening file.\n");
			return;
		}
		fwrite(endDate, sizeof(endDate), 1, f);
		fclose(f);
		return;
	}
	// If student to be removed is later in the list
	while (current->next) {
		if (strcmp(name, current->next->student) == 0) {
			// Remove student from linked list
			node *tmp = current->next;
			current->next = current->next->next;
			system("cls");
			printf("Student removed From Schedule.\nEnd Date Appended.\n");
			free(tmp);
			// Append end date to student file
			FILE *f = fopen(path, "a");
			if (f == NULL) {
				printf("Error opening file.\n");
				return;
			}
			fwrite(endDate, sizeof(endDate), 1, f);
			fclose(f);
			return;
		}
		current = current->next;
	}
	printf("Student Not Found.\n");
}

void writeRoster(node *head, int day) {
	char *filename[6] = {"tuesday.bin", "wednesday.bin", "thursday.bin",
											 "friday.bin", "saturday.bin", "test.bin"};
	node *temp = head;
	FILE *f = fopen(filename[day - 1], "wb");
	while (temp) {
		fwrite(temp, sizeof(node), 1, f);
		temp = temp->next;
	}
	system("cls");
	printf("Roster Updated.\n");
	fclose(f);
}

node *readRoster(int day) {
	char *filename[6] = {"tuesday.bin", "wednesday.bin", "thursday.bin",
											 "friday.bin", "saturday.bin", "test.bin"};
	node *temp = malloc(sizeof(node));
	node *current = malloc(sizeof(node));
	node *head = current;
	FILE *f = fopen(filename[day - 1], "rb");
	if (f == NULL) {
		printf("No Roster Found.\n");
		exit(1);
	}
	while (fread(temp, sizeof(node), 1, f)) {
		strcpy(current->student, temp->student);
		strcpy(current->dateStart, temp->dateStart);
		strcpy(current->lessonTime, temp->lessonTime);
		strcpy(current->assign, temp->assign);
		strcpy(current->attend, temp->attend);

		if (temp->next == NULL) {
			current->next = NULL;
		}
		else {
			current->next = malloc(sizeof(node));
			current = current->next;
		}
	}
	fclose(f);
	free(temp);
	return head;
}

void deleteList(node **href) {
	if (*href == NULL) return;
	node *n = *href;
	while (*href) {
		*href = (*href)->next;
		free(n);
		n = *href;
	}
}
// Fully commented
void exportStudents(node *head, char date[]) {
	// Error handling
	if (head == NULL) {
		printf("List is Empty. File(s) not exported.\n");
		return;
	}
	// Initialize buffer for serialization
	char buffer[MAX * 2] = {};
	// Iterates through linked list until null pointer is found
	while (head) {
		char filename[50] = {};
		char path[150] = {PATH};

		/* Build filename from student name, replace '\n' with '\0',
		 	 Concatenate path and filename into path variable */
		strcpy(filename, head->student);
		int i = 0;
		while (filename[i] != '\n') i++;
		filename[i] = '\0';
		strcat(filename, ".txt");
		strcat(path, filename);

		// Open or create student file for appending
		FILE *f = fopen(path, "a");
		// Error handling
		if (f == NULL) {
			printf("%s failed to load!\n", filename);
			return;
		}
		// If new student, write start date at top of file
		fseek(f, 0, SEEK_END);
		int d = ftell(f);
		if (d == 0) {
			char header[40] = {"Start Date: "};
			strcat(header, head->dateStart);
			fwrite(header, strlen(header), 1, f);
		}
		// Composite data and store in buffer
		sprintf(buffer, "%s%s%s%s%s",
					  date, head->student,
						head->assign, head->attend,
						"------------------------------\n");
		// Write from buffer to file
		fwrite(buffer, strlen(buffer), 1, f);
		fclose(f);
		// clear input buffer to prepare for next node
		for (int i = 0; i < (MAX * 2); i++) {
			buffer[i] = '\0';
		}
		head = head->next;
	}
	system("cls");
	printf("Students Exported.\n");
}

void export_1_student(node *head, char name[], char date[]) {
	// Error handling
	if (head == NULL) {
		printf("List is Empty. File not exported.\n");
		return;
	}
	// Initialize buffer for serialization
	char buffer[MAX * 2] = {};
	// Iterates through linked list until null pointer is found
	while (head) {
		if (strcmp(name, head->student) == 0) {
			char filename[50] = {};
			char path[150] = {PATH};

			/* Build filename from student name, replace '\n' with '\0',
			 	 Concatenate path and filename into path variable */
			strcpy(filename, head->student);
			int i = 0;
			while (filename[i] != '\n') i++;
			filename[i] = '\0';
			strcat(filename, ".txt");
			strcat(path, filename);

			// Open or create student file for appending
			FILE *f = fopen(path, "a");
			// Error handling
			if (f == NULL) {
				printf("%s failed to load!\n", filename);
				return;
			}
			// If new student, write start date at top of file
			fseek(f, 0, SEEK_END);
			int d = ftell(f);
			if (d == 0) {
				char header[40] = {"Start Date: "};
				strcat(header, head->dateStart);
				fwrite(header, strlen(header), 1, f);
			}
			// Composite data and store in buffer
			sprintf(buffer, "%s%s%s%s%s",
						  date, head->student,
							head->assign, head->attend,
							"------------------------------\n");
			// Write from buffer to file
			fwrite(buffer, strlen(buffer), 1, f);
			fclose(f);
			// clear input buffer to prepare for next node
			for (int i = 0; i < (MAX * 2); i++) {
				buffer[i] = '\0';
			}
			system("cls");
			printf("Student Exported.\n");
			return;
		}
		head = head->next;
	}
}
// Fully commented
void clear_stdin() {
	// Clears any remaining inputs in the stdin buffer
	while (1) {
		int c = getc(stdin);
		if (c == EOF || c == '\n') break;
	}
}

char studentMenu() {
	char c;
	printf("------------------------------\n");
	printf("A - Assign Homework\n"\
				 "S - Set Attendance\n"\
				 "O - Open Student File\n"\
				 "T - Set Lesson Time\n"\
				 "E - Export Student\n"\
				 "R - Return to Main Menu\n\n");
	printf("Enter Choice: ");
	scanf("\n%c", &c);
	c = toupper(c);
	clear_stdin();
	return c;
}

char *textWrap(char str[], int wrap) {
	int i = 0;
	bool b = 0;
	char temp[MAX];
	while (str[i]) {
		if (i % wrap == 0 && i != 0) {
			b = 1;
		}
		if (str[i] == ' ' && b == 1) {
			temp[i] = '\n';
			str[i] = '\0';
			i++;
			b = 0;
		}
		else {
			temp[i] = str[i];
			str[i] = '\0';
			i++;
		}
	}
	// Format as in fgets() & copy to str
	temp[i++] = '\n';
	temp[i] = '\0';
	strcpy(str, temp);
	return str;
}

void currDate(char date[]) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(date, "%02d-%02d-%02d\n",  tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);
	date[11] = '\0';
}

// BACK DOOR FOR WHEN I NEED TO UPDATE THE STRUCT NODE
void transfer(node_n *head_n, node **href) {
	node *new = malloc(sizeof(node));
	*href = new;
	while (head_n) {
		strcpy(new->student, head_n->student);
		strcpy(new->dateStart, head_n->dateStart);
		strcpy(new->assign, head_n->assign);
		strcpy(new->attend, head_n->attend);
		strcpy(new->lessonTime, "Not Set\n");

		if (head_n->next == NULL) {
			new->next = NULL;
		}
		else {
			new->next = malloc(sizeof(node));
			new = new->next;
		}
		head_n = head_n->next;
	}
}

node_n *readRoster_n(int day) {
	char *filename[6] = {"tuesday.bin", "wednesday.bin", "thursday.bin",
											 "friday.bin", "saturday.bin", "test.bin"};
	node_n *buffer = malloc(sizeof(node_n));
	node_n *current = malloc(sizeof(node_n));
	node_n *head_n = current;
	FILE *f = fopen(filename[day - 1], "rb");
	if (f == NULL) {
		printf("No Roster Found.\n");
		exit(1);
	}
	while (fread(buffer, sizeof(node_n), 1, f)) {
		strcpy(current->student, buffer->student);
		strcpy(current->dateStart, buffer->dateStart);
		strcpy(current->assign, buffer->assign);
		strcpy(current->attend, buffer->attend);

		if (buffer->next == NULL) {
			current->next = NULL;
		}
		else {
			current->next = malloc(sizeof(node_n));
			current = current->next;
		}
	}
	fclose(f);
	free(buffer);
	return head_n;
}

void deleteList_n(node_n **href) {
	if (*href == NULL) return;
	node_n *n = *href;
	while (*href) {
		*href = (*href)->next;
		free(n);
		n = *href;
	}
}
