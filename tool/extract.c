#include <stdio.h>
#include <stdlib.h>

#define display_command_usage(command_name) fprintf(stderr, "Usage: %s FILENAME OFFSET AMOUNT\n", command_name)
#define OPEN_FILE(filename) fopen(filename, "rb")
#define READ_FROM_FILE(file, offset, buffer, length) fread(buffer, sizeof(char), length, file)
#define WRITE_TO_OUTPUT(buffer, length) fwrite(buffer, sizeof(char), length, stdout)

int main(int argc, char **argv) {
    FILE *file_pointer;
    char *buffer_pointer;
    size_t offset, length, bytes_read;

    if(argc != 4) {
        display_command_usage(argv[0]);
        return EXIT_FAILURE;
    }

    file_pointer = OPEN_FILE(argv[1]);
    if(file_pointer == NULL) {
        perror("fopen");
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

    if(fseek(file_pointer, offset, SEEK_SET) != 0) {
        perror("fseek");
        return EXIT_FAILURE;
    }

    bytes_read = READ_FROM_FILE(file_pointer, offset, buffer_pointer, length);
    if(ferror(file_pointer) != 0) {
        perror("fread");
        return EXIT_FAILURE;
    }

    if(bytes_read < length) {
        fprintf(stderr, "got only %zu of %zu bytes\n", bytes_read, length);
        return EXIT_FAILURE;
    } else {
        WRITE_TO_OUTPUT(buffer_pointer, length);
    }

    fclose(file_pointer);
    free(buffer_pointer);

    return EXIT_SUCCESS;
}