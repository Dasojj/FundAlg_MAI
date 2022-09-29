#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_letter(char l){
    if((l >= 'A' && l <= 'Z') || (l >= 'a' && l <= 'z')) return 1;
    return 0;
}
int is_number(char l){
	if(l >= '0' && l <= '9') return 1;
	return 0;
}

int main(){
	int base; 
	int max = 0;
	char numbers[34], maxnumber[34];
	printf("Введите систему счисления: ");
	scanf("%d", &base);
	if (base < 2 || base > 36){
		printf("Некорректный ввод!");
	}
	printf("Введите числа: ");
	while (strcmp(numbers, "Stop")){
		scanf("%s", numbers);
		char *pb = numbers - 1;
		int result = 0;
		while (*++pb){
			if ((is_number(*pb) && (*pb - '0' >= base) || is_letter(*pb) && *pb - 'A' + 10 >= base) && strcmp(numbers, "Stop"))
				printf("Некорректный ввод: %c", *pb);
			if(is_number(*pb)) result = result * base + *pb - '0';
			else result = result * base + *pb - 'A' + 10;
		}
		if (result > max && strcmp(numbers, "Stop")){
			max = result;
			strcpy(maxnumber, numbers);
		}
	}
	printf("Максимальное по модулю число в %d системе счисления: %s\n", base, maxnumber);
	printf("Оно в десятичной системе: %d\n", max);
	char str[34];
	for (int i = 9; i <= 36; i += 9){
		int r;
		base = i;
		int temp = max;
		int j = 0;
		while (temp){
			r = temp % base;
			if(r > 9) str[j] = r - 10 + 'A';
			else str[j] = r + '0';
			temp /= base;
			j++;
		}
		printf("%d система счисления: ", base);
		for (int n = j - 1; n >= 0; n--){
			printf("%c", str[n]);
		}
		printf("\n");
	}
	return 0;
}
