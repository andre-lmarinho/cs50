#include <stdio.h>
#include <cs50.h>

int main (void)
{
    string name = get_string("What's your name? ");
    printf("Hey yo!, %s\n", name);
}