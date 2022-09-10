#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int ston(char* s){
	int num = 0;
	int is_negative = 1;
	if(*s - '0' == '-'){
		is_negative = -1;
	}
	while(*s)
	{
		num = num*10 + (*s++ - '0');
	}
	return num * is_negative;
}
float quad_solution(int a, int b, int c) {
	double d,x1,x2;
	d=(b*b)-4*a*c;
	if (d>0) {
		x1=(-b+sqrt(d))/(2*a);
		x2=(-b-sqrt(d))/(2*a);
		printf("x1=%.3f\n",x1); 
		printf("x2=%.3f\n",x2); 
	}
	if (d<0) {
		printf("No real roots\n");
	}
	if (d==0) {
	x1=(-b+sqrt(d))/(2*a);
	printf("x1=x2=%.3f\n", x1);
	}
	return 0; 
}

int main(int argc, char** argv) {
	if (argc == 1) {
		printf("No arguments\n");
		return 0;
	}
	if (!strcmp(argv[1], "-q")) {
		if(argc != 5) {
			printf("This flag must have 3 arguments\n");
			return 0;
		}
		quad_solution(ston(argv[2]), ston(argv[3]), ston(argv[4]));
	}
	else if (!strcmp(argv[1], "-m")) {
		if(argc != 4) {
			printf("This flag must have 3 arguments\n");
			return 0;
		}
		if(argv[2] == "0" || argv[3] == "0"){
			printf("Arguments must be non-zero\n");
		}
		if(ston(argv[2]) % ston(argv[3]) == 0) printf("%s is multiple to %s \n", argv[2], argv[3]);
		else printf("%s is not multiple to %s \n", argv[2], argv[3]);
	}
	else if (!strcmp(argv[1], "-t")) {
		if(argc != 5) {
			printf("This flag must have 3 arguments\n");
			return 0;
		}
		int a = ston(argv[2]) * ston(argv[2]);
		int b = ston(argv[3]) * ston(argv[3]);
		int c = ston(argv[4]) * ston(argv[4]);
		if(a + b == c || a + c == b || b + c == a) printf("This numbers can be lengths of right triangle\n");
		else printf("These numbers can not be lengths of right triangle\n");
	}
	else {
		printf("Wrong argument(s)\n");
		return 0;
	}
	return 0;
}
