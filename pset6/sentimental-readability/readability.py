import math


def main():
    # Prompt user and trim whitespace begin/end
    text = input("Text: ").strip()

    # Counters
    letters = sum(1 for c in text if c.isalpha())
    words = len(text.split())  # handles extra spaces between
    sentences = sum(1 for c in text if c in (".", "?", "!"))

    # Calculate averages
    L = letters / words * 100
    S = sentences / words * 100

    # Compute coleman-Liau index
    index = round(0.0588 * L - 0.296 * S - 15.8)

    # Print grade
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


# Start
if __name__ == "__main__":
    main()
