#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long int address;
char operation;
long int **cache, **p_cache;
int cache_size, block_size;
int assoc_n = 0, tag_size = 0, log_block = 0, log_tag = 0;
int reads = 0, writes = 0, hits = 0, misses = 0;
int p_reads = 0, p_writes = 0, p_hits = 0, p_misses = 0;

int is_pow_2(int num) {
    if (num == 0) return 0;
    while (num != 1) {
        if (num%2 != 0) return 0;
        num = num/2;
    }
    return 1;
}

int main(int argc, char **argv) {

    if (argc != 6) {
        printf("ERROR: Incorrect Number of Arguments\n");
        return 1;
    }

    if (!is_pow_2(atoi(argv[1])) || !is_pow_2(atoi(argv[4]))) {
        puts("Bad size");
        return 1;
    }


    cache_size = atoi(argv[1]); 
    block_size = atoi(argv[4]); 
    
    


    switch(argv[2][0]) {

        case 'd':
            assoc_n = 1;
            tag_size = cache_size / (block_size * assoc_n);
            log_block = log2(block_size), log_tag = log2(tag_size);
            break;

        case 'a':

            if (argv[2][5] == ':') {
                sscanf(argv[2], "assoc:%d", &assoc_n);

                if (!is_pow_2(assoc_n)) {
                    puts("Bad associativity");
                    return 1;
                }
                tag_size = cache_size / (block_size * assoc_n);
                log_block = log2(block_size), log_tag = log2(tag_size);
            } else {
                tag_size = 1;
                assoc_n = cache_size / (block_size * tag_size);
                log_block = log2(block_size), log_tag = 0;
            }
            break;
    }


    int broke_size = assoc_n * tag_size;
    cache = malloc(broke_size * sizeof(long int *)), p_cache = malloc(broke_size * sizeof(long int *));


    int i;
    for (i = 0; i < broke_size; i += 1) {
        cache[i] = malloc(3 * sizeof(long int)), p_cache[i] = malloc(3 * sizeof(long int));
        cache[i][0] = 0, p_cache[i][0] = 0;
        cache[i][1] = 0, p_cache[i][1] = 0;
        cache[i][2] = 0, p_cache[i][2] = 0;
    }


    FILE *trace = fopen(argv[5], "r");

    for (;;) {
        fscanf(trace, "%*x");
        fscanf(trace, " %c %c", &operation, &operation);
        fscanf(trace, "%lx\n", &address);
        if (operation == 'e') {
            break;
        }

        long int address_loc = address >> log_block;
        long int loc = address_loc & ((1 << log_tag) - 1);
        long int tag = address_loc >> log_tag;
        int si, p_found, found;

       

        si = (loc * assoc_n), p_found = 0;
        for (i = si; i < si + assoc_n; i += 1) {
            if (p_cache[i][0] == 1 && p_cache[i][1] == tag) p_found = 1;
        }

        si = (loc * assoc_n), found = 0;
        for (i = si; i < si + assoc_n; i += 1) {
            if (cache[i][0] == 1 && cache[i][1] == tag) found = 1;
        }


        if (found) {
            hits += 1;
        }
        else {
            misses += 1;
            reads += 1;
            si = (assoc_n * loc);
            found = -1;
            for (i = si; i < si + assoc_n; i += 1) {
                if (cache[i][0] == 0) found = i;
                else cache[i][2] += 1;
            }

            if (found >= 0) {
                cache[found][0] = 1;
                cache[found][1] = tag;
            }
            else {
            
                int oldest = 0, old_i = -1;
                for (i = si; i < si + assoc_n; i += 1) {
                    if (cache[i][0] == 1 && cache[i][2] > oldest) {
                        oldest = cache[i][2];
                        old_i = i;
                    }
                }
                cache[old_i][0] = 1;
                cache[old_i][1] = tag;
                cache[old_i][2] = 0;
            }
        }

        if (p_found) {
            p_hits += 1;
        }
        else {
            p_misses += 1;
            p_reads += 1;
            si = (assoc_n * loc);
            found = -1;
            for (i = si; i < si + assoc_n; i += 1) {
                if (p_cache[i][0] == 0) found = i;
                else p_cache[i][2] += 1;
            }

            if (found >= 0) {
                p_cache[found][0] = 1;
                p_cache[found][1] = tag;
            }
            else {
            
                int oldest = 0, old_i = -1;
                for (i = si; i < si + assoc_n; i += 1) {
                    if (p_cache[i][0] == 1 && p_cache[i][2] > oldest) {
                        oldest = p_cache[i][2];
                        old_i = i;
                    }
                }


                p_cache[old_i][0] = 1;
                p_cache[old_i][1] = tag;
                p_cache[old_i][2] = 0;
            }
            
            long preId = (address + block_size) >> log_block;
            loc = preId & ((1 << log_tag) - 1);
            tag = preId >> log_tag;
           

            si = (loc * assoc_n), p_found = 0;
            for (i = si; i < si + assoc_n; i += 1) {
                if (p_cache[i][0] == 1 && p_cache[i][1] == tag) p_found = 1;
            }

            if (!p_found) {
                p_reads += 1;
                si = (assoc_n * loc);
                found = -1;
                for (i = si; i < si + assoc_n; i += 1) {
                    if (p_cache[i][0] == 0) found = i;
                    else p_cache[i][2] += 1;
                }

                if (found >= 0) {
                    p_cache[found][0] = 1;
                    p_cache[found][1] = tag;
                }
                else {
                
                    int oldest = 0, old_i = -1;
                    for (i = si; i < si + assoc_n; i += 1) {
                        if (p_cache[i][0] == 1 && p_cache[i][2] > oldest) {
                            oldest = p_cache[i][2];
                            old_i = i;
                        }
                    }


                    p_cache[old_i][0] = 1;
                    p_cache[old_i][1] = tag;
                    p_cache[old_i][2] = 0;
                }    
            }
        }


        if (operation == 'W') {
            writes += 1;
            p_writes += 1;
        }
    
    }


    puts("Prefetch 0");
    printf("Memory reads: %d\n", reads);
    printf("Memory writes: %d\n", writes);
    printf("Cache hits: %d\n", hits);
    printf("Cache misses: %d\n", misses);
    
    puts("Prefetch 1");
    printf("Memory reads: %d\n", p_reads);
    printf("Memory writes: %d\n", p_writes);
    printf("Cache hits: %d\n", p_hits);
    printf("Cache misses: %d\n", p_misses);


    for (i = 0; i < broke_size; i += 1) {
        free(cache[i]);
        free(p_cache[i]);
    }
    free(cache);
    free(p_cache);


    return 0;
}