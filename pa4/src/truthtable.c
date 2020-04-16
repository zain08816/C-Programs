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
    char *vars; //names
    int *input; //inputs
    int *params; //vars
    int size_in; // numInputs
    int size_out // numOutputs
};

struct node {
    char variable;
    int value;
    struct node *next
};

struct hash {
    int size;
    struct node **table;
};
/*
globals
varTable
instructions
*/

char values[100]; 
struct hash *vars; 
struct gate *gates[100]; 
char output[100];
int total_gates;

void set_var(char var, int value) {
    int key = var % vars -> size;
    struct node *found_var = vars -> table[key];

    if (found_var != NULL) {
        if (found_var -> variable == var) {
            found_var -> value = value;
            return;
        }
    }

    struct node *new_node = malloc(sizeof(struct node));
    new_node -> variable = var;
    new_node -> value = value;
    new_node -> next = NULL;

    struct node *curr = found_var;
    while (curr -> next != NULL) {
        curr = curr -> next; 
    }
    if (curr -> next == NULL) {
        curr -> next = new_node;
    }
    return;
}
 
int get_var(char var) {
    int key = var % vars -> size;

    struct node *head = vars -> table[key];
    if ((head == NULL) || head -> variable == var) {
        return -99;
    } else {
        while (head != NULL) {
            if (head -> variable == var) {
                return head -> value;
            }
        }
        return -99;
    }

}


void solve_gate(int gate_number) {
    struct gate *single_gate = gates[gate_number];

    int i, gate_input;
    for (i = 0; single_gate -> vars[i] != '\0'; i += 1) {
        if(get_var(single_gate -> vars[i]) < 0) {
            return;
        }
    }

    int num_inputs =  single_gate -> size_in;
    if (single_gate -> kind == MULTIPLEXER) {
        num_inputs += single_gate -> size_out;
    }
    for (i = 0; i < num_inputs; i += 1) {
        if (single_gate -> kind == MULTIPLEXER) {
            if ( i >= single_gate -> size_in) {
                gate_input = single_gate -> params[i - single_gate -> size_in];
            } else {
                gate_input = single_gate -> input[i];
            }
        } else {
            gate_input = single_gate -> input[i];
        }

        if (isalpha(gate_input)) {
            char gate_var = gate_input;

            if (get_var(gate_var) < 0) {
                int j;
                struct gate *unsolved_gates;
                int k, found;
                for (j = gate_number+1;  j < total_gates; j += 1) {
                    unsolved_gates = gates[j];
                    found = 0;
                    for (k = 0; unsolved_gates -> vars[k] != '\0'; k += 1) {
                        if (unsolved_gates -> vars[k] == gate_input) {
                            solve_gate(j);
                            found = 1;
                            break;
                        }
                        if (found == 1) {
                            break;
                        }
                    }
                }
                
                for (j = gate_number-1;  j >= 0; j -= 1) {
                    unsolved_gates = gates[j];
                    found = 0;
                    for (k = 0; unsolved_gates -> vars[k] != '\0'; k += 1) {
                        if (unsolved_gates -> vars[k] == gate_input) {
                            solve_gate(j);
                            found = 1;
                            break;
                        }
                        if (found == 1) {
                            break;
                        }
                    }
                }


            }
        }
    }

    int temp_gate = 0;
    switch (single_gate -> kind) {

        case AND:
            temp_gate = single_gate -> params[0];
            if (temp_gate > 1) {
                temp_gate = get_var(temp_gate);
            }
            for (i = 1; i < single_gate -> size_in: i += 1){
                int param = single_gate -> params[p];
                if (param > 1) {
                    param = get_var(param);
                }
                temp_gate = temp_gate && param;
            }
            break;

        case OR:
            temp_gate = single_gate -> params[0];
            if (temp_gate > 1) {
                temp_gate = get_var(temp_gate);
            }
            for (i = 1; i < single_gate -> size_in: i += 1){
                int param = single_gate -> params[p];
                if (param > 1) {
                    param = get_var(param);
                }
                temp_gate = temp_gate || param;
            }
            break;

        case NAND:
            temp_gate = single_gate -> params[0];
            if (temp_gate > 1) {
                temp_gate = get_var(temp_gate);
            }
            for (i = 1; i < single_gate -> size_in: i += 1){
                int param = single_gate -> params[p];
                if (param > 1) {
                    param = get_var(param);
                }
                temp_gate = !(temp_gate && param);
            }
            break;

        case NOR:
            break;

        case XOR:
            break;

        case NOT:
            break;

        case PASS:
            break;

        case DECODER:
            break;

        case MULTIPLEXER:
            break;
        

    }
    
    
}



int main(int argc, char **argv) {

    printf("%s: not implemented\n", argv[0]);



    return EXIT_SUCCESS;
}
