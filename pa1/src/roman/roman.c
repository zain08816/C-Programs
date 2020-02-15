#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int roman(int number){

    if (number == 0) {
        printf("\n");
        return 0;
    }

    while (number != 0) {
        if (number >= 1000) {printf("M"); number -= 1000;}
        else if (number >= 900) {printf("CM"); number -= 900;}
        else if (number >= 500) {printf("D"); number -= 500;}
        else if (number >= 400) {printf("CD"); number -= 400;}
        else if (number >= 100) {printf("C"); number -= 100;}
        else if (number >= 90) {printf("XC"); number -= 90;}
        else if (number >= 50) {printf("L"); number -= 50;}
        else if (number >= 40) {printf("XL"); number -= 40;}
        else if (number >= 10) {printf("X"); number -= 10;}
        else if (number >= 9) {printf("IX"); number -= 9;}
        else if (number >= 5) {printf("V"); number -= 5;}
        else if (number >= 4) {printf("IV"); number -= 4;}
        else if (number >= 1) {printf("I"); number -= 1;}
    }
    printf("\n");

    return 0;
}



int main(int argc, char **argv){

    int number = atoi(argv[1]);

    roman(number);

    return 0;
}