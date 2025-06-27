#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means there is an edge from i to j in the graph
bool locked[MAX][MAX];

// Each pair has a winner and a loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidate names
string candidates[MAX];

// Array of all pairs, and counts for both
pair pairs[MAX * (MAX - 1) / 2];
int pair_count = 0;
int candidate_count = 0;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
bool creates_cycle(int winner, int loser);
void lock_pairs(void);
void print_winner(void);
int compare_pairs(const void *a, const void *b);

int main(int argc, string argv[])
{
    // Validate usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate candidates array
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear locked graph
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    // Query number of voters
    int voter_count = get_int("Number of voters: ");

    // Process each voter's preferences
    for (int v = 0; v < voter_count; v++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Prompt for each rank
        for (int r = 0; r < candidate_count; r++)
        {
            string name = get_string("Rank %i: ", r + 1);
            if (!vote(r, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        // Update global preferences matrix
        record_preferences(ranks);
        printf("\n");
    }

    // Create, sort, and lock pairs, then output winner
    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Record a single vote, updating ranks array
bool vote(int rank, string name, int ranks[])
{
    // Look for candidate name
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            // Store candidate index at this preference rank
            ranks[rank] = i;
            return true;
        }
    }
    // Name not found
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // For each pair i, j where i is preferred over j, increment preference
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Populate pairs array with all winning pairs
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int vote_ij = preferences[i][j];
            int vote_ji = preferences[j][i];
            if (vote_ij > vote_ji)
            {
                pairs[pair_count++] = (pair) {.winner = i, .loser = j};
            }
            else if (vote_ji > vote_ij)
            {
                pairs[pair_count++] = (pair) {.winner = j, .loser = i};
            }
            // Ties are skipped
        }
    }
}

// Compare function for qsort: sort by margin descending
int compare_pairs(const void *a, const void *b)
{
    pair *p1 = (pair *) a;
    pair *p2 = (pair *) b;
    int margin1 = preferences[p1->winner][p1->loser] - preferences[p1->loser][p1->winner];
    int margin2 = preferences[p2->winner][p2->loser] - preferences[p2->loser][p2->winner];
    return margin2 - margin1;
}

// Sort pairs by strength of victory in descending order
void sort_pairs(void)
{
    qsort(pairs, pair_count, sizeof(pair), compare_pairs);
}

// Recursive check to see if locking winner→loser creates a cycle
bool creates_cycle(int winner, int loser)
{
    // Base case: if loser already beats winner, cycle found
    if (locked[loser][winner])
    {
        return true;
    }
    // Recurse through graph
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i] && creates_cycle(winner, i))
        {
            return true;
        }
    }
    return false;
}

// Lock pairs into graph without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int w = pairs[i].winner;
        int l = pairs[i].loser;
        // Only lock if it does not create a cycle
        if (!creates_cycle(w, l))
        {
            locked[w][l] = true;
        }
    }
}

// Print the winner: the candidate with no incoming edges
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool has_incoming = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                has_incoming = true;
                break;
            }
        }
        if (!has_incoming)
        {
            // Candidate i is the source of the graph
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
