#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

// Function prototypes
void    check_argc(int argc);
void    check_file(FILE *file, const char *msg);
int     is_jpeg(BYTE buffer[]);

int main(int argc, char *argv[])
{
    // Validate command-line arguments
    check_argc(argc);

    // Open input file
    FILE *raw = fopen(argv[1], "r");
    check_file(raw, "Could not open input file");

    BYTE   buffer[BLOCK_SIZE];
    char   filename[8];
    FILE  *image = NULL;
    int    file_count = 0;

    // Read 512-byte blocks until end of file
    while (fread(buffer, BLOCK_SIZE, 1, raw) == 1)
    {
        // If block is start of a new JPEG
        if (is_jpeg(buffer))
        {
            // Close previous JPEG, if any
            if (image != NULL)
            {
                fclose(image);
            }

            // Generate filename: 000.jpg, 001.jpg, etc.
            snprintf(filename, sizeof filename, "%03i.jpg", file_count++);

            // Open new output file
            image = fopen(filename, "w");
            check_file(image, "Could not create output file");

            // Write this block to the new file
            fwrite(buffer, BLOCK_SIZE, 1, image);
        }
        else if (image != NULL)
        {
            // Already found a JPEG—keep writing blocks to it
            fwrite(buffer, BLOCK_SIZE, 1, image);
        }
    }

    // Close any remaining open files
    if (image != NULL)
    {
        fclose(image);
    }
    fclose(raw);

    return 0;
}

// Ensure correct usage
void check_argc(int argc)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        exit(1);
    }
}

// Ensure file opened successfully
void check_file(FILE *file, const char *msg)
{
    if (file == NULL)
    {
        fprintf(stderr, "%s\n", msg);
        exit(2);
    }
}

// Return 1 if buffer contains a JPEG signature
int is_jpeg(BYTE buffer[])
{
    return (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
           (buffer[3] & 0xf0) == 0xe0);
}
