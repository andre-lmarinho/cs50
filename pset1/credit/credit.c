#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // Prompt user the number
    long n = get_long("Number: ");

    // Valitation Variable
    int v = 0;

    // Individual Numbers for validation
    int q = n % 10;
    int w = (n /10) % 10 * 2;
      if (w > 9) w = (w % 10) + ((w /10) % 10);
      else w = w + 0;
    int e = (n /100) % 10;
    int r = (n /1000) % 10 * 2;
      if (r > 9) r = (r % 10) + ((r /10) % 10);
      else r = r + 0;
    int t = (n /10000) % 10;
    int y = (n /100000) % 10 * 2;
      if (y > 9) y = (y % 10) + ((y /10) % 10);
      else y = y + 0;
    int u = (n /1000000) % 10;
    int i = (n /10000000) % 10 * 2;
      if (i > 9) i = (i % 10) + ((i /10) % 10);
      else i = i + 0;
    int o = (n /100000000) % 10;
    int p = (n /1000000000) % 10 * 2;
      if (p > 9) p = (p % 10) + ((p /10) % 10);
      else p = p + 0;
    int a = (n /10000000000) % 10;
    int s = (n /100000000000) % 10 * 2;
      if (s > 9) s = (s % 10) + ((s /10) % 10);
      else s = s + 0;
    int d = (n /1000000000000) % 10;
    int f = (n /10000000000000) % 10 * 2;
      if (f > 9) f = (f % 10) + ((f /10) % 10);
      else f = f + 0;
    int g = (n /100000000000000) % 10 ;
    int h = (n /1000000000000000) % 10 * 2;
      if (h > 9) h = (h % 10) + ((h /10) % 10);
      else h = h + 0;

 /*Print numbers (Check 01)
    printf("%d\n", q);
    printf("%d\n", w);
    printf("%d\n", e);
    printf("%d\n", r);
    printf("%d\n", t);
    printf("%d\n", y);
    printf("%d\n", u);
    printf("%d\n", i);
    printf("%d\n", o);
    printf("%d\n", p);
    printf("%d\n", a);
    printf("%d\n", s);
    printf("%d\n", d);
    printf("%d\n", f);
    printf("%d\n", g);
    printf("%d\n", h);*/

    //Check Valid Number
    int z = w + r + y + i + p + s + f + h;
    int x = z + q + e + t + u + o + a + d + g;
    x = x % 10;
      if (x == 0) v = 1;
    //printf("Validade: %d\n", v);

    //Printing Results
    if (v == 1 && (n /1000000000000000) % 10 == 4)
      printf("VISA\n");
    else if (v == 1 && (n /100000000000000) % 10 == 4)
      printf("VISA\n");
    else if (v == 1 && (n /10000000000000) % 10 == 4)
      printf("VISA\n");
    else if (v == 1 && (n /1000000000000) % 10 == 4)
      printf("VISA\n");

   else if (v == 1 && (n /1000000000000000) % 10 == 5 && (n /100000000000000) % 10 == 1)
      printf("MASTERCARD\n");
   else if (v == 1 && (n /1000000000000000) % 10 == 5 && (n /100000000000000) % 10 == 2)
      printf("MASTERCARD\n");
   else if (v == 1 && (n /1000000000000000) % 10 == 5 && (n /100000000000000) % 10 == 3)
      printf("MASTERCARD\n");
   else if (v == 1 && (n /1000000000000000) % 10 == 5 && (n /100000000000000) % 10 == 4)
      printf("MASTERCARD\n");
   else if (v == 1 && (n /1000000000000000) % 10 == 5 && (n /100000000000000) % 10 == 5)
      printf("MASTERCARD\n");

   else if (v == 1 && (n /100000000000000) % 10 == 3 && (n /10000000000000) % 10 == 4)
      printf("AMEX\n");
   else if (v == 1 && (n /100000000000000) % 10 == 3 && (n /10000000000000) % 10 == 7)
      printf("AMEX\n");

    else
      printf("INVALID\n");

}