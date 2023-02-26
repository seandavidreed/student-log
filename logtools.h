#ifndef __LOGTOOLS_H_
#define __LOGTOOLS_H_

// CURRENT STRUCT NODE
typedef struct node {
	char student[50];
	char dateStart[30];
	char assign[2000];
	char attend[10];
	char lessonTime[10];
	struct node *next;
} node;

// OLD STRUCT NODE
typedef struct node_n {
	char student[50];
	char dateStart[30];
	char assign[1000];
	char attend[8];
	struct node_n *next;
} node_n;

// function Prototypes
void displayExpand(node *head);
void selectStudent(node *head, char date[]);
node *createStudent(char date[]);
void insertStudent(node **h_ref, node *t);
void removeStudent(node **h_ref, char date[]);
void writeRoster(node *head, int day);
node *readRoster(int day);
void deleteList(node **href);
void exportStudents(node *head, char date[]);
void export_1_student(node *head, char name[], char date[]);
void clear_stdin();
char studentMenu();
char *textWrap(char str[], int wrap);
void currDate(char date[]);

void transfer(node_n *head_n, node **href);
node_n *readRoster_n(int day);
void deleteList_n(node_n **href);

#endif
