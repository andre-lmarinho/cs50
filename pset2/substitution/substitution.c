#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{

//Get and Validate Key

    //argv = 2
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //making argv a variable
    string key = argv[1];

    //argv 1 = Only letters
    for (int i = 0; i < strlen(key); i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
    }

    //argv 1 = 26 letters
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    //argv 1 = Only Unic letters
    for (int i = 0; i < strlen(key); i++)
    {
        for (int j = i + 1; j < strlen(key); j++)
        {
            if ((toupper(key[i])) == (toupper(key[j])))
            {
                printf("Usage: ./substitution key\n");
                return 1;
            }
        }
    }


//Get plaintext
    string plaintext = get_string("Plaintext: ");

//Prep for Encipher - > Convert to uppercase

    for (int i = 0; i < strlen(key); i++)
    {
        if (islower(key[i]))
        {
            key[i] = key[i] - 32;
        }
    }

//Encipher

    printf("ciphertext: ");

    for (int i = 0; i < strlen(plaintext); i++)
    {
        //For uppercase
        if (isupper(plaintext[i]))
        {
            int cipher = plaintext[i] - 65;
            printf("%c", key[cipher]);
        }

        //For lowercase
        else if (islower(plaintext[i]))
        {
            int cipher = plaintext[i] - 97;
            printf("%c", key[cipher] + 32);
        }

        //For else
        else
        {
            printf("%c", plaintext[i]);
        }

    }

//Print cipher

    printf("\n");

}