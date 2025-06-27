#include <cs50.h>
#include <stdio.h>

int main(void)
{
    /* h = Altura Variavel / h0 = Altura Inicial / Z= Espaços / x = Limite do Prog. / y = Minimo do Prog.*/
    int x = 8;
    int y = 1;
    int h0 = 1;
    int z = x + 1;
    int h = 0;

    while (h > x || h < y)
    {
        h = get_int("Who tall boss: ");
        z = h - 1;
    }

    /* Check variables - Usable for Debugging
    printf ("Height=%d\n", h);
    printf ("x=%d\n", x);
    printf ("z=%d\n", z);
    printf ("\n");*/


    /* Loop to create pyramids */
    do
    {
        printf("%.*s", z, "         ");
        printf("%.*s", h0, "#########");
        printf("  ");
        printf("%.*s", h0, "#########");
        printf("\n");
        ++h0;
        --z;
    }
    while (h0 <= h);

}