#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <err.h>
#include <string.h>

#define DA_INIT_CAPACITY 32

#define da_append(da, elem) do {                                                            \
    assert(da.count <= da.capacity);                                                      \
    assert((da.data == NULL) <= (da.capacity == 0));                                      \
    if (da.count == da.capacity) {                                                        \
        size_t new_capacity = da.capacity == 0 ? DA_INIT_CAPACITY : da.capacity * 2;      \
        da.data = realloc(da.data, new_capacity * sizeof(da.data[0]));                   \
        if (!da.data) err(EXIT_FAILURE, "Dynamic array reallocation failed");              \
        da.capacity = new_capacity;                                                        \
    }                                                                                       \
    da.data[da.count++] = elem;                                                           \
} while (0)


#define YASH_READLINE_BUFSIZE 128

/**
 * Reads one line and allocates memory for it. 
 * Caller should free memory.
 */
char *yash_readline() {
    size_t bufsize = YASH_READLINE_BUFSIZE;
    char *buf = malloc(bufsize * sizeof(char));
    if (!buf) {
        err(EXIT_FAILURE, "Memory allocation failed");
    }

    int c;
    int cnt = 0;
    while (true) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            buf[cnt] = '\0';
            return buf;
        }
        buf[cnt++] = c;
        if (cnt == bufsize) {
            bufsize *= 2;
            buf = realloc(buf, bufsize);
            if (!buf) {
                err(EXIT_FAILURE, "Memory reallocation failed");
            }
        } 
    }

    buf[cnt] = '\0';
    printf("read: %s\n", buf);
    return buf;
}


/**
 * Parses line into array of strings.
 * Caller should free memory (only char** array because char* elems)
 * are pointers to the line
 */
char **yash_parse_line(char *line) {
    struct {
        char **data;
        int count;
        int capacity;
    } args = {0};
    

    bool word_start = true;
    for (char *c = line; *c != '\0'; c++) {
        if (word_start && !isspace(*c)) {
            da_append(args, c);
            word_start = false;
        }
        switch (*c) {
            case ' ':
            case '\t':
                *c = '\0';
                if (!isspace(*(c + 1)))   {
                    word_start = true; 
                }
                break;
            default:
                continue;
        }
    }


    printf("parsed:  [");
    for (int i = 0; i < args.count; i++) {
        printf("'%s'%s", args.data[i], i != args.count - 1 ? ", " : "");
    }
    printf("]\n");
  
  
    da_append(args, NULL);
    return args.data;
}


int yash_exec(char **args) {
    return 0;
}


void yash_loop() {
    
    bool is_exit;
    do {
        printf("> ");
        char *line = yash_readline();
        char **argv  = yash_parse_line(line);
        is_exit = yash_exec(argv);

        free(argv);
        free(line);
    } while (!is_exit);
}


int yash_main(int argc, char *argv[]) {
    // load config files
    
    yash_loop();

    // clean up
    return EXIT_SUCCESS;
}


int main(int argc, char *argv[]) {

    yash_main(argc, argv);
    return EXIT_SUCCESS;
}



