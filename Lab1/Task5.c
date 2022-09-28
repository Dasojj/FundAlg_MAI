#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    if (argc == 1) {
		printf("No arguments\n");
		return 0;
	}
	FILE *in;
	FILE *out;
	char* inname;
	char sym;
	if((out = fopen("output5", "w")) == NULL){
	    printf("File opening/creation failed\n");
	    return 0;
	}
	if(!strcmp(argv[1], "-fi")){
		if (argc != 3) {
			printf("Wrong arguments\n");
			return 0;
		}
		if((in = fopen(argv[2], "r")) == NULL){
			printf("File opening failed\n");
			return 0;
		}
		FILE *in2;
		while(fscanf(in, "%s", inname) == 1){
			if((in2 = fopen(inname, "r")) == NULL){
				printf("File opening failed\n");
				return 0;
			}
			while((sym = fgetc(in2)) != EOF){
				fputc(sym, out);
			}
			fclose(in2);
		}
		fclose(in);
	}
	else if(!strcmp(argv[1], "-cin")){
		if (argc > 2) {
			printf("Too many arguments\n");
			return 0;
		}
		while(scanf("%s", inname) == 1){
			if((in = fopen(inname, "r")) == NULL){
				printf("File opening failed\n");
				return 0;
			}
			while((sym = fgetc(in)) != EOF){
				fputc(sym, out);
			}
			fclose(in);
		}
		fclose(out);
	}
	else if(!strcmp(argv[1], "-arg")){
		for(int i = 2; i < argc; i++){
			if((in = fopen(argv[i], "r")) == NULL){
			    printf("File opening failed\n");
			    return 0;
			}
			while((sym = fgetc(in)) != EOF){
				fputc(sym, out);
			}
			fclose(in);
		}
		fclose(out);
	}
    return 0;
}
