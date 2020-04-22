/* CS 211 PA4
 * Created for: zaa23
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum { AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } type;


struct node {
    int var;
    char name[20];
    struct node *next;
};

struct gate {
    type op_type;
    int variables;
    int temps;
    char name[100][20];
    int solved;
    int selectors;
};

struct hashtable {
    int size;
    struct node **table;
};

int gi; //global iterator
int *input_keys, *output_keys;
int num_inputs, num_outputs;
struct hashtable *inputs;
struct hashtable *outputs;
int temp_vars[300];
int num_temps = 0;
struct gate *gate_list[100];
int num_gates = 0;


void create_table(int size, int type) {
    if (type) {
        inputs = malloc(sizeof(struct hashtable));
        inputs -> size = size;
        inputs -> table = malloc(sizeof(struct node*) * size);
        for (gi = 0; gi < size; gi += 1) {
            inputs -> table[gi] = NULL;
        }
    } else {
        outputs = malloc(sizeof(struct hashtable));
        outputs -> size = size;
        outputs -> table = malloc(sizeof(struct node*) * size);
        for (gi = 0; gi < size; gi += 1) {
            outputs -> table[gi] = NULL;
        }
    }
}

void reset() {
    for (gi = 0; gi < num_gates; gi += 1) {
        gate_list[gi] -> solved = 0;
    }
}

int hash_search(char name[], int key, int type) {
    struct node *chain;
    if (type) {
        chain = inputs -> table[key];
    } else {
        chain = outputs -> table[key];
    }
    
    while(chain != NULL) {
        if (strcmp(chain -> name, name) == 0) {
            return 1;
        }
        chain = chain -> next;
    }
    return 0;
}

void hash_insert(char name[], int key, int type) {
    if (hash_search(name, key, type) == 1) return;

    struct hashtable *table;
    if(type) {
        table = inputs;
    } else {
        table = outputs;
    }

    // struct node *temp;
    struct node *insert = malloc(sizeof(struct node));
    if (table -> table[key] == NULL) {
        strcpy(insert -> name, name);
        insert -> var = -1;
        insert -> next = NULL;
        table -> table[key] = insert;
    } else {
        strcpy(insert -> name, name);
        insert -> var =  -1;
        insert -> next = table -> table[key];
        table -> table[key] = insert;
    }
}

void hash_set(char name[], int key, int type, int value) {
    struct node *chain;
    if (type) {
        chain = inputs -> table[key];
    } else {
        chain = outputs -> table[key];
    }

    while(1) {
        if (strcmp(chain -> name, name) == 0) {
            chain -> var = value;
            break;
        }
        chain = chain -> next;
    }
}

int hash_key(char var[]) {
    int length = strlen(var);
    int running = 0;
    for (gi = 0; gi < length; gi += 1) {
        running += var[gi] * (10+gi);
    }
    return abs(running % 1000);
}

int hash_get(char var[], int key, int type) {
    struct node *chain;
    if (type) chain = inputs -> table[key];
    else chain = outputs -> table[key];

    while (chain != NULL) {
        if (strcmp(chain -> name, var) == 0) return chain -> var;
        chain = chain -> next;
    }
    return -1;
}

void solve_circuit() {
    int i, j, key, count, num_solved, skip, get_in, get_out, num_found;
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
            hash_set(inputs -> table[key] -> name, key, 1, binary);
        }
        printf("|");
        num_solved = 0;
        int first, second, var_value, find, output_value, section, search_in, value, var_count;
        char *var_name;
        for (count = 0; 1 < 2; count += 1) {
            num_found = 0;
            first = -1; second = -1;
            if (count == num_gates) count = 0;
            if (num_solved == num_gates) break;
            if (gate_list[count] -> solved == 0) { // UNSOLVED
                
                switch(gate_list[count] -> op_type) {
                    case AND:
                        for (j = 0; j < gate_list[count] -> variables-1; j += 1) {
                            var_name = gate_list[count] -> name[j];
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
                            var_name = gate_list[count] -> name[2];
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                            gate_list[count] -> solved = 1;
                            num_solved += 1;
                        } else continue;
                        break;

                    case NAND:
                        for (j = 0; j < gate_list[count] -> variables-1; j += 1) {
                            var_name = gate_list[count] -> name[j];
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
                            var_name = gate_list[count] -> name[2];
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                            gate_list[count] -> solved = 1;
                            num_solved += 1;
                        } else continue;
                        break;

                    case OR:
                        for (j = 0; j < gate_list[count] -> variables-1; j += 1) {
                            var_name = gate_list[count] -> name[j];
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
                            output_value = first || second;
                            var_name = gate_list[count] -> name[2];
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                            gate_list[count] -> solved = 1;
                            num_solved += 1;
                        } else continue;
                        break;

                    case NOR:
                        for (j = 0; j < gate_list[count] -> variables-1; j += 1) {
                            var_name = gate_list[count] -> name[j];
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
                            output_value = !(first || second);
                            var_name = gate_list[count] -> name[2];
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                            gate_list[count] -> solved = 1;
                            num_solved += 1;
                        } else continue;
                        break;

                    case XOR:
                        for (j = 0; j < gate_list[count] -> variables-1; j += 1) {
                            var_name = gate_list[count] -> name[j];
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
                            output_value = first ^ second;
                            var_name = gate_list[count] -> name[2];
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                            gate_list[count] -> solved = 1;
                            num_solved += 1;
                        } else continue;
                        break;

                    case NOT:
                        var_name = gate_list[count] -> name[0];
                        key = hash_key(var_name);
                        output_value = hash_get(var_name, key, 1);
                        output_value = !output_value;
                        if (output_value != -1) {
                            var_name = gate_list[count] -> name[1];
                            // printf( "%s\n", var_name);
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            if (get_in == 1) hash_set(var_name, key, 1, output_value);
                            else hash_set(var_name, key, 0, output_value);
                            gate_list[count] -> solved = 1;
                            num_solved += 1;
                        } else continue;

                        break;

                    case PASS:
                        var_name = gate_list[count] -> name[0];
                        key = hash_key(var_name);
                        output_value = hash_get(var_name, key, 1);
                        output_value = !output_value;
                        if (output_value != -1) {
                            var_name = gate_list[count] -> name[1];
                            // printf( "%s\n", var_name);
                            key = hash_key(var_name);
                            get_in = hash_search(var_name, key, 1);
                            if (get_in == 1) hash_set(var_name, key, 1, output_value && 1);
                            else hash_set(var_name, key, 0, output_value);
                            gate_list[count] -> solved = 1;
                            num_solved += 1;
                        } else continue;

                        break;

                    case DECODER:

                        skip = 0;
                        for (j =0; j < gate_list[count] -> selectors; j += 1) {
                            var_name = gate_list[count] -> name[j];
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

                        for (j = 0; j < gate_list[count] -> selectors; j += 1) {
                            var_name = gate_list[count] -> name[j];
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
                        for (j = gate_list[count] -> selectors; j < gate_list[count] -> variables; j += 1) {
                            var_name = gate_list[count] -> name[j];
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

                        gate_list[count] -> solved = 1;
                        num_solved += 1;

                        break;

                    case MULTIPLEXER:

                        skip = 0;
                        for (j = 0; j < gate_list[count] -> variables-1; j += 1) {
                            var_name = gate_list[count] -> name[j];
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
                        for (j = pow(2, gate_list[count] -> selectors); j < gate_list[count] -> variables-1; j += 1) {
                            var_name = gate_list[count] -> name[j];
                            key = hash_key(var_name);
                            get_in = hash_get(var_name, key, 1);
                            get_out = hash_get(var_name, key, 0);
                            if (get_in != -1) {
                                var_value = get_in;
                            } else { var_value = get_out; }

                            section = section << 1;
                            section += var_value; 
                        }

                        var_name = gate_list[count] -> name[section];
                        key = hash_key(var_name);
                        value = hash_get(var_name, key, 1);
                        if (value == -1) { value = hash_get(var_name, key, 1); }
                        var_count = gate_list[count] -> variables-1;
                        var_name = gate_list[count] -> name[var_count];
                        key = hash_key(var_name);
                        if (hash_search(var_name, key, 1) == 1) {
                            hash_set(var_name, key, 1, value);
                        } else {
                            hash_set(var_name, key, 0, value);
                        }
                        gate_list[count] -> solved = 1;
                        num_solved += 1;

                        break;

                }
            }
        }

        for(j = 0; j < num_outputs; j += 1) {
            key = output_keys[j];
            printf(" %d", outputs -> table[key] -> var);
            outputs -> table[key] -> var = -1;
        }
        printf("\n");
        for (j = 0; j < num_temps; j += 1) {
            key = temp_vars[j];
            inputs -> table[key] -> var = -1;
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
    char buffer[256];
    int num_vars, i, key;
    create_table(1000, 1); // create inputs hashmap
    create_table(1000, 0); // create output hashmap
    

    // Inputs
    fscanf(ins, " %s", buffer);
    fscanf(ins, " %d", &num_inputs);
    input_keys = malloc(sizeof(int) * num_inputs);
    for(i = 0; i < num_inputs; i += 1) {
        fscanf(ins, " %s", buffer);
        key = hash_key(buffer);
        input_keys[i] = key;
        hash_insert(buffer, key, 1);
    }

    // Outputs
    fscanf(ins, " %s", buffer);
    fscanf(ins, " %d", &num_outputs);
    output_keys = malloc(sizeof(int) * num_outputs);
    for (i = 0; i < num_outputs; i += 1) {
        fscanf(ins, " %s", buffer);
        key = hash_key(buffer);
        output_keys[i] = key;
        hash_insert(buffer, key, 0);
    }


    while (fscanf(ins, " %16s", buffer) != EOF) {
        struct gate *gate;
        if (strcmp(buffer, "AND") == 0) {
            gate = malloc(sizeof(struct gate));
            gate -> op_type = AND;
            gate -> variables = 3;
            gate -> solved = 0;

            for (i = 0; i < 3; i += 1) {
                fscanf(ins, " %s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    hash_insert(buffer, key, 1);
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    hash_insert(buffer, key, 1);
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "OR") == 0) {
            gate = malloc(sizeof(struct gate));
            gate -> op_type = OR;
            gate -> variables = 3;
            gate -> solved = 0;

            for (i = 0; i < 3; i += 1) {
                fscanf(ins, " %s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    hash_insert(buffer, key, 1);
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    hash_insert(buffer, key, 1);
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "NAND") == 0) {
            gate = malloc(sizeof(struct gate));
            gate -> op_type = NAND;
            gate -> variables = 3;
            gate -> solved = 0;

            for (i = 0; i < 3; i += 1) {
                fscanf(ins, " %s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    hash_insert(buffer, key, 1);
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    hash_insert(buffer, key, 1);
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "NOR") == 0) {
            gate = malloc(sizeof(struct gate));
            gate -> op_type = NOR;
            gate -> variables = 3;
            gate -> solved = 0;

            for (i = 0; i < 3; i += 1) {
                fscanf(ins, " %s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    hash_insert(buffer, key, 1);
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    hash_insert(buffer, key, 1);
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "XOR") == 0) {
            gate = malloc(sizeof(struct gate));
            gate -> op_type = XOR;
            gate -> variables = 3;
            gate -> solved = 0;

            for (i = 0; i < 3; i += 1) {
                fscanf(ins, " %s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    hash_insert(buffer, key, 1);
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    hash_insert(buffer, key, 1);
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "NOT") == 0) {
            gate = malloc(sizeof(struct gate));
            gate -> op_type = NOT;
            gate -> variables = 2;
            gate -> solved = 0;

            for (i = 0; i < 2; i += 1) {
                fscanf(ins, " %s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    hash_insert(buffer, key, 1);
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (in_inputs == 0 && in_outputs == 0) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    hash_insert(buffer, key, 1);
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "PASS") == 0) {
            gate = malloc(sizeof(struct gate));
            gate -> op_type = PASS;
            gate -> variables = 2;
            gate -> solved = 0;

            for (i = 0; i < 2; i += 1) {
                fscanf(ins, " %s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    puts("poop");
                    hash_insert(buffer, key, 1);
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (in_inputs == 0 && in_outputs == 0) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    hash_insert(buffer, key, 1);
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "DECODER") == 0) {
            gate = malloc(sizeof(struct gate));
            gate -> op_type = DECODER;
            gate -> solved = 0;
            fscanf(ins, " %d", &num_vars);
            gate -> variables = num_vars + pow(2, num_vars);
            gate -> selectors = num_vars;
            for (i = 0; i < gate -> variables; i += 1) {
                fscanf(ins, " %s", buffer);
                key = hash_key(buffer); //THINK
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    hash_insert(buffer, key, 1);
                    hash_set(buffer, key, 1, buffer[0] - '0'); //THINK
                } else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    hash_insert(buffer, key, 1);
                }
                strcpy(gate -> name[i], buffer);
            }
            gate_list[num_gates] = gate;
            num_gates += 1;
        }

        else if (strcmp(buffer, "MULTIPLEXER") == 0) {
            gate = malloc(sizeof(struct gate));
            gate -> op_type = MULTIPLEXER;
            gate -> solved = 0;
            fscanf(ins, "%d", &num_vars);
            gate -> variables = pow(2, num_vars) +  num_vars + 1;
            gate -> selectors = num_vars;
            for (i = 0; i < gate -> variables; i += 1) {
                fscanf(ins, "%s", buffer);
                key = hash_key(buffer);
                int in_inputs = hash_search(buffer, key, 1);
                int in_outputs = hash_search(buffer, key, 0);
                if (isdigit(buffer[0]) != 0) {
                    hash_insert(buffer, key, 1);
                    hash_set(buffer, key, 1, buffer[0] - '0');
                }
                else if (!in_inputs && !in_outputs) {
                    temp_vars[num_temps] = key;
                    num_temps += 1;
                    hash_insert(buffer, key, 1);
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
