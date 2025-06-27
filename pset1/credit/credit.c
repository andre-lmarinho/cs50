#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for credit card number
    long number = get_long("Number: ");

    // Variables for Luhn's algorithm
    int sum = 0;
    bool alternate = false;
    int digits = 0;
    long temp = number;

    // Loop through each digit of the card number
    while (temp > 0)
    {
        int digit = temp % 10;
        if (alternate)
        {
            // Multiply every second digit by 2
            digit *= 2;
            // If product is two digits, add them together (e.g., 12 -> 1 + 2)
            if (digit > 9)
            {
                digit = digit / 10 + digit % 10;
            }
        }
        // Add digit to running sum
        sum += digit;
        // Toggle flag so we process every other digit
        alternate = !alternate;
        // Move to next digit
        temp /= 10;
        digits++;
    }

    // If the final sum doesn't end in 0, it's invalid
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Determine the first one and two digits for card type checks
    temp = number;
    while (temp >= 100)
    {
        temp /= 10;
    }
    int first_two = temp;
    int first_one = temp / 10;

    // Identify card type by length and prefix
    if (digits == 15 && (first_two == 34 || first_two == 37))
    {
        printf("AMEX\n");
    }
    else if (digits == 16 && (first_two >= 51 && first_two <= 55))
    {
        printf("MASTERCARD\n");
    }
    else if ((digits == 13 || digits == 16) && first_one == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}
