/* CS 211 PA4
 * Created for: zaa23
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } kind_t;

struct gate {
    kind_t kind;
    int size;
    int *params;
};





int main(int argc, char **argv) {

    printf("%s: not implemented\n", argv[0]);



    return EXIT_SUCCESS;
}
