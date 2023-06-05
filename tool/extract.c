#include <stdio.h>
#include <stdlib.h>

/* Function to display usage information */
void display_usage(char *program_name){
    fprintf(stderr, "Usage: %s FILENAME OFFSET AMOUNT\n", program_name);
}

/* Function to open file */
FILE *open_file(char *filename){
    FILE *file = fopen(filename, "rb");
    if(file == NULL){
        fprintf(stderr, "cannot open \"%s\"\n", filename);
    }
    return file;
}

/* Function to read bytes from file */
size_t read_from_file(FILE *f, int offset, char *buffer, int n){
    fseek(f, offset, SEEK_SET);
    return fread(buffer, 1, n, f);
}

/* Function to write bytes to output */
void write_to_output(char *buffer, int n){
    fwrite(buffer, 1, n, stdout);
}

int main(int argc, char **argv){
    FILE *f;
    char *zBuf;
    int ofst, n;
    size_t got;

    if(argc!=4){
        display_usage(*argv);
        return 1;
    }

    f = open_file(argv[1]);
    if(f == NULL){
        return 1;
    }

    ofst = atoi(argv[2]);
    n = atoi(argv[3]);
    if(n <= 0){
        fprintf(stderr, "invalid amount of bytes\n");
        return 1;
    }

    zBuf = malloc(n);
    if(zBuf == NULL){
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    got = read_from_file(f, ofst, zBuf, n);
    fclose(f);

    if(got < n){
        fprintf(stderr, "got only %d of %d bytes\n", (int)got, n);
        return 1;
    }else{
        write_to_output(zBuf, n);
    }

    free(zBuf);
    return 0;
}