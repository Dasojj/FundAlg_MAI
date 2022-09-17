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
char* getfilename(char* path){
    char *filename;
    filename = path + strlen(path);
    for (; filename > path; filename--)
    {
        if ((*filename == '\\') || (*filename == '/'))
        {
            filename++;
            break;
        }
    }
    return filename;
}
int is_letter(char l){
    if((l >= 'A' && l <= 'Z') || (l >= 'a' && l <= 'z')) return 1;
    return 0;
}
int is_special(char l){
    if((l >= 'A' && l <= 'Z') || (l >= 'a' && l <= 'z') || (l >= '0' && l <= '9') || l == ' ' || l == '\n') return 0;
    return 1;
}
int is_number(char l){
	if(l >= '0' && l <= '9') return 1;
	return 0;
}

int main(int argc, char** argv){
    if (argc == 1) {
		printf("No arguments\n");
		return 0;
	}
	FILE *in;
	FILE *out;
	if(argv[1][0] == '-' && argv[1][1] == 'n'){
		if(argc != 4) {
			printf("This flag must have 2 arguments\n");
			return 0;
		}
		char* inname = argv[2];
		if((in = fopen(inname, "r")) == NULL){
		    printf("File opening failed\n");
		    return 0;
		}
		char* outname = argv[3];
		if((out = fopen(outname, "w")) == NULL){
		    printf("File opening/creation failed\n");
		    return 0;
		}
	}
	else if(argv[1][0] == '-'){
		if(argc != 3) {
			printf("This flag must have 1 argument\n");
			return 0;
		}
		char* inname = argv[2];
		if((in = fopen(inname, "r")) == NULL){
		    printf("File opening failed\n");
		    return 0;
		}
		char* outname = concat("out_", getfilename(argv[2]));
		if((out = fopen(outname, "w")) == NULL){
		    printf("File opening/creation failed\n");
		    return 0;
		}
	}
	else{
		printf("Wrong arguments\n");
		return 0;
	}
	if ((!strcmp(argv[1], "-d")) || (!strcmp(argv[1], "-nd"))) {
		char sym;
		while((sym = fgetc(in)) != EOF){
		    if('0' <= sym && sym <= '9') continue;
		    fputc(sym, out);
		}
	}
	else if ((!strcmp(argv[1], "-i")) || (!strcmp(argv[1], "-ni"))) {
		int count = 0;
		int line = 1;
		char sym;
		while((sym = fgetc(in)) != EOF){
			if(is_letter(sym)) count += 1;
			else if(sym == '\n'){
				fprintf(out, "Line %d: %d letters\n", line, count);
				count = 0;
				line += 1;
			}
		}
	}
	else if ((!strcmp(argv[1], "-s")) || (!strcmp(argv[1], "-ns"))){
		int count = 0;
		int line = 1;
		char sym;
		while((sym = fgetc(in)) != EOF){
			if(is_special(sym)) count += 1;
			else if(sym == '\n'){
				fprintf(out, "Line %d: %d special symbols\n", line, count);
				count = 0;
				line += 1;
			}
		}
	}
	else if ((!strcmp(argv[1], "-a")) || (!strcmp(argv[1], "-na"))){
		char sym;
		int asci;
		while((sym = fgetc(in)) != EOF){
			if(!is_number(sym)){
				asci = sym;
				fprintf(out, "%d", asci);
			}
			else fputc(sym, out);
		}
	}
	else if ((!strcmp(argv[1], "-f")) || (!strcmp(argv[1], "-nf"))){
		char sym;
		int asci;
		int lecsemcnt = 1;
		int is_skip = 1;
		while((sym = fgetc(in)) != EOF){
			if(sym == ' ' || sym == '\t' || sym == '\v' || sym == '\n'){
				if(!is_skip){
					lecsemcnt++;
					is_skip = 1;
				}
				fputc(sym, out);
			}
			else{
				is_skip = 0;
				if((lecsemcnt % 2 == 0) && (sym >= 'A' && sym <= 'Z')) sym += 32;
				if(lecsemcnt % 5 == 0){
					asci = (int)sym;
					fprintf(out, "%d", asci);
				}
				else fputc(sym, out);
			}
		}
	}
	else {
		printf("Wrong argument(s)\n");
		return 0;
	}
	fclose(in);
	fclose(out);
    return 0;
}
