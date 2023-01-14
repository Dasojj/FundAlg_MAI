#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() {
    char str[1000];
    FILE *input;
    if((input = fopen("input.txt", "r"))==NULL){
        printf("File opening failed\n");
        return -1;
    }
    FILE *output;
    if((output = fopen("output.txt", "w"))==NULL){
        printf("File opening failed\n");
        fclose(input);
        return -2;
    }
    while (fscanf(input, "%s", str) != EOF) {
        int base = 2;
        while (base <= 36) {
            char *endptr;
            long long num = strtoll(str, &endptr, base);
            if (str == endptr) {
                base++;
                continue;
            }
            if (*endptr == '\0') {
                fprintf(output, "%lld %d %lld\n", num, base, num);
                break;
            }
            base++;
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}
