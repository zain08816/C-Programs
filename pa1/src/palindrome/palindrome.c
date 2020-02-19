#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int palindrome(char *string){

    int left = 0;
    int right = strlen(string) - 1;
    // printf("left: %d with %c\n", left, string[left]);
    // printf("right: %d with %c\n", right, string[right]);

    while (right > left) {
        // printf("left: %d with %c\n", left, string[left]);
        // printf("right: %d with %c\n", right, string[right]);
        

        if (isalpha(string[left]) && isalpha(string[right])) {
            // puts("both are letters");
            if (tolower(string[left]) == tolower(string[right])){
                // puts("both are the same");
                left += 1;
                right -= 1;
                continue;
            } else {
                // puts("not a palindrome");
                return 0;
            }
        }

         else if (!isalpha(string[left])) {
            // puts("left isnt letter");
            left += 1;
            continue;
        }

        else if (!isalpha(string[right])) {
            // puts("right isnt letter");
            right -= 1;
            continue;
        }

    }

    return 1;

}

int main(int argc, char **argv){

    char *string = argv[1];


    if (string != NULL) {
        if (palindrome(string)) {
            printf("yes\n");
        }  else {
            printf("no\n");
    }

    }




    return 0;
}
