#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void push_back(char** buff, char* element){
	size_t len1 = strlen(*buff);
	size_t len2 = strlen(element);
	char* temp = malloc(len1 + len2 + 1);
	if (!temp) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return;
    }
	memcpy(temp, *buff, len1);
	memcpy(temp + len1, element, len2 + 1);
	*buff = malloc(len1 + len2 + 1);
	if (!*buff) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return;
    }
	memcpy(*buff, temp, len1 + len2 + 1);
	free(temp);
}

int main(int argc, char** argv){
    if (argc == 1) {
		printf("No arguments\n");
		return 0;
	}
	else if (argc > 2){
		printf("Too many arguments\n");
		return 0;
	}
	FILE *in;
	char* inname = argv[1];
	if((in = fopen(inname, "r")) == NULL){
	    printf("File opening failed\n");
	    return 0;
	}
	int lecsemcnt = 1;
	int is_skip = 1;
	char sym[1];
	char* buff = "";
	char* lecsem1 = "";
	char* lecsem2 = "";
	char* lecsem3 = "";
	while((sym[0] = fgetc(in)) != EOF){
		if(sym[0] == ' ' || sym[0] == '\t' || sym[0] == '\v' || sym[0] == '\n'){
			if(!is_skip){
				if(lecsemcnt == 3){
					lecsemcnt = 0;
					push_back(&buff, lecsem3);
					push_back(&buff, " ");
					push_back(&buff, lecsem1);
					push_back(&buff, " ");
					push_back(&buff, lecsem2);
					push_back(&buff, "\n");
					lecsem1 = "";
					lecsem2 = "";
					lecsem3 = "";
				}
				lecsemcnt++;
				is_skip = 1;
			}
		}
		else{
			is_skip = 0;
			if(lecsemcnt == 1) push_back(&lecsem1, sym);
			if(lecsemcnt == 2) push_back(&lecsem2, sym);
			if(lecsemcnt == 3) push_back(&lecsem3, sym);
		}
	}
	fclose(in);
	if((in = fopen(inname, "w")) == NULL){
	    printf("File opening failed\n");
	    return 0;
	}
	while(*buff){
		fputc(*buff, in);
		++buff;
	}
	fclose(in);
    return 0;
}
