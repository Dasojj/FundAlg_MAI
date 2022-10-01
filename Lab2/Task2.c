#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* concat(char *s1, char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);
    if (!result) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    return result;
}

int is_letter(char l){
    if((l >= 'A' && l <= 'Z') || (l >= 'a' && l <= 'z')) return 1;
    return 0;
}

int is_number(char l){
	if(l >= '0' && l <= '9') return 1;
	return 0;
}

int len(char *str){
    int lens = 0;
    while(*str){
        *str++;
        lens++;
    }
    return lens;
}

char *reversed(char *str){
    int l = len(str);
    char* reverse = malloc(l);
    for(int i = 0; i < l; i++)
        reverse[l - i - 1] = str[i];
    return reverse;
}

char toUpper(char ch){
    if(ch >= 97 && ch <= 122) return ch - 32;
    else return ch;
}

void push_back(char** buff, char element){
	size_t len1 = strlen(*buff);
	char* temp = malloc(len1 + 2);
	if (!temp) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return;
    }
	memcpy(temp, *buff, len1);
	temp[len1] = element;
	*buff = malloc(len1 + 2);
	if (!*buff) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return;
    }
	memcpy(*buff, temp, len1 + 2);
	free(temp);
}

int main(int argc, char **argv){
    if(argc == 1){
        printf("No arguments\n");
        return 0;
    }
    if((argc > 3 && strcmp(argv[1], "-c")) || argc > 4){
        printf("Too many arguments\n");
        return 0;
    }
    if(!strcmp(argv[1], "-l")){
        printf("Длина строки: %d\n", len(argv[2]));
    }
    else if(!strcmp(argv[1], "-r")){
        argv[2] = reversed(argv[2]);
        while(*argv[2]) printf("%c", *argv[2]++);
        printf("\n");
    }
    else if(!strcmp(argv[1], "-u")){
        int l = len(argv[2]);
        for(int i = 0; i < l; i++)
            if(i%2 == 1) argv[2][i] = toUpper(argv[2][i]);
        for(int i = 0; i < l; i++) printf("%c", argv[2][i]);
        printf("\n");
    }
    else if(!strcmp(argv[1], "-n")){
        int l = len(argv[2]);
        char *nums = "";
        char *letters = "";
        char *specials = "";
        for(int i = 0; i < l; i++){
            if(is_number(argv[2][i])) push_back(&nums, argv[2][i]);
            else if(is_letter(argv[2][i])) push_back(&letters, argv[2][i]);
            else push_back(&specials, argv[2][i]);
        }
        argv[2] = concat(nums, letters);
        argv[2] = concat(argv[2], specials);
        while(*argv[2]) printf("%c", *argv[2]++);
        printf("\n");
    }
    else if(!strcmp(argv[1], "-c")){
        argv[2] = concat(argv[2], argv[3]);
        while(*argv[2]) printf("%c", *argv[2]++);
        printf("\n");
    }
    else{
        printf("Wrong argument(s)\n");
    }
    return 0;
}
