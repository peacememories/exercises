#include<stdlib.h>
#include<stdio.h>
#include<getopt.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>
#include<stdbool.h>

#define MAX_LINE_LENGTH (100)
#define EXIT_ERROR (-1)
#define print_error(argstr, ...) do { (void)fprintf(stderr, "%s: " argstr ": %s\n", argv[0], __VA_ARGS__, strerror(errno)); } while(0)

static bool ignore_case;
static char* file1_name;
static char* file2_name;

/**
 * @brief Parse arguments and set globals
 *
 * Uses `getopt` to parse the command line arguments.
 *
 * Sets `ignore_case` if `-i` is part of the argument list.
 * Sets `file1_name` and `file2_name`
 *
 * Errors: If unrecognized flags are entered or there is not
 * exactly two file names supplied sets errno to EINVAL
 * (Invalid Argument)
 *
 * See `man getopt` for information on the getopt command.
 *
 * `optind` contains the first argument not consumed by getopt
 * so in an argument array ["progname", "-i", "file1", "file2"]
 * it would point to "file1". This way we can extract the two
 * file names. Also because of this, `argc-optind` is the number
 * of non-flag arguments. The function checks if this is exactly
 * 2 (two file names).
 *
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return EXIT_SUCCESS on success, EXIT_ERROR on failure
 */
int parse_args(int argc, char* const argv[]) {
    ignore_case = false;

    extern int optind;

    for(int c; (c=getopt(argc, argv, "i")) != -1;) {
        switch(c) {
            case 'i':
                ignore_case = true;
                break;
            default:
                errno = EINVAL;
                return -1;
        }
    }
    if(argc-optind != 2) {
        errno = EINVAL;
        return -1;
    }
    
    file1_name = argv[optind];
    file2_name = argv[optind+1];
    return 0;
}

/**
 * @brief Reads two lines and compares them
 *
 * @param 
 */
int compare_lines(const char* line_1, const char* line_2) {
    unsigned int differences = 0;
    for(unsigned int i = 0; i < MAX_LINE_LENGTH+1; i++) {
        if(*line_1 == '\n' || *line_2 == '\n') {
            return differences;
        }
        if(ignore_case) {
            if(tolower(*line_1) != tolower(*line_2)) {
                differences++;
            }
        } else {
            if(*line_1 != *line_2) {
                differences++;
            }
        }
        line_1++;
        line_2++;
    }
    errno = EMSGSIZE;
    return -1;
}

int compare_files(FILE* file_1, FILE* file_2) {
    char* line_1 = malloc((MAX_LINE_LENGTH+2)*sizeof(char));
    if(line_1 == NULL) {
        return -1;
    }
    char* line_2 = malloc((MAX_LINE_LENGTH+2)*sizeof(char));
    if(line_2 == NULL) {
        return -1;
    }

    unsigned int line_no = 1;
    while(!feof(file_1) && !feof(file_1)) {
        if(fgets(line_1, MAX_LINE_LENGTH+1, file_1) == NULL) {
            return -1;
        }
        if(fgets(line_2, MAX_LINE_LENGTH+1, file_2) == NULL) {
            return -1;
        }
        int comparison = compare_lines(line_1, line_2);
        if(comparison == -1) {
            return -1;
        }
        if(comparison > 0) {
            (void)printf("Zeile: %d Zeichen: %d\n", line_no, comparison);
        }
        line_no++;
    }
    return 0;
}

int main(int argc, char* const argv[]) {
    if(parse_args(argc, argv) == -1) {
        (void)fprintf(stderr, "Usage: %s [-i] file1 file2\n", argv[0]);
        return EXIT_FAILURE;
    }
    FILE* file1 = fopen(file1_name, "r");
    if(file1 == NULL) {
        print_error("Could not open %s", file1_name);
        return EXIT_FAILURE;
    }
    FILE* file2 = fopen(file2_name, "r");
    if(file2 == NULL) {
        print_error("Could not open %s", file2_name);
        return EXIT_FAILURE;
    }
    if(compare_files(file1, file2) == -1) {

    };
    return EXIT_SUCCESS;
}