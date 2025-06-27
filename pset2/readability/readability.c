#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int main(void)
{

    //Prompt User
    string text = get_string("Text: ");

    //Counting variables
    int letters = 0;
    int words = 1;
    int sentences = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        //Letters
        if (isalpha(text[i]))
        {
            letters++;
        }

        //Words
        // BUG: Empty spaces on the end of the sentence will "inflate" the score
        else if (text[i] == ' ')
        {
            words++;
        }


        //Sentences
        else if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentences++;
        }

    }


    //Calculating Grade Level

    float L = letters / (float) words * 100;
    float S = sentences / (float) words * 100;

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    //Print Grade Level


    if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (index > 16)
    {
        printf("Grade 16+\n");
    }

    else
    {
        printf("Grade %i\n", index);
    }

} 