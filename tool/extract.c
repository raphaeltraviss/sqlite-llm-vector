#include <stdio.h>
#include <stdlib.h>

void display_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s FILENAME OFFSET AMOUNT\n", program_name);
}

FILE* open_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if(file == NULL) {
        perror("fopen");
    }
    return file;
}

size_t read_from_file(FILE *f, size_t offset, char *buffer, size_t n) {
    if(fseek(f, offset, SEEK_SET) != 0) {
        perror("fseek");
        return 0;
    }

    size_t bytes_read = fread(buffer, sizeof(char), n, f);
    if(ferror(f) != 0) {
        perror("fread");
        return 0;
    }

    return bytes_read;
}

void write_to_output(const char *buffer, size_t n) {
    fwrite(buffer, sizeof(char), n, stdout);
}

int main(int argc, char **argv) {
    FILE *f;
    char *zBuf;
    size_t ofst, n, got;

    if(argc != 4) {
        display_usage(argv[0]);
        return EXIT_FAILURE;
    }

    f = open_file(argv[1]);
    if(f == NULL) {
        return EXIT_FAILURE;
    }

    ofst = (size_t)strtol(argv[2], NULL, 10);
    n = (size_t)strtol(argv[3], NULL, 10);

    if(n == 0) {
        fprintf(stderr, "invalid amount of bytes\n");
        return EXIT_FAILURE;
    }

    zBuf = malloc(n);

    if(zBuf == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    got = read_from_file(f, ofst, zBuf, n);

    if(got < n) {
        fprintf(stderr, "got only %zu of %zu bytes\n", got, n);
        return EXIT_FAILURE;
    } else {
        write_to_output(zBuf, n);
    }

    fclose(f);
    free(zBuf);

    return EXIT_SUCCESS;
}