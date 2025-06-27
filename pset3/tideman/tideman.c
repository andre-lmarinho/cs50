#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
bool no_loop(int winner, int loser);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
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

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //Check candidate name, If found, update
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; i < candidate_count; j++)
        {
            // Compare candidates
            if (preferences[i][j] > preferences [j][i])
            {
                pair new_pair = {i, j};
                pairs[pair_count] = new_pair;
                pair_count ++;
            }
        }
    }

    return;
}

// Making a new int for weight the diferences

int compare(const void *elem1, const void *elem2)
{
    pair f = *((pair *) elem1);
    pair s = *((pair *) elem2);
    int fir_marg = preferences[f.winner][f.loser] - preferences[f.loser][f.winner];
    int sec_marg = preferences[s.winner][s.loser] - preferences[s.loser][s.winner];
    return sec_marg - fir_marg;

}


// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{

    qsort(pairs, pair_count, sizeof(pair), compare);
    return;

}

// Check if candidate graph is creating a loop
bool no_loop(int winner, int loser)
{
    if (locked[loser][winner] == true)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked [loser][i] == true && no_loop(winner, i))
        {
            return true;
        }
    }

    return false;

}



// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;
        if (!no_loop(winner, loser))
        {
            locked[winner][loser] = true;
        }
    }
    return;
}


// Print the winner of the election
void print_winner(void)
{
    for (int row = 0; row < candidate_count; row++)
    {
        for (int col = 0; col < candidate_count; col++)
        {
            if (locked[row][col] == true)
            {
                break;
            }
            else if (col == candidate_count - 1)
            {
                printf("%s", candidates[row]);
            }
        }
    }
    return;
}