def main():
    # set height bounds
    MAX_HEIGHT = 8
    MIN_HEIGHT = 1

    # Prompt for height until valid input
    height = 0
    while height < MIN_HEIGHT or height > MAX_HEIGHT:
        try:
            height = int(input("Who tall boss: "))
        except ValueError:
            height = 0

    # Build and print pyramid
    for row in range(1, height + 1):
        spaces = height - row
        blocks = "#" * row
        print(" " * spaces + blocks + "  " + blocks)


if __name__ == "__main__":
    main()
