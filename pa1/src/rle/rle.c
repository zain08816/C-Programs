#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int rle(char *string) {

    int original_length = strlen(string);
    int current_length = 0;
    char *buffer = malloc((original_length * sizeof(char))+10);
    int i;

    // printf("original: %d\n", original_length);

    for(i = 0; i < original_length; i += 1) {
        int count = 1;
        if (isdigit(string[i])) {
            free(buffer);
            printf("error");
            return 0;
        } 

        while (i < original_length - 1 && string[i] == string[i + 1]) {
            count += 1;
            i += 1;
        }
        
        current_length += sprintf(buffer + current_length, "%c%d", string[i], count);
        // printf("current: %d\n", current_length);
        // printf("%s\n", buffer);

        if (current_length > original_length) {
            printf("%s\n", string);
            free(buffer);
            return 0;
        }
    }

    if (current_length > original_length) {
        printf("%s\n", string);
        free(buffer);
        return 0;
    }

    printf("%s\n", buffer);
    free(buffer);

    return 0;

}


int main(int argc, char **argv){


    if (argc != 2 || strlen(argv[1]) == 0) {
        puts("");
        return 0;
    }

    char *string = argv[1];
    rle(string);


    return 0;

}