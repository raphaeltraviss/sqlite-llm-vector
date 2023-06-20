#include <stdio.h>
#include <stdlib.h>

void display_command_usage(const char *command_name) {
    fprintf(stderr, "Usage: %s FILENAME OFFSET AMOUNT\n", command_name);
}

FILE* open_file(const char *filename) {
    FILE *file_pointer = fopen(filename, "rb");
    if(file_pointer == NULL) {
        perror("fopen");
    }
    return file_pointer;
}

size_t read_from_file(FILE *file, size_t offset, char *buffer, size_t length) {
    if(fseek(file, offset, SEEK_SET) != 0) {
        perror("fseek");
        return 0;
    }

    size_t bytes_read = fread(buffer, sizeof(char), length, file);
    if(ferror(file) != 0) {
        perror("fread");
        return 0;
    }

    return bytes_read;
}

void write_to_output(const char *buffer, size_t length) {
    fwrite(buffer, sizeof(char), length, stdout);
}

int main(int argc, char **argv) {
    FILE *file_pointer;
    char *buffer_pointer;
    size_t offset, length, bytes_read;

    if(argc != 4) {
        display_command_usage(argv[0]);
        return EXIT_FAILURE;
    }

    file_pointer = open_file(argv[1]);
    if(file_pointer == NULL) {
        return EXIT_FAILURE;
    }

    offset = (size_t)strtol(argv[2], NULL, 10);
    length = (size_t)strtol(argv[3], NULL, 10);

    if(length == 0) {
        fprintf(stderr, "invalid amount of bytes\n");
        return EXIT_FAILURE;
    }

    buffer_pointer = malloc(length);

    if(buffer_pointer == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    bytes_read = read_from_file(file_pointer, offset, buffer_pointer, length);

    if(bytes_read < length) {
        fprintf(stderr, "got only %zu of %zu bytes\n", bytes_read, length);
        return EXIT_FAILURE;
    } else {
        write_to_output(buffer_pointer, length);
    }

    fclose(file_pointer);
    free(buffer_pointer);

    return EXIT_SUCCESS;
}