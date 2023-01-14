#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

int search_substring(const char* str, const char* substr) {
    int i, j;
    for (i = 0; str[i]; i++) {
        for (j = 0; substr[j]; j++) {
            if (str[i + j] != substr[j]) {
                break;
            }
        }
        if (!substr[j]) {
            return 1;
        }
    }
    return 0;
}

void search_in_files(const char* substr, int file_count, ...) {
    va_list files;
    va_start(files, file_count);
    char* buffer = NULL;
    size_t buffer_size = 0;
    int i;
    for (i = 0; i < file_count; i++) {
        FILE* file = va_arg(files, FILE*);
        printf("Searching in file %d\n", i + 1);
        int line_number = 1;
        while (getline(&buffer, &buffer_size, file) != -1) {
            if (search_substring(buffer, substr)) {
                printf("Found '%s' on line %d\n", substr, line_number);
            }
            line_number++;
        }
        rewind(file);
    }
    va_end(files);
    free(buffer);
}

int main() {
    FILE* file1 = fopen("file1.txt", "r");
    FILE* file2 = fopen("file2.txt", "r");
    search_in_files("example", 2, file1, file2);
    fclose(file1);
    fclose(file2);
    return 0;
}
