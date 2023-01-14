#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

char* addStringsInBase(char* num1, char* num2, int base) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int maxLen = (len1 > len2) ? len1 : len2;
    int carry = 0;
    char* result = (char*)malloc((maxLen + 2) * sizeof(char));
    int i, j, k = 0;
    for (i = 0; i < maxLen; i++) {
        result[i] = '0';
    }
    result[maxLen] = '\0';
    for (i = len1 - 1, j = len2 - 1; i >= 0 || j >= 0; i--, j--) {
        int a;
        if(num1[i] >= 65) a = (i >= 0) ? (num1[i] - '0' - 7) : 0;
        else a = (i >= 0) ? num1[i] - '0' : 0;
        int b;
        if(num2[j] >= 65) b = (j >= 0) ? (num2[j] - '0' - 7) : 0;
        else b = (j >= 0) ? num2[j] - '0' : 0;
        int sum = a + b + carry;
        if((sum % base) >= 10) result[maxLen - 1 - k++] = (sum % base) + '0' + 7;
        else result[maxLen - 1 - k++] = (sum % base) + '0';
        carry = sum / base;
    }
    if (carry > 0) {
        for (i = maxLen; i > 0; i--) {
            result[i] = result[i - 1];
        }
        result[0] = carry + '0';
    }
    return result;
}

char* addInBase(int base, int num_args, ...) {
    if(base > 36) return "Base is too big";
    va_list args;
    va_start(args, num_args);
    char* result = "0";
    while (num_args--) {
        char* num = va_arg(args, char*);
        result = addStringsInBase(result, num, base);
    }
    va_end(args);
    return result;
}

int main() {
    char* result;
    //base can't be bigger then 36 (26 latin symbols and 10 num symbols)
    result = addInBase(16, 3, "AB", "1E", "F");
    printf("Sum in base 16: %s\n", result);
    result = addInBase(2, 3, "1011", "1111", "1010");
    printf("Sum in base 2: %s\n", result);
    return 0;
}
