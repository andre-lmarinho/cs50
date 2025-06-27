#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{

//Get key and Validate the Key
    //argv = 2
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    //argv 1 = Only Numbers
    //Create a loop to determine the total amount of numbers then Use ! to create a Matrix of numbers to be checked
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    //Convert argv 1 from Int to Double
    int k = atoi(argv[1]);


//Get plaintext
    string plaintext = get_string("Plaintext: ");

//Encipher and Print ciphertext

    printf("Ciphertext: ");

    //Start at 0
    //Create a loop of 26 (MOD: % 26)
    //Convert the loop for ASCII value

    for (int j = 0; j < strlen(plaintext); j++)
    {
        //For uppercase
        if (isupper(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 65 + k) % 26 + 65);
        }

        //For lowercase
        else if (islower(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 97 + k) % 26 + 97);
        }

        //For else
        else
        {
            printf("%c", plaintext[j]);
        }

    }

    //Create a new line
    printf("\n");

}