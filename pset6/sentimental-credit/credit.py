def main():
    # Prompt for card number
    while True:
        num_str = input("Number: ").strip()
        if num_str.isdigit():
            break

    # Convert to list
    digits = [int(ch) for ch in num_str]

    # Luhn's algorithm
    # Sum of digits not multiplied
    sum1 = sum(digits[-1::-2])
    # Sum of digits after doubling every other digit
    sum2 = 0
    for d in digits[-2::-2]:
        product = d * 2
        sum2 += product // 10 + product % 10

    total = sum1 + sum2

    # If total's last digit isn't 0, not valid
    if total % 10 != 0:
        print("INVALID")
        return

    # Find card type
    length = len(digits)
    first_two = int(num_str[:2])
    first_one = int(num_str[0])

    if first_one == 4 and length in (13, 16):
        print("VISA")
    elif 51 <= first_two <= 55 and length == 16:
        print("MASTERCARD")
    elif first_two in (34, 37) and length == 15:
        print("AMEX")
    else:
        print("INVALID")


# Start
if __name__ == "__main__":
    main()
