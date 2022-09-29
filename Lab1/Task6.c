#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int power(int x, int n) {
	if(n == 0) return 1;
	if(n < 0) return power (1/x, -n);
	return x * power(x, n-1);
}
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

int main(){
    FILE *in;
	FILE *out;
	int is_skip = 1;
	char *num = "";
	char sym[1];
	int num_size;
	int p = 0;
	int result;
	if((in = fopen("testfor6", "r")) == NULL){
	    printf("File opening failed\n");
	    return 0;
	}
	if((out = fopen("output6", "w")) == NULL){
	    printf("File opening failed\n");
	    return 0;
	}
	while((sym[0] = fgetc(in)) != EOF){
		if(sym[0] == ' ' || sym[0] == '\t' || sym[0] == '\v' || sym[0] == '\n'){
			if(!is_skip){
				num_size = 0;
				result = 0;
				p = 0;
				while(*num){
					if(*num >= 48 && *num <= 57 && (*num - 48) > p) p = *num - 48;
					if(*num >= 65 && *num <= 90 && (*num - 55) > p) p = *num - 55;
					++num;
					++num_size;
				}
				p += 1;
				--num;
				for(int i = num_size - 1; i >= 0; i--){
					if(*num >= 48 && *num <= 57){
						result += (*num - 48) * power(p, num_size - i - 1);
					}
					else if(*num >= 65 && *num <= 90){
						result += (*num - 55) * power(p, num_size - i - 1);
					}
					--num;
				}
				++num;
				fprintf(out, "%s %d %d\n", num, p, result);
				num = "";
				is_skip = 1;
			}
		}
		else{
			is_skip = 0;
			push_back(&num, sym);
		}
	}
	fclose(in);
	fclose(out);
    return 0;
}
