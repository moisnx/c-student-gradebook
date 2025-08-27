#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENTS_FILE "students.dat"
#define BUFFER_SIZE 256
#define MAX_GRADES 10

// Next ID
int nextID = 1;

void cleanScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

typedef struct Student {
    int id;
    char name[60];
    int grades[10];
    int grade_count;  // Tracks how many grades are stored

} Student;

typedef struct StudentsList {
    Student *students;
    size_t size;
    size_t capacity;
} StudentsList;

void loadStudentsFromFile(StudentsList *list, const char *filename);
void updateNextID(StudentsList *list);
void listStudents(StudentsList *list);
void addNewStudent(StudentsList *list, const char *filename);
void removeStudent(StudentsList *list, const char *filename);
void addNewGrade(StudentsList *list, const char *filename);
void calculateStudentAverage(StudentsList *list);
float calculateAverage(Student Student);
void addInternalStudent(StudentsList *list, Student newStudent);
void saveToFile(StudentsList *list, const char *filename);
Student *getStudentById(StudentsList *list, const int id);

int main(void) {
    int n;

    StudentsList myList;
    myList.size = 0;
    myList.capacity = 2;
    myList.students = malloc(myList.capacity * sizeof(Student));

    if (myList.students == NULL) {
        printf("[!] Error allocating memory.");
        exit(1);
    }

    loadStudentsFromFile(&myList, STUDENTS_FILE);

    do {
        cleanScreen();
        printf(
            "\n\t--------------------------------------------------------------"
            "-\n");
        printf("\tStudent Gradebook Progeam\n");
        printf(
            "\t---------------------------------------------------------------"
            "\n");
        printf("\t[1] -> List All Students\n");
        printf("\t[2] -> Add new student\n");
        printf("\t[3] -> Add grade\n");
        printf("\t[4] -> Calculate average\n");
        printf("\t[5] -> Remove Student\n");
        printf("\t[0] -> Quit\n");
        printf(
            "\t---------------------------------------------------------------"
            "\n");
        printf("\tEnter your choice: ");
        scanf("%i", &n);
        while (getchar() != '\n');

        switch (n) {
            case 1:
                printf("\n");
                printf("\t╔══════════════════════════════════════╗\n");
                printf("\t║           List of Students           ║\n");
                printf("\t╚══════════════════════════════════════╝\n");
                listStudents(&myList);
                printf("\n\tPress Key to contiue...");
                getchar();
                break;
            case 2:
                printf("\n");
                printf("\t╔══════════════════════════════════════╗\n");
                printf("\t║            Add New Student           ║\n");
                printf("\t╚══════════════════════════════════════╝\n");
                addNewStudent(&myList, STUDENTS_FILE);
                printf("\n\tPress Key to contiue...");
                getchar();
                break;

            case 3:
                printf("\n");
                printf("\t╔══════════════════════════════════════╗\n");
                printf("\t║            Add New Grade             ║\n");
                printf("\t╚══════════════════════════════════════╝\n");
                addNewGrade(&myList, STUDENTS_FILE);
                printf("\n\tPress Key to contiue...");
                getchar();
                break;
            case 4:
                printf("\n");
                printf("\t╔══════════════════════════════════════╗\n");
                printf("\t║        STUDENT GRADE ANALYSIS        ║\n");
                printf("\t╚══════════════════════════════════════╝\n");
                calculateStudentAverage(&myList);
                printf("\n\tPress Key to contiue...");
                getchar();
                break;
            case 5:
                printf("\n");
                printf("\t╔══════════════════════════════════════╗\n");
                printf("\t║           Remove Student             ║\n");
                printf("\t╚══════════════════════════════════════╝\n");
                removeStudent(&myList, STUDENTS_FILE);
                printf("\n\tPress Key to contiue...");
                getchar();
                break;
            case 0:
                printf("\n\t[!] Bye, bye! Closing the program...\n\n");
                break;
            default:
                printf("\n\t[!] Wrong Option, try again");
                printf("\n\tPress Key to contiue...");
                getchar();
                break;
        }

    } while (n != 0);
}

void updateNextID(StudentsList *list) {
    int maxID = 0;
    for (size_t i = 0; i < list->size; i++) {
        if (list->students[i].id > maxID) {
            maxID = list->students[i].id;
        }
    }
    nextID = maxID + 1;
}

void loadStudentsFromFile(StudentsList *list, const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        // Return nothing just continue
        return;
    }
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        Student newStudent;
        char *id_str = strtok(buffer, "|");
        char *name_str = strtok(NULL, "|");
        char *grades_str = strtok(NULL, "|");
        // char *average_str = strtok(NULL, "|");

        if (id_str == NULL || name_str == NULL || grades_str == NULL) {
            fprintf(stderr, "Warning: Skipping malformed line in file.\n");
            continue;
        }

        newStudent.id = atoi(id_str);
        newStudent.grade_count = atoi(grades_str);
        // int *grades = malloc(newStudent.grade_count * sizeof(int));

        for (size_t i = 0; i < newStudent.grade_count; i++) {
            char *temp = strtok(NULL, "|");
            newStudent.grades[i] = atoi(temp);
        }

        strncpy(newStudent.name, name_str, sizeof(newStudent.name) - 1);
        newStudent.name[sizeof(newStudent.name) - 1] = '\0';
        // *newStudent.grades = grades;

        addInternalStudent(list, newStudent);
        // free(grades);
    }
    updateNextID(list);
}
void listStudents(StudentsList *list) {
    const int MAX_DISPLAY_GRADES = 5;  // Show max 4 grades

    if (list->size == 0) {
        printf("\n\t[INFO] No contacts to show.\n");
        return;
    }
    // Print Header
    printf("\n");
    printf(
        "\t--------------------------------------------------------------------"

        "-------\n");
    printf("\t| %-4s | %-20s | %-25s | %-13s |\n", "ID", "NAME", "Grades",
           "Average Score");
    printf(
        "\t--------------------------------------------------------------------"

        "-------\n");

    // Print Data Rows
    for (size_t i = 0; i < list->size; i++) {
        // Use a temporary pointer/variable for clarity
        Student current_student = list->students[i];
        char grades_str[200] = "";
        char temp[100];

        int grades_to_show = (current_student.grade_count > MAX_DISPLAY_GRADES)
                                 ? MAX_DISPLAY_GRADES
                                 : current_student.grade_count;
        for (int j = 0; j < grades_to_show; j++) {
            sprintf(temp, "%d", current_student.grades[j]);
            strcat(grades_str, temp);

            if (j < grades_to_show - 1) {
                strcat(grades_str, " ");
            }
        }

        // Add "..." if there are more grades
        if (current_student.grade_count > MAX_DISPLAY_GRADES) {
            sprintf(temp, " (+%d more)",
                    current_student.grade_count - MAX_DISPLAY_GRADES);
            strcat(grades_str, temp);
        }

        // Print combined name (First + Last)
        char fullName[41];  // Max 20 + 20 + 1 space = 41
        snprintf(fullName, sizeof(fullName), "%s", current_student.name);

        // Use the width specifiers to align columns:
        // %-4d : Left-align ID in 4 spaces
        // %-20s : Left-align Name in 20 spaces
        // %-15s : Left-align Phone in 15 spaces
        // %-10d : Left-align In/Out counts in 10 spaces

        printf("\t| %-4d | %-20s | %-25s | %-13.1f |\n", current_student.id,
               fullName, grades_str, calculateAverage(current_student));
    }
    printf(
        "\t--------------------------------------------------------------------"

        "-------\n");
}

void addNewStudent(StudentsList *list, const char *filename) {
    Student newStudent;
    newStudent.id = nextID;

    // Clear input buffer
    int c;
    // while ((c = getchar()) != '\n' && c != EOF);

    printf("\n\t>> Student Name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';

    int n = 0;
    printf("\n\t>> How many grades do you want to add? ");
    scanf("%d", &n);
    newStudent.grade_count = n;

    printf("\n\t----------- GRADES -----------");
    for (size_t i = 0; i < newStudent.grade_count; i++) {
        printf("\n\t   Grade #%zu: ", i + 1);
        scanf("%d", &newStudent.grades[i]);

        // Simple grade indicators
        if (newStudent.grades[i] >= 90) {
            printf("\t[A - Excellent]");
        } else if (newStudent.grades[i] >= 80) {
            printf("\t[B - Good]");
        } else if (newStudent.grades[i] >= 70) {
            printf("\t[C - Average]");
        } else if (newStudent.grades[i] >= 60) {
            printf("\t[D - Below Average]");
        } else {
            printf("\t[F - Failing]");
        }
    }

    addInternalStudent(list, newStudent);
    saveToFile(list, filename);
    nextID++;
    printf("\n\n\t*** SUCCESS ***");
    printf("\n\tStudent '%s' has been successfully added!", newStudent.name);
    printf("\n\tAssigned ID: %d | Total Grades: %d", newStudent.id,
           newStudent.grade_count);
    printf("\n\t=====================================\n");
}

void removeStudent(StudentsList *list, const char *filename) {
    int studentId;
    printf("\n\t>> Enter Student ID: ");
    scanf("%d", &studentId);
    while (getchar() != '\n');

    int idx = -1;
    for (int i = 0; i < list->size; i++) {
        if (list->students[i].id == studentId) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("\n\t[!] ERROR: Student with ID %d not found.", studentId);
        printf("\n\t    Please check the ID and try again.");
        return;
    }

    for (size_t i = idx; i < list->size - 1; i++) {
        list->students[i] = list->students[i + 1];
    }
    list->size--;
    printf("\n\n\t*** SUCCESS ***");
    printf("\n\tStudent with id %i was removed.", studentId);
    printf("\n\t=====================================\n");
    saveToFile(list, filename);
}

void addNewGrade(StudentsList *list, const char *filename) {
    // Clear input buffer
    int studentId;
    // while ((studentId = getchar()) != '\n' && studentId != EOF);

    printf("\n\t>> Student ID: ");
    scanf("%d", &studentId);

    while (getchar() != '\n');
    Student *student = getStudentById(list, studentId);

    if (student == NULL) {
        printf("\n\t[!] This student doesnt exists.");
        return;
    }

    int n = 0;
    printf("\n\t>> How many grades do you want to add? ");
    scanf("%d", &n);

    if (student->grade_count + n > 10) {
        printf("\n\t[!] Can't exceed 10 grades!");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    printf("\n\t----------- GRADES -----------");
    for (size_t i = 0; i < n; i++) {
        printf("\n\t   Grade #%zu: ", (size_t)student->grade_count + 1);
        scanf("%i", &student->grades[student->grade_count]);
        // temp.grade_count++;
        // Simple grade indicators
        if (student->grades[i] >= 90) {
            printf("\t[A - Excellent]");
        } else if (student->grades[i] >= 80) {
            printf("\t[B - Good]");
        } else if (student->grades[i] >= 70) {
            printf("\t[C - Average]");
        } else if (student->grades[i] >= 60) {
            printf("\t[D - Below Average]");
        } else {
            printf("\t[F - Failing]");
        }
        student->grade_count++;
    }

    // addInternalStudent(list, temp);
    saveToFile(list, filename);

    printf("\n\n\t*** SUCCESS ***");
    printf("\n\tStudent '%s' has been added %i grades!", student->name,
           student->grade_count);
    printf("\n\t=====================================\n");
    // getchar();
    // fgets(newStudent.name, sizeof(newStudent.name), stdin);
    // newStudent.name[strcspn(newStudent.name, "\n")] = '\0';
}

void saveToFile(StudentsList *list, const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("\n\t[!] File not found, creating a new one.");
    }

    for (size_t i = 0; i < list->size; i++) {
        Student temp = list->students[i];

        fprintf(fp, "%i|%s|%i", temp.id, temp.name, temp.grade_count);

        for (size_t i = 0; i < temp.grade_count; i++) {
            fprintf(fp, "|%i", temp.grades[i]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    // updateNextID(list);
    return;
}

void addInternalStudent(StudentsList *list, Student newStudent) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        Student *temp =
            realloc(list->students, list->capacity * sizeof(Student));

        if (temp == NULL) {
            perror("Error assigning memery");
            return;
        }
        list->students = temp;
    }
    // printf("\n\tContact succesfully added\n");
    // Modify next ID
    list->students[list->size] = newStudent;
    list->size++;
}

Student *getStudentById(StudentsList *list, const int id) {
    for (int i = 0; i < list->size; i++) {
        if (list->students[i].id == id) {
            return &list->students[i];
        }
    }
    return NULL;
}

void calculateStudentAverage(StudentsList *list) {
    int studentId;

    printf("\n\t>> Enter Student ID: ");
    scanf("%d", &studentId);
    while (getchar() != '\n');

    Student *student = getStudentById(list, studentId);

    if (student == NULL) {
        printf("\n\t[!] ERROR: Student with ID %d not found.", studentId);
        printf("\n\t    Please check the ID and try again.");
        return;
    }

    // Check if student has any grades
    if (student->grade_count == 0) {
        printf("\n\t[!] WARNING: Student '%s' has no grades recorded.",
               student->name);
        printf("\n\t    Cannot calculate average without grades.");
        return;
    }

    float average = calculateAverage(*student);

    // Display detailed student information
    printf("\n\t==========================================");
    printf("\n\t  GRADE REPORT FOR: %s", student->name);
    printf("\n\t==========================================");
    printf("\n\t  Student ID: %d", student->id);
    printf("\n\t  Total Grades: %d/10", student->grade_count);

    // Display all grades
    printf("\n\t  Individual Grades: ");
    for (int i = 0; i < student->grade_count; i++) {
        if (i > 0) printf(", ");
        printf("%d", student->grades[i]);
    }

    // Calculate additional statistics
    int highest = student->grades[0];
    int lowest = student->grades[0];
    for (int i = 1; i < student->grade_count; i++) {
        if (student->grades[i] > highest) highest = student->grades[i];
        if (student->grades[i] < lowest) lowest = student->grades[i];
    }

    printf("\n\t------------------------------------------");
    printf("\n\t  STATISTICS:");
    printf("\n\t  Average Grade: %.2f", average);
    printf("\n\t  Highest Grade: %d", highest);
    printf("\n\t  Lowest Grade: %d", lowest);
    printf("\n\t  Grade Range: %d", highest - lowest);

    // Letter grade and performance analysis
    char letterGrade;
    const char *performance;

    if (average >= 90) {
        letterGrade = 'A';
        performance = "Excellent";
    } else if (average >= 80) {
        letterGrade = 'B';
        performance = "Good";
    } else if (average >= 70) {
        letterGrade = 'C';
        performance = "Average";
    } else if (average >= 60) {
        letterGrade = 'D';
        performance = "Below Average";
    } else {
        letterGrade = 'F';
        performance = "Failing";
    }

    printf("\n\t  Letter Grade: %c", letterGrade);
    printf("\n\t  Performance: %s", performance);

    // Progress indicator
    printf("\n\t------------------------------------------");
    printf("\n\t  PROGRESS: [");
    int filled = (int)(average / 10);
    for (int i = 0; i < 10; i++) {
        if (i < filled) {
            printf("█");
        } else {
            printf("░");
        }
    }
    printf("] %.1f%%", average);

    // Improvement suggestions
    if (average < 70) {
        printf("\n\t------------------------------------------");
        printf("\n\t  💡 SUGGESTION: Consider additional study");
        printf("\n\t     time to improve performance.");
    } else if (average < 90) {
        printf("\n\t------------------------------------------");
        printf("\n\t  💡 SUGGESTION: Great work! A little more");
        printf("\n\t     effort could push you to excellence.");
    } else {
        printf("\n\t------------------------------------------");
        printf("\n\t  🏆 OUTSTANDING: Excellent performance!");
        printf("\n\t     Keep up the great work!");
    }

    printf("\n\t==========================================\n");
}

float calculateAverage(Student Student) {
    if (Student.grade_count == 0) {
        return 0;
    }
    float sum = 0;
    for (size_t i = 0; i < Student.grade_count; i++) {
        sum += Student.grades[i];
    }
    return sum / Student.grade_count;
}