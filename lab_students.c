/********************************************************
 *                                                      *
 *      			Dimitris Greasidis                  *
 *                                                      *
 *				Email: temeteron@gmail.com				*
 *														*
 *      		Github Username: Temeteron         		*
 *                                                      *
 *      			Sections with Labs       			*
 *      		  	   && students           			*
 *            			                       			*
 *                                                      *
 ********************************************************/
#include "libraries.h"

enum SWITCH debug; /* global variable */

	/* Structs & Enum */

/* struct with info of student */
typedef struct student {
	char *name;						/*	   		name of student         */
	int num_section;				/*	  	   number of section        */
	struct student *nxt;			/*		  pointers to previous  	*/
	struct student *prv;			/*	 		and next nodes     		*/
} studentT;

enum DAY {Monday, Tuesday, Wednesday, Thursday, Friday}; /* enumeration of days, monday to friday */

/* struct with lab's info */
typedef struct section {
	struct student *head;			/*	pointer to the head of student's list */
	int time_start;					/*			   start time of lab		  */
	enum DAY day_of_week;			/*				  lab's day				  */
} sectionT;

enum SWITCH {ON, OFF}; /* enumeration for on & off */


	/* Prototypes of Functions */

sectionT* init();
studentT* list_init(studentT *root);
void sorted_add_student(sectionT *info_section, studentT *newnode, int number_section);
void print_sections(sectionT *array_of_sections, int number_of_sections);
void funct_free(int num_sections, sectionT *array_section);
studentT* find_delete_student(char *newname, sectionT *array_section, int num_section);
void switch_students(char *first_name, char *second_name, sectionT *array_section, int number_section);
void remove_student(char *name, sectionT *array_section, int num_sections);

/**********
 *  main  *
 **********/
int main (int argc, char *argv[]) {
	/* check to print messages or not */
	if (argc == 1) {
		debug = OFF;
	}
	else if (argc == 2) {
		if (!strcmp(argv[1], "-d") || !strcmp(argv[1],"-debug")) {
			debug = ON;
		}
		else {
			if (argv[1][0] == '-') {
				printf("%s: illegal option -- %s\n", argv[0], argv[1] + 1);
			}
			else {
				printf("%s: illegal option -- %s\n", argv[0], argv[1]);
			}
			printf("Usage: %s [-d]\n", argv[0]);
			return 1;
		}
	}
	else {
		printf("%s: wrong number of arguments %d\n", argv[0], argc);
		return 1;
	}
	/* variables */
	sectionT *array_section; 							/* array of labs's struct */
	int num_sections;								    /* number of labs to create */
	char format[13], format1[13];   						/* 	  format fo sprintf	       */ 
	char action[SIZE_ACTION], first_name[SIZE_NAME], second_name[SIZE_NAME]; 	/* string for commands of user and name of student */


	array_section = init(&num_sections);		/* init list */	
	print_sections(array_section, num_sections);	/* print labs */

	sprintf(format, STRING, SIZE_ACTION - 1);	/*	sprintf to read	*/
	sprintf(format1, STRING, SIZE_NAME - 1);	/*	  string		*/

	/* loop for commands of user */
	while (1) {
		printf("Enter action: ");
		scanf(format, action);

		/* read 2 names of students and swap them labs */
		if (strcmp("SWAP", action) == 0) {
			printf("Enter first name: ");
			scanf(format1, first_name);
			
			printf("Enter second name: ");
			scanf(format1, second_name);

			switch_students(first_name, second_name, array_section, num_sections);
		}
		/* read name of student and delete from lab permantly */
		else if (strcmp("REMOVE", action) == 0) {
			printf("Enter name: ");
			scanf(format1, first_name);
			/* call function to delete student */
			remove_student(first_name, array_section, num_sections);
		}
		/* print labs and students */
		else if (strcmp("PRINT", action) == 0) {
			print_sections(array_section, num_sections);
		}

		else if (strcmp("EXIT", action) == 0) {
			funct_free(num_sections, array_section);	/* free memory */
			return 1;
		}
		/* Available commands */
		else {
			printf("Invalid action. Valid actions are SWAP, REMOVE, PRINT, EXIT.\n");
		}
	}
	
	funct_free(num_sections, array_section);			/* free memory */
	return 0;
}

	/**************
	 *  Functions *
	 **************/

/* Function malloc */
studentT* list_init(studentT *root) {
	root = (studentT *) malloc(sizeof(studentT));
	
	return root;
}

/* Function free memory */
void funct_free(int num_sections, sectionT *array_section) {
	studentT *curr;	/* current node of list */
	int i;		/*    counter of loop    */
	
	for (i = 0; i < num_sections; i++) {
		curr = array_section[i].head; /* current node becomes head of list */
		/* find last node of list */
		while (curr->nxt != NULL) {
			curr = curr->nxt; 
		}

		while (curr != array_section[i].head) {
			curr = curr->prv;
			free (curr->nxt);
		}

		free (array_section[i].head);
	}
	/* free memory */
	free (array_section);
}

/* Function */
sectionT* init(int *number_of_sections) {
	int num_students, num_computers;
	sectionT *info_section;
	studentT *node;
	int i, j, k;
	char format[13], help_string[SIZE_NAME];

	printf("Enter number of students: ");
	scanf("%d", &num_students);
	
	printf("Enter maximum lab size: ");
	scanf("%d", &num_computers);

	/* calculate number of labs */
	*number_of_sections = num_students / num_computers;
	if (num_students % num_computers) {
		(*number_of_sections)++;
	}

	/* malloc for labs */
	info_section = (sectionT *) malloc((*number_of_sections) * sizeof(sectionT));


	if (info_section == NULL) {
		printf("No memory\n");
		return NULL;
	}


	i = 0;
	j = 0;
	k = 0;
	while (i < *number_of_sections) {
		if (TIME_START + k < TIME_END) {
			info_section[i].head = NULL;
			info_section[i].time_start = TIME_START + k;
			info_section[i].day_of_week = Monday + j;
			k++;
		}
		/* go to next cause there are is no room in the current day */
		else {
			/* extreme case */
			if (info_section[i - 1].day_of_week == Friday) {
				break;
			}
			info_section[i].head = NULL;
			info_section[i].day_of_week = info_section[i - 1].day_of_week++;
			j++;
			k = 0;
		}
		i++;
	}


	if (debug == ON) {
		printf("\n>>Created %d lab sections<<\n", i);
	}

	sprintf(format, STRING, SIZE_NAME - 1);


	for (i = 0; i < *number_of_sections; i++) {
		info_section[i].head = NULL;
	}

	i = 0;
	j = 0;
	/* create nodes for the students */
	while (j < num_students) {
		printf("Enter student name: ");
		scanf(format, help_string);
		/* mallloc */
		node = list_init(node);
		if (node == NULL) {
			printf("No memory");
			funct_free(j, info_section);
			return NULL;
		}

		if (i == *number_of_sections) {
			i = 0;
		}

		node->name = strdup(help_string);
		node->num_section = i;
		node->nxt = NULL;
		node->prv = NULL;

		sorted_add_student(info_section, node, i);
		i++;
		j++;
	}

	return info_section;
}

/* Function */
void sorted_add_student(sectionT *info_section, studentT *newnode, int number_section) {
	studentT *crnt;		/* current node in list */


	if (debug == ON) {
		printf("\n>>Adding student \"%s\" in section %d<<\n", newnode->name, number_section);
	}

	/* insert node of student in list */
	/* check list cases */
	if (info_section[number_section].head == NULL) {
		info_section[number_section].head = newnode;
		info_section[number_section].head->nxt = NULL;
		info_section[number_section].head->prv = NULL;
	}
	/* empty list */
	else {
		/* current node becomes node */
		crnt = info_section[number_section].head;
		
		while (1) {
			if (crnt->nxt == NULL || strcmp(crnt->name, newnode->name) > 0) {
				break;
			}
			crnt = crnt->nxt;
		}

		/* check which case we are */
		if (crnt->nxt == NULL) {
			/* proper add to the list */
			if (strcmp(crnt->name, newnode->name) > 0) {
				newnode->nxt = crnt;
				newnode->prv = crnt->prv;
				if (crnt->prv != NULL) {
					crnt->prv->nxt = newnode;
				}
				else {
					info_section[number_section].head = newnode;
				}
				crnt->prv = newnode;
			}
			/* add node at the end of the list */
			else {
				crnt->nxt = newnode;
				newnode->prv = crnt;
			}
		}
		/* add node before the next bigger */
		else {
			if (info_section[number_section].head == crnt) {
				newnode->prv = NULL;
				info_section[number_section].head = newnode;
			}
			else {
				newnode->prv = crnt->prv;
				crnt->prv->nxt = newnode;
			}
			newnode->nxt = crnt;
			crnt->prv = newnode;
		}
	}
}

/* Function */
void print_sections(sectionT *array_of_sections, int number_of_sections) {
	int i;
	studentT *curr;

	printf("\n==========LABS==========\n");
	
	for (i = 0; i < number_of_sections; i++) {
		printf("\n*Section %d,", i);
		switch (array_of_sections[i].day_of_week) {
			case Monday:
				printf(" Monday");
				break;
			case Tuesday:
				printf(" Tuesday");
				break;
			case Wednesday:
				printf(" Wednesday");
				break;
			case Thursday:
				printf(" Thursday");
				break;
			case Friday:
				printf(" Friday");
				break;
		}
		printf(" %d:00*\n", array_of_sections[i].time_start);
		curr = array_of_sections[i].head;
		while (curr->nxt != NULL) {
			printf("\t%s", curr->name);
			curr = curr->nxt;
		}
		printf("\t%s", curr->name);
	}
	printf("\n========================\n");
}

/* Function */
studentT* find_delete_student(char *newname, sectionT *array_section, int num_section) {
	studentT *curr;
	int i;
	
	for (i = 0; i < num_section; i++) {

		if (debug == ON) {
			printf("\n>>Searching section %d...<<\n", i);
		}
		
		for (curr = array_section[i].head; (curr != NULL); curr = curr->nxt) {

			if ((debug == ON) && (strcmp(newname, curr->name) >= 0)) {
				printf("%s...", curr->name);
			}

			if (strcmp(curr->name, newname) == 0) {
				break;
			}
		}
		/* check if we found the given name in the labs */
		if ((curr != NULL) && (strcmp(curr->name, newname) == 0)) {
			if ((curr->nxt == NULL) && (curr->prv == NULL)) {
				array_section[i].head = NULL;
			}
			else if (curr->nxt == NULL) {
				curr->prv->nxt = curr->nxt;
			}
			else if (curr->prv == NULL) {
				curr->nxt->prv = curr->prv;
				array_section[i].head = curr->nxt;
			}
			else {
				curr->prv->nxt = curr->nxt;
				curr->nxt->prv = curr->prv;
			}
			curr->nxt = NULL;
			curr->prv = NULL;
			break;
		}

		if (i + 1 == num_section) {
			return NULL;
		}
	}


	return curr;
}

/* Function */
void switch_students(char *first_name, char *second_name, sectionT *array_section, int number_section) {
	studentT *student1, *student2;
	int help_num_student;
	
	student1 = find_delete_student(first_name, array_section, number_section);
	student2 = find_delete_student(second_name, array_section, number_section);
	if (student1 == NULL) {
		printf("Student \"%s\" not in lab. Swap canceled.\n", first_name);
		if (student2 != NULL) {
			/* add student back to his lab cause the second student wasnt found */
			sorted_add_student(array_section, student2, student2->num_section);
		}
		return;
	}
	
	if (student2 == NULL) {
		printf("Student \"%s\" not in lab. Swap canceled.\n", second_name);
		/* add student back to his lab cause the second student wasnt found */
		sorted_add_student(array_section, student1, student1->num_section);
		return;
	}

	if (student1->num_section == student2->num_section) {
		printf("Students \"%s\" and \"%s\" are in the same section. Swap canceled.\n", first_name, second_name);
		/* add student back to his lab cause both students are in the same lab */
		sorted_add_student(array_section, student1, student1->num_section);
		sorted_add_student(array_section, student2, student2->num_section);
		return;
	}

	/* add students to new labs */
	sorted_add_student(array_section, student2, student1->num_section);	/* add student2 to student1's lab */
	sorted_add_student(array_section, student1, student2->num_section);	/* add student1 to student2's lab */

	help_num_student = student2->num_section;
	student2->num_section = student1->num_section;
	student1->num_section = help_num_student;

}

/* Function */
void remove_student(char *name, sectionT *array_section, int num_sections) {
	studentT *curr;


	curr = find_delete_student(name, array_section, num_sections);

	if (debug == ON) {
		printf("\n>>Removed student \"%s\" from section %d<<\n", curr->name, curr->num_section);
	}
	
	if (curr == NULL) {
		printf("Student \"%s\" not in lab. Remove canceled.\n", name);
	}
	else {

		free(curr);
	}
}

