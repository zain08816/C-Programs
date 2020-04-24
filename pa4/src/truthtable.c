/* CS 211 PA4
 * Created for: zaa23
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum { AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } type;




struct gate {
    type op_type;
    int solved;
    int variables;
    int temps;
    int selectors;
    char name[50][20];
};

struct hashtable {
    int size;
    struct node **array;
};

struct node {
    int var;
    struct node *next;
    char name[20];
};

int gi; //global iterator
int *input_keys, *output_keys;
int num_inputs, num_outputs;
struct hashtable *inputs;
struct hashtable *outputs;
int temp_vars[100];
int num_temps = 0;
struct gate *gate_list[100];
int num_gates = 0;

int hash_key(char var[]) {
    int length = strlen(var);
    int running = 0;
    for (gi = 0; gi < length; gi += 1) {
        running += var[gi] * (10+gi);
    }
    return abs(running % 1000);
}


int hash_search(char name[], int key, int type) {
    struct node *chain;
    if (type) {
        chain = inputs -> array[key];
    } else {
        chain = outputs -> array[key];
    }
    
    while(chain != NULL) {
        if (strcmp(chain -> name, name) == 0) {
            return 1;
        }
        chain = chain -> next;
    }
    return 0;
}


void hash_set(char name[], int key, int type, int value) {
    struct node *chain;
    if (type) {
        chain = inputs -> array[key];
    } else {
        chain = outputs -> array[key];
    }

    while(1) {
        if (strcmp(chain -> name, name) == 0) {
            chain -> var = value;
            break;
        }
        chain = chain -> next;
    }
}



int hash_get(char var[], int key, int type) {
    struct node *chain;
    if (type) chain = inputs -> array[key];
    else chain = outputs -> array[key];

    while (chain != NULL) {
        if (strcmp(chain -> name, var) == 0) return chain -> var;
        chain = chain -> next;
    }
    return -1;
}

void solve_circuit() {
    int i, j, key, indexer, num_solved, skip, get_in, get_out, num_found;
    //pow(2, num_inputs)

    for (i = 0; i < pow(2, num_inputs); i += 1) {
        // printf("num_inputs: %d\n", num_inputs);
        // printf("Iteration: %d\n", i);

        //DO BINARY AND SETUP BINARY
        for (j = num_inputs - 1; j >= 0; j -= 1) {
            int binary = i >> j;
            if (binary & 1) binary = 1;
            else binary = 0;
            printf("%d ", binary);
            key = input_keys[num_inputs-j-1];
            hash_set(inputs -> array[key] -> name, key, 1, binary);
        }
        printf("|");
        num_solved = 0;
        int first, second, var_value, find, output_value, section, search_in, value, var_count;
        char *var_name;
        for (indexer = 0; num_solved != num_gates; indexer += 1) {
            num_found = 0;
            first = -1; second = -1;
            if (indexer == num_gates){
                indexer = 0;
            } 
            if (gate_list[indexer] -> solved == 0) { // UNSOLVED
                
                switch(gate_list[indexer] -> op_type) {
                    case AND:
                        for (j = 0; j < gate_list[indexer] -> variables; j += 1) {
                            var_name = gate_list[indexer] -> name[j];
                            key = hash_key(var_name);
                            var_value = hash_get(var_name, key, 1);

                            if (var_value != -1) {
                                num_found += 1;
                                if (first == -1) first = var_value;
                                else second = var_value;
                            } else {
                                var_value = hash_get(var_name, key, 0);
                                if (var_value != -1) {
                                    num_found += 1;
                                    if (first == -1) first = var_value;
                                    else second = var_value;
                                }
                                else break;
                            }
                        }
                        if (num_found == 2) {
                            output_value = first && second;
                            var_name = gate_list[indexer] -> name[2];
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                            gate_list[indexer] -> solved = 1;
                            num_solved += 1;
                        } else continue;
                        break;

                    case NAND:
                        for (j = 0; j < gate_list[indexer] -> variables; j += 1) {
                            var_name = gate_list[indexer] -> name[j];
                            key = hash_key(var_name);
                            var_value = hash_get(var_name, key, 1);

                            if (var_value != -1) {
                                num_found += 1;
                                if (first == -1) first = var_value;
                                else second = var_value;
                            } else {
                                var_value = hash_get(var_name, key, 0);
                                if (var_value != -1) {
                                    num_found += 1;
                                    if (first == -1) first = var_value;
                                    else second = var_value;
                                }
                                else break;
                            }
                        }
                        if (num_found == 2) {
                            output_value = !(first && second);
                            var_name = gate_list[indexer] -> name[2];
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                            gate_list[indexer] -> solved = 1;
                            num_solved += 1;
                        } else continue;
                        break;

                    case OR:
                        for (j = 0; j < gate_list[indexer] -> variables; j += 1) {
                            var_name = gate_list[indexer] -> name[j];
                            key = hash_key(var_name);
                            var_value = hash_get(var_name, key, 1);

                            if (var_value != -1) {
                                num_found += 1;
                                if (first == -1) first = var_value;
                                else second = var_value;
                            } else {
                                var_value = hash_get(var_name, key, 0);
                                if (var_value != -1) {
                                    num_found += 1;
                                    if (first == -1) first = var_value;
                                    else second = var_value;
                                }
                                else break;
                            }
                        }
                        if (num_found == 2) {
                            num_solved += 1;
                            output_value = first || second;
                            var_name = gate_list[indexer] -> name[2];
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            gate_list[indexer] -> solved = 1;
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                        } else continue;
                        break;

                    case NOR:
                        for (j = 0; j < gate_list[indexer] -> variables; j += 1) {
                            var_name = gate_list[indexer] -> name[j];
                            key = hash_key(var_name);
                            var_value = hash_get(var_name, key, 1);

                            if (var_value != -1) {
                                num_found += 1;
                                if (first == -1) first = var_value;
                                else second = var_value;
                            } else {
                                var_value = hash_get(var_name, key, 0);
                                if (var_value != -1) {
                                    num_found += 1;
                                    if (first == -1) first = var_value;
                                    else second = var_value;
                                }
                                else break;
                            }
                        }
                        if (num_found == 2) {
                            num_solved += 1;
                            output_value = !(first || second);
                            var_name = gate_list[indexer] -> name[2];
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            gate_list[indexer] -> solved = 1;
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                            
                            
                        } else continue;
                        break;

                    case XOR:
                        for (j = 0; j < gate_list[indexer] -> variables; j += 1) {
                            var_name = gate_list[indexer] -> name[j];
                            key = hash_key(var_name);
                            var_value = hash_get(var_name, key, 1);

                            if (var_value != -1) {
                                num_found += 1;
                                if (first == -1) first = var_value;
                                else second = var_value;
                            } else {
                                var_value = hash_get(var_name, key, 0);
                                if (var_value != -1) {
                                    num_found += 1;
                                    if (first == -1) first = var_value;
                                    else second = var_value;
                                }
                                else break;
                            }
                        }
                        if (num_found == 2) {
                            num_solved += 1;
                            output_value = first ^ second;
                            var_name = gate_list[indexer] -> name[2];
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            gate_list[indexer] -> solved = 1;
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                        } else continue;
                        break;

                    case NOT:
                        var_name = gate_list[indexer] -> name[0];
                        key = hash_key(var_name);
                        output_value = hash_get(var_name, key, 1);
                        output_value = !output_value;
                        if (output_value != -1) {
                            num_solved += 1;
                            var_name = gate_list[indexer] -> name[1];
                            // printf( "%16s\n", var_name);
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            gate_list[indexer] -> solved = 1;
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                            
                        } else continue;

                        break;

                    case PASS:
                        var_name = gate_list[indexer] -> name[0];
                        key = hash_key(var_name);
                        output_value = hash_get(var_name, key, 1);
                        output_value = !output_value;
                        if (output_value != -1) {
                            num_solved += 1;
                            var_name = gate_list[indexer] -> name[1];
                            // printf( "%16s\n", var_name);
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            gate_list[indexer] -> solved = 1;
                            if (get_in == 1) hash_set(var_name, key, 1, output_value && 1);
                            else hash_set(var_name, key, 0, output_value);
                            
                        } else continue;

                        break;

                    case DECODER:

                        skip = 0;
                        for (j =0; j < gate_list[indexer] -> selectors; j += 1) {
                            var_name = gate_list[indexer] -> name[j];
                            key = hash_key(var_name);
                            get_in = hash_get(var_name, key, 1);
                            get_out = hash_get(var_name, key, 0);
                            if (get_in != -1) { continue; }
                            else if (get_out == -1) {
                                skip = 1;
                                break;
                            } else { continue; }
                        }
                        if (skip) { continue; }
                        section = 0;

                        for (j = 0; j < gate_list[indexer] -> selectors; j += 1) {
                            var_name = gate_list[indexer] -> name[j];
                            key = hash_key(var_name);
                            get_in = hash_get(var_name, key, 1);
                            get_out = hash_get(var_name, key, 0);
                            if (get_in != -1) {
                                var_value = get_in;
                            } else { var_value = get_out; }
                            section = section << 1;
                            section += var_value;
                        }

                        find = 0;
                        for (j = gate_list[indexer] -> selectors; j < gate_list[indexer] -> variables+1; j += 1) {
                            var_name = gate_list[indexer] -> name[j];
                            key = hash_key(var_name);
                            search_in = hash_search(var_name, key, 1);

                            if (find == section) {
                                if (search_in == 1) hash_set(var_name, key, 1, 1);
                                else hash_set(var_name, key, 0, 1);
                                
                            } else {
                                if (search_in == 1) hash_set(var_name, key, 1, 0);
                                else hash_set(var_name, key, 0, 0);
                            }
                            find += 1;
                        }

                        gate_list[indexer] -> solved = 1;
                        num_solved += 1;

                        break;

                    case MULTIPLEXER:

                        skip = 0;
                        for (j = 0; j < gate_list[indexer] -> variables; j += 1) {
                            var_name = gate_list[indexer] -> name[j];
                            key = hash_key(var_name);
                            get_in = hash_get(var_name, key, 1);
                            get_out = hash_get(var_name, key, 0);
                            if (get_in != -1) { continue; }
                            else if (get_out == -1) {
                                skip = 1;
                                break;
                            } else { continue; }
                        }
                        if (skip == 1) { continue; }

                        section = 0;
                        for (j = pow(2, gate_list[indexer] -> selectors); j < gate_list[indexer] -> variables; j += 1) {
                            var_name = gate_list[indexer] -> name[j];
                            key = hash_key(var_name);
                            get_in = hash_get(var_name, key, 1);
                            get_out = hash_get(var_name, key, 0);
                            if (get_in != -1) {
                                var_value = get_in;
                            } else { var_value = get_out; }

                            section = section << 1;
                            section += var_value; 
                        }

                        var_name = gate_list[indexer] -> name[section];
                        key = hash_key(var_name);
                        value = hash_get(var_name, key, 1);
                        if (value == -1) { value = hash_get(var_name, key, 1); }
                        var_count = gate_list[indexer] -> variables;
                        var_name = gate_list[indexer] -> name[var_count];
                        key = hash_key(var_name);
                        if (hash_search(var_name, key, 1) == 1) {
                            hash_set(var_name, key, 1, value);
                        } else {
                            hash_set(var_name, key, 0, value);
                        }
                        gate_list[indexer] -> solved = 1;
                        num_solved += 1;

                        break;

                }
            }
        }

        for(j = 0; j < num_outputs; j += 1) {
            key = output_keys[j];
            printf(" %d", outputs -> array[key] -> var);
            outputs -> array[key] -> var = -1;
        }
        printf("\n");
        for (j = 0; j < num_temps; j += 1) {
            key = temp_vars[j];
            inputs -> array[key] -> var = -1;
        }
        for (j = 0; j < num_gates; j += 1) {
            gate_list[j] -> solved = 0;
        }
        
    }
}



int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Invalid Arguments\n");
        return 0;
    }

    //get file, return if invalid path
    FILE *ins = fopen(argv[1], "r");
    if (ins == NULL) {
        printf("Invalid Arguments\n");
        return 0;
    }

    // LOCALS
    char buffer[30];
    int num_vars, i, key;
    inputs = (struct hashtable*) malloc(sizeof(struct hashtable));
    outputs = (struct hashtable*) malloc(sizeof(struct hashtable));
    inputs -> size = 1000;
    inputs -> array = (struct node**) malloc(sizeof(struct node*) * 1000);
    outputs -> size = 1000;
    outputs -> array = (struct node**) malloc(sizeof(struct node*) * 1000);
    for (gi = 0; gi < 1000; gi += 1) {
        inputs -> array[gi] = NULL;
        outputs -> array[gi] = NULL;
    }

    // Inputs
    fscanf(ins, " %16s", buffer);
    fscanf(ins, " %d", &num_inputs);
    input_keys = (int*) malloc(sizeof(int) * num_inputs);
    for(i = 0; i < num_inputs; i += 1) {
        fscanf(ins, " %16s", buffer);
        key = hash_key(buffer);
        input_keys[i] = key;
        if (hash_search(buffer, key, 1) != 1) {
            struct node *insert = (struct node*) malloc(sizeof(struct node));
            if (inputs -> array[key] == NULL) {
                insert -> next = NULL;
                insert -> var = -1;
                strcpy(insert -> name, buffer);
                inputs -> array[key] = insert;
            } else {
                insert -> next = inputs -> array[key];
                insert -> var = -1;
                strcpy(insert -> name, buffer);
                inputs -> array[key] = insert;
            }

        }
       
    }

    // Outputs
    fscanf(ins, " %16s", buffer);
    fscanf(ins, " %d", &num_outputs);
    output_keys = (int*) malloc(sizeof(int) * num_outputs);
    for (i = 0; i < num_outputs; i += 1) {
        fscanf(ins, " %16s", buffer);
        key = hash_key(buffer);
        output_keys[i] = key;
        if (hash_search(buffer, key, 1) != 1) {
            struct node *insert = (struct node*) malloc(sizeof(struct node));
            if (outputs -> array[key] == NULL) {
                insert -> next = NULL;
                insert -> var = -1;
                strcpy(insert -> name, buffer);
                outputs -> array[key] = insert;
            } else {
                insert -> next = outputs -> array[key];
                insert -> var = -1;
                strcpy(insert -> name, buffer);
                outputs -> array[key] = insert;
            }

        }
 
    }


    while (fscanf(ins, " %16s", buffer) != EOF) {
        struct gate *gate;
        if (strcmp(buffer, "AND") == 0) {
            gate = (struct gate*) malloc(sizeof(struct gate));
            gate -> op_type = AND;
            gate -> variables = 2;
            gate -> solved = 0;

            for (i = 0; i < 3; i += 1) {
                fscanf(ins, " %16s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
             
                    
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
             
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "OR") == 0) {
            gate = (struct gate*) malloc(sizeof(struct gate));
            gate -> op_type = OR;
            gate -> variables = 2;
            gate -> solved = 0;

            for (i = 0; i < 3; i += 1) {
                fscanf(ins, " %16s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
                   
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
              
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "NAND") == 0) {
            gate = (struct gate*) malloc(sizeof(struct gate));
            gate -> op_type = NAND;
            gate -> variables = 2;
            gate -> solved = 0;

            for (i = 0; i < 3; i += 1) {
                fscanf(ins, " %16s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
             
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
           
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "NOR") == 0) {
            gate = (struct gate*) malloc(sizeof(struct gate));
            gate -> op_type = NOR;
            gate -> variables = 2;
            gate -> solved = 0;

            for (i = 0; i < 3; i += 1) {
                fscanf(ins, " %16s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
  
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
            
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "XOR") == 0) {
            gate = (struct gate*) malloc(sizeof(struct gate));
            gate -> op_type = XOR;
            gate -> variables = 2;
            gate -> solved = 0;

            for (i = 0; i < 3; i += 1) {
                fscanf(ins, " %16s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
           
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
            
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "NOT") == 0) {
            gate = (struct gate*) malloc(sizeof(struct gate));
            gate -> op_type = NOT;
            gate -> variables = 1;
            gate -> solved = 0;

            for (i = 0; i < 2; i += 1) {
                fscanf(ins, " %16s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
                  
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (in_inputs == 0 && in_outputs == 0) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
            
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "PASS") == 0) {
            gate = (struct gate*) malloc(sizeof(struct gate));
            gate -> op_type = PASS;
            gate -> variables = 1;
            gate -> solved = 0;

            for (i = 0; i < 2; i += 1) {
                fscanf(ins, " %16s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
            
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (in_inputs == 0 && in_outputs == 0) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
            
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "DECODER") == 0) {
            gate = (struct gate*) malloc(sizeof(struct gate));
            gate -> op_type = DECODER;
            gate -> solved = 0;
            fscanf(ins, " %d", &num_vars);
            gate -> variables = num_vars + pow(2, num_vars)-1;
            gate -> selectors = num_vars;
            for (i = 0; i < gate -> variables+1; i += 1) {
                fscanf(ins, " %16s", buffer);
                key = hash_key(buffer); //THINK
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
                
                    hash_set(buffer, key, 1, buffer[0] - '0'); //THINK
                } else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
                 
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "MULTIPLEXER") == 0) {
            gate = (struct gate*) malloc(sizeof(struct gate));
            gate -> op_type = MULTIPLEXER;
            gate -> solved = 0;
            fscanf(ins, "%d", &num_vars);
            gate -> variables = pow(2, num_vars) +  num_vars;
            gate -> selectors = num_vars;
            for (i = 0; i < gate -> variables+1; i += 1) {
                fscanf(ins, "%16s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
                   
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    if (hash_search(buffer, key, 1) != 1) {
                        struct node *insert = (struct node*) malloc(sizeof(struct node));
                        if (inputs -> array[key] == NULL) {
                            insert -> next = NULL;
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        } else {
                            insert -> next = inputs -> array[key];
                            insert -> var = -1;
                            strcpy(insert -> name, buffer);
                            inputs -> array[key] = insert;
                        }

                    }
             
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

    }
    
    solve_circuit();

    return 0;
}
