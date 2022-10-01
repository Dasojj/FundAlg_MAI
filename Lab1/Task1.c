#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_by_num(unsigned int num) {
	if (num/10 > 0) print_by_num(num/10);
	printf("%d ", num%10);
	return;
}
unsigned int ston(char* s){
	unsigned int num = 0;
	while(*s)
	{
		num = num*10 + (*s++ - '0');
	}
	return num;
}


int main(int argc, char** argv) {
	if (argc == 1) {
		printf("No arguments\n");
		return 0;
	}
	if (argc > 3) {
		printf("Too many arguments\n");
		return 0;
	}
	if (ston(argv[2]) < 0) {
		printf("Second argument must be a positive number\n");
		return 0;
	}
	unsigned int num = ston(argv[2]);
	if (num == 0 && strcmp(argv[2], "0")) {
		printf("Second argument must be a positive number\n");
		return 0;
	}
	if (!strcmp(argv[1], "-h")) {
		for (int i = num; i <= 100; i+=num)
			printf("%d ", i);
		printf("\n");
	}
	else if (!strcmp(argv[1], "-p")) {
		if (num <= 1) printf("Number is neither simple nor composite\n");
		else {
			for (unsigned int i = 2; i < num; i++)
				if (num % i == 0) {
					printf("Number is composite\n");
					return 0;
				}
			printf("Number is simple\n");
		}
	}
	else if (!strcmp(argv[1], "-s")) {
		print_by_num(num);
		printf("\n");
	}
	else if (!strcmp(argv[1], "-e")) {
		if(num > 10 || num == 0) printf("Incorrect number for this argument\n");
		else{
			for(int i = 1; i <= 10; i++){
				int p = i;
				for(int j = 1; j <= num; j++){
					printf("%d ", p);
					p *= i;
				}
				printf("\n");
			}
		}
	}
	else if (!strcmp(argv[1], "-a")) {
		unsigned int res = 0;
		for(int i = 1; i <= num; i++){
			res += i;
		}
		printf("%d\n", res);
	}
	else if (!strcmp(argv[1], "-f")) {
		unsigned int res = 1;
		for(int i = 2; i <= num; i++){
			res *= i;
		}
		printf("%d\n", res);
	}
	else {
		printf("Wrong argument(s)\n");
		return 0;
	}
	return 0;
}
