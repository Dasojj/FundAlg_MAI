#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
    int id;
    char name[256];
    char surname[256];
    int course;
    char group[256];
    int exams[5];
};

struct student* read_students_from_file(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }
    int student_count;
    fscanf(file, "%d", &student_count);
    struct student* students = (struct student*) malloc(student_count * sizeof(struct student));
    if (!students) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }
    for (int i = 0; i < student_count; i++) {
        fscanf(file, "%d %s %s %d %s", &students[i].id, students[i].name, students[i].surname, &students[i].course, students[i].group);
        for (int j = 0; j < 5; j++) {
            fscanf(file, "%d", &students[i].exams[j]);
        }
    }
    fclose(file);
    return students;
}

int compare_by_id(const void* a, const void* b) {
    return ((struct student*) a)->id - ((struct student*) b)->id;
}

int compare_by_surname(const void* a, const void* b) {
    return strcmp(((struct student*) a)->surname, ((struct student*) b)->surname);
}

int compare_by_name(const void* a, const void* b) {
    return strcmp(((struct student*) a)->name, ((struct student*) b)->name);
}

int compare_by_group(const void* a, const void* b) {
    return strcmp(((struct student*) a)->group, ((struct student*) b)->group);
}

struct student** group_by_course(struct student* students, int student_count) {
    struct student** groups = (struct student**) malloc(4 * sizeof(struct student*));
    if (!groups) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }
    for (int i = 0; i < 4; i++) {
        groups[i] = (struct student*) malloc(sizeof(struct student));
        if (!groups[i]) {
            while(i) free(groups[--i]);
            free(groups);
            fprintf(stderr, "malloc() failed: insufficient memory!\n");
            return NULL;
        }
        int group_size = 0;
        for (int j = 0; j < student_count; j++) {
            if (students[j].course == i + 1) {
                groups[i][group_size++] = students[j];
            }
        }
    }
    return groups;
}

void print_student(struct student s) {
    printf("%s %s, Course: %d, Group: %s\n", s.name, s.surname, s.course, s.group);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please provide filepath.\n");
        return 1;
    }
    struct student* students = read_students_from_file(argv[1]);
    if(students == NULL){
        printf("Error reading students from file\n");
        return -1;
    }
    int student_count = sizeof(students) / sizeof(students[0]);
    // Sort by id
    qsort(students, student_count, sizeof(struct student), compare_by_id);
    // Sort by surname
    qsort(students, student_count, sizeof(struct student), compare_by_surname);
    // Sort by name
    qsort(students, student_count, sizeof(struct student), compare_by_name);
    // Sort by group
    qsort(students, student_count, sizeof(struct student), compare_by_group);
    // Group by course
    struct student** groups = group_by_course(students, student_count);
    if(groups == NULL){
        printf("Error reading students from file\n");
        return -2;
    }
    // Print all students
    for (int i = 0; i < student_count; i++) {
        print_student(students[i]);
    }
    // Print students by group
    for (int i = 0; i < 4; i++) {
        printf("Course %d:\n", i + 1);
        for (int j = 0; groups[i][j].id != 0; j++) {
            print_student(groups[i][j]);
        }
    }
    //Clear students
    free(students);
    //Clear groups
    for (int i = 0; i < 4; i++) free(groups[i]);
    free(groups);
    return 0;
}

