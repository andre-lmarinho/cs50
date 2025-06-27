import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # TODO: Read database file into a variable
    database_filename = sys.argv[1]
    with open(database_filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        # Extract STR names from the CSV header
        str_names = reader.fieldnames[1:]  # skip name column
        # Load individuals on memory
        database = list(reader)

    # TODO: Read DNA sequence file into a variable
    sequence_filename = sys.argv[2]
    with open(sequence_filename, 'r') as seqfile:
        dna_sequence = seqfile.read().strip()

    # TODO: Find longest match of each STR in DNA sequence
    str_counts = {}
    for subseq in str_names:
        str_counts[subseq] = longest_match(dna_sequence, subseq)

    # TODO: Check database for matching profiles
    for person in database:
        # Convert STR count in person record to int and compare
        if all(int(person[subseq]) == str_counts[subseq] for subseq in str_names):
            print(person["name"])
            return  # Found > Exit

    # Not Found
    print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


# Start
if __name__ == "__main__":
    main()
