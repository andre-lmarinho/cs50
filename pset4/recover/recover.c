#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

void isArgument(int argc);
void fileExists(FILE *file);
int isJPEG(BYTE buffer[]);

int main(int argc, char *argv[])
{

    //Input validation process
    isArgument(argc);
    FILE *raw = fopen(argv[1], "r");
    fileExists(raw);

    //Memory allocation
    BYTE buffer[BLOCK_SIZE];

    //Buffer the files names for the new files
    char filename[8];
    FILE *image;
    int i = 0;

    //PROCESS images after the first one is founded
    while (fread(buffer, BLOCK_SIZE, 1, raw) == 1)
    {
        // If the first is a JPEG
        if (isJPEG(buffer) == 1)
        {
            //Close last image if not the first JPEG block
            if (i != 0)
            {
                fclose(image);
            }

            //Managing the file
            sprintf(filename, "%03i.jpg", i++);
            image = fopen(filename, "w");
            fwrite(buffer, BLOCK_SIZE, 1, image);
        }

        // if not...
        else if (i > 0)
        {
            fwrite(buffer, BLOCK_SIZE, 1, image);
        }

    }
}

// Check argument has 2 count
void isArgument(int argc)
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        exit(1);
    }
}

// Check if the file exist
void fileExists(FILE *file)
{
    if (file == NULL)
    {
        printf("File could not be openned.\n");
        exit(2);
    }

}

// Check if is JPEG
int isJPEG(BYTE buffer[])
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return 1;
    }
    return 0;

}