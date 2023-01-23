#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100

struct Resident {
    char surname[MAX_NAME_LEN];
    char name[MAX_NAME_LEN];
    char patronymic[MAX_NAME_LEN];
    int birth_year;
    char gender;
    float avg_monthly_income;
    struct Resident *next;
};

void swap(struct Resident* a, struct Resident* b) {
struct Resident temp = *a;
*a = *b;
*b = temp;
}

int count_residents(struct Resident* head) {
    int count = 0;
    struct Resident *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

struct Resident *load_residents_from_file(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        printf("Error opening file: %s\n", file_name);
        return NULL;
    }
    struct Resident *head = (struct Resident *)malloc(sizeof(struct Resident));
    if (!head) {
        printf("Error allocating memory\n");
        fclose(file);
        return NULL;
    }
    fscanf(file, "%s %s %s %d %c %f", head->surname, head->name, head->patronymic, &head->birth_year, &head->gender, &head->avg_monthly_income);
    head->next = NULL;
    struct Resident *tail = head;
    struct Resident *new_resident;
    while (fscanf(file, "%s %s %s %d %c %f", new_resident->surname, new_resident->name, new_resident->patronymic, &new_resident->birth_year, &new_resident->gender, &new_resident->avg_monthly_income) != EOF) {
        new_resident = (struct Resident *)malloc(sizeof(struct Resident));
        if (!new_resident) {
            printf("Error allocating memory\n");
            break;
        }
        tail->next = new_resident;
        tail = new_resident;
        new_resident->next = NULL;
    }
    fclose(file);
    return head;
}

void save_residents_to_file(const char *file_name, struct Resident *head) {
    FILE *file = fopen(file_name, "w");
    if (!file) {
        printf("Error opening file: %s\n", file_name);
        return;
    }
    struct Resident *current = head;
    while (current) {
        fprintf(file, "%s %s %s %d %c %f\n", current->surname, current->name, current->patronymic, current->birth_year, current->gender, current->avg_monthly_income);
        current = current->next;
    }
    fclose(file);
}

struct Resident* sort_residents_by_age(struct Resident* head) {
    int i, j, k;
    struct Resident *p, *q, *top;
    top = (struct Resident *)malloc(sizeof(struct Resident));
    top->next = head;
    k = 0;
    for (i = 1, p = top; i < count_residents(head); i++, p = p->next) {
        for (j = i, q = p; j > 0 && q->next->birth_year < q->birth_year; j--, q = q->next) {
            swap(q->next, q);
            k = 1;
        }
    }
    if (k == 1) {
        head = top->next;
    }
    free(top);
    return (head);
}

void add_resident(struct Resident **head, struct Resident *new_resident) {
    new_resident->next = *head;
    *head = new_resident;
}

struct Resident* search_residents_by_surname(struct Resident* head, char* surname) {
    struct Resident *current = head;
    struct Resident *searched_residents = NULL;
    struct Resident *tail = NULL;

    while (current != NULL) {
        if (strcmp(current->surname, surname) == 0) {
            // Allocate memory for the searched resident
            struct Resident *new_resident = (struct Resident *)malloc(sizeof(struct Resident));
            if (!new_resident) {
                printf("Error allocating memory\n");
                break;
            }
            // Copy the data from current resident to searched resident
            memcpy(new_resident, current, sizeof(struct Resident));
            new_resident->next = NULL;

            // Add the searched resident to the list
            if (!searched_residents) {
                searched_residents = new_resident;
                tail = new_resident;
            } else {
                tail->next = new_resident;
                tail = new_resident;
            }
        }
        current = current->next;
    }
    return searched_residents;
}

void print_residents(struct Resident* head) {
    struct Resident *current = head;
    while (current != NULL) {
        printf("Surname: %s\n", current->surname);
        printf("Name: %s\n", current->name);
        printf("Patronymic: %s\n", current->patronymic);
        printf("Birth year: %d\n", current->birth_year);
        printf("Gender: %c\n", current->gender);
        printf("Average monthly income: %f\n", current->avg_monthly_income);
        current = current->next;
    }
}

int delete_resident(struct Resident **head, char* surname) {
    struct Resident *current = *head;
    struct Resident *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->surname, surname) == 0) {
            if (previous == NULL) {
                // Deleting the first element
                *head = current->next;
            } else {
                // Deleting an element in the middle or end of the list
                previous->next = current->next;
            }
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

void free_residents(struct Resident* head) {
    struct Resident *current = head;
    struct Resident *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main(int argc, char* argv[]) {
    // Load residents from file
    struct Resident *residents = load_residents_from_file();

    // Sort the list of residents by age
    residents = sort_residents_by_age(residents);

    // Search for residents by surname
    char surname_to_search[MAX_NAME_LEN];
    printf("Enter the surname to search for: ");
    scanf("%s", surname_to_search);
    struct Resident *searched_residents = search_residents_by_surname(residents, surname_to_search);
    if (!searched_residents) {
        printf("No residents with the surname %s were found\n", surname_to_search);
    } else {
        printf("Residents with the surname %s:\n", surname_to_search);
        print_residents(searched_residents);
    }

    // Add a new resident to the list
    struct Resident new_resident;
    printf("Enter the new resident's surname: ");
    scanf("%s", new_resident.surname);
    printf("Enter the new resident's name: ");
    scanf("%s", new_resident.name);
    printf("Enter the new resident's patronymic: ");
    scanf("%s", new_resident.patronymic);
    printf("Enter the new resident's birth year: ");
    scanf("%d", &new_resident.birth_year);
    printf("Enter the new resident's gender: ");
    scanf(" %c", &new_resident.gender);
    printf("Enter the new resident's average monthly income: ");
    scanf("%f", &new_resident.avg_monthly_income);
    add_resident(&residents, &new_resident);
    printf("New resident added to the list:\n");
    print_residents(residents);

    // Delete a resident from the list
    char* resident_surname_to_delete;
    printf("Enter the surname of the resident to delete: ");
    scanf("%s", resident_surname_to_delete);
    if (delete_resident(&residents, resident_surname_to_delete)) {
        printf("Resident with surname %s was deleted from the list\n", resident_surname_to_delete);
    } else {
        printf("Resident with surname %s was not found in the list\n", resident_surname_to_delete);
    }

    // Save residents to file
    save_residents_to_file("residents.txt", residents);

    // Free memory
    free_residents(residents);

    return 0;
}
