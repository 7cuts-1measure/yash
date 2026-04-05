#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <err.h>
#include <string.h>

#define YASH_READLINE_BUFSIZE 1024

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define SB_INIT_CAPACITY 32

#define SV_Fmt "%.*s"
#define SV_Arg(sv) (int)(sv).count, (sv).data
typedef struct {
    const char *data;
    size_t count;
} StringView;

typedef struct {
    char *data;
    size_t count;
    size_t capacity;
} StringBuilder;

void sb_append(StringBuilder *sb, char c){
    assert(sb->count <= sb->capacity);
    assert((sb->data == NULL) <= (sb->capacity == 0));  // text==NULL --> capacity==0
    if (sb->count + 1 >= sb->capacity) {    // +1 for null-terminator
        size_t new_capacity = sb->capacity == 0 ? SB_INIT_CAPACITY : sb->capacity * 2;
        sb->data = (char *)realloc(sb->data, new_capacity * sizeof(sb->data[0]));
        if (!sb->data) err(EXIT_FAILURE, "StringBuilder reallocation failed");
        sb->capacity = new_capacity;
    }
    sb->data[sb->count++] = c;
    sb->data[sb->count] = '\0'; //TODO: do i really need a nulll-terminator
}

void sb_append_many(StringBuilder *sb, const char* text, int count) {
    for (int i = 0; i < count; i++) {
        sb_append(sb, text[i]);
    }
} 

void sb_append_string(StringBuilder *sb, const char* string) {
    if (!string) return;
    assert(sb != NULL);
    for (; *string != '\0'; ++string) {
        sb_append(sb, *string);
    }
} 


void sb_println(const StringBuilder* sb) {
    for (int i = 0; i < sb->count; ++i) {
        putchar(sb->data[i]);
    }
    putchar('\n');
}

void sb_remove_last(StringBuilder *sb) {
    assert(sb->count > 0);
    sb->count--;
}


void sb_free(StringBuilder *sb) {
    if (sb) {
        sb->capacity = 0;
        sb->count = 0;
        if (sb->data) free(sb->data);
        sb->data = NULL;
    }
}

void sb_test_yourself() {
    StringBuilder sb = {0};
    puts("> Test sb_append (69):"); {
        sb_append(&sb, '6');
        sb_append(&sb, '9');
        sb_println(&sb);   
    }    

    puts("> Test sb_remove_last(6)"); {
        sb_remove_last(&sb);
        sb_println(&sb);
    }

    puts (">Test sb_append_many(6keeek)"); {
        sb_append_many(&sb, "keeek", strlen("keeek"));
        sb_println(&sb);
    }

    puts("> Test sb_append_string(6keeekCOW)"); {
        sb_append_string(&sb, "COW");
        sb_println(&sb);
    }

    puts("> Test sb_append_string(6keeekCOW): NULL"); {
        sb_append_string(&sb, NULL);
        sb_println(&sb);
    }
}


StringView sv(const char *cstr) {
    return (StringView) {
        .data = cstr,
        .count = strlen(cstr)   
    };
} 
StringView sv_from_sb(const StringBuilder *sb) {
    return (StringView) {
        .data = sb->data,
        .count = sb->count   
    };
} 

void sv_chop_right(StringView *sv, size_t n) {
    if (n > sv->count) n = sv->count;
    sv->count -= n;
}

void sv_chop_left(StringView *sv, size_t n) {
    if (n > sv->count) n = sv->count;
    sv->data  += n;
    sv->count -= n;
}


StringView sv_chop_by_delim(StringView *sv, char delim) {
    size_t i = 0;
    for (; i < sv->count; ++i) {
        if (sv->data[i] == delim) break;
    }
    StringView result = {
        .data = sv->data,
        .count = i
    };
    sv_chop_left(sv, i + 1);
    return result;    
}

void sv_trim_left(StringView *sv) {
    while (sv->count > 0 && isspace(sv->data[0])) {
        sv_chop_left(sv, 1);
    }
}

void sv_trim_right(StringView *sv) {
    while (sv->count > 0 && isspace(sv->data[sv->count-1])) {
        sv_chop_right(sv, 1);
    }
}

void sv_trim(StringView *sv) {
    sv_trim_left(sv);
    sv_trim_right(sv);
}

/*
 *  Read the command from stdin
 *  Parse line (separate arguments and program) 
 *  Execute program
*/
char *yash_readline() {
    size_t bufsize = YASH_READLINE_BUFSIZE;
    char *buf = malloc(bufsize * sizeof(char));
    if (!buf) {
        err(EXIT_FAILURE, "Memory allocation failed");
    }

    int c;
    int cnt;
    while (true) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            buf[cnt] = '\0';
            return buf;
        }
        buf[cnt++] = c;
        if (cnt >= bufsize) {
            bufsize *= 2;
            buf = realloc(buf, bufsize);
            if (!buf) {
                err(EXIT_FAILURE, "Memory reallocation failed");
            }
        } 
    }
}

char **yash_parse_line(const char *line) {

}

int yash_exec(char **args) {

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
    
    //yash_loop();

    // clean up
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    StringView view = sv("Hello, world");
    StringView tok = sv_chop_by_delim(&view, '!');
    
    printf(SV_Fmt"\n", SV_Arg(tok));
    printf(SV_Fmt"\n", SV_Arg(view));
    return EXIT_SUCCESS;
}



