#include <stdio.h>
#include <stdint.h>

#define BUFF_SIZE 4096

FILE *open_file_rb(char *filename)
{
    FILE *file = fopen(filename, "rb");

    if (file == NULL)
    {
        fprintf(stderr, "Error opening %s\n", filename);
        return NULL;
    }
    return file;
}

void close_file(FILE *file)
{
    fclose(file);
    return;
}

uint64_t shift_8(uint64_t value)
{
    return value >> 8;
}

uint8_t isolate_lsb(uint64_t value)
{
    return value & 0xFF;
}

void process_address(uint64_t value, int add_idx)
{
    printf("Address %4d: 0x%016llx -> 0x%014llx : 0x%02llx\n", add_idx, value, shift_8(value), isolate_lsb(value));
}

long get_filesize(FILE *file)
{
    if (fseek(file, 0, SEEK_END) == 0)
    {
        long file_size = ftell(file);
        if (file_size != -1)
        {
            fseek(file, 0, SEEK_SET);
            return file_size;
        }
        else
        {
            fprintf(stderr, "ERROR getting file size\n");
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "ERROR seeking file\n");
        return -1;
    }
}

int main(int argc, char *argv[])
{
    int i;
    int j;
    FILE *file;
    uint64_t buffer[BUFF_SIZE];

    if (argc < 2)
    {
        fprintf(stderr, "ERROR USAGE: file1 file2 ... file n\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; ++i)
    {
        int j = 0;
        int add_idx = 1;
        file = open_file_rb(argv[i]);
        if (file != NULL)
        {
            long file_size = get_filesize(file);

            if (file_size > 0)
            {
                size_t file_read = fread(&buffer, sizeof(uint64_t), BUFF_SIZE, file);
                if (file_read > 0)
                {
                    for (j = 0; j < file_read; j++)
                    {
                        process_address(buffer[j], add_idx);
                        ++add_idx;
                    }
                }
                else if (ferror(file))
                {
                    fprintf(stderr, "ERROR: reading file %s into buffer\n", argv[i]);
                    continue;
                }
                close_file(file);
            }
        }
        else
        {
            fprintf(stderr, "ERROR: with open_file_rb");
        }
    }
    return 0;
}
