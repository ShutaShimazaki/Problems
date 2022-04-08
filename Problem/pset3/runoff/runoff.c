#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];  //candidate型の９マスの配列=candidates 

// Numbers of voters and candidates
int voter_count; //投票者数
int candidate_count; //候補者数

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES) //候補者数がMAXを上回ってたらエラー表示
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    
    //candidateの各要素 name, vote, elimineteの初期値入力
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: "); //投票者数を入力させる
    if (voter_count > MAX_VOTERS) //投票者数がMAXを上回ってたらエラー表示
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++) //各投票者に順に尋ねる
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++) //ひとりの投票者に候補者順位を付けさせる
        {
            string name = get_string("Rank %i: ", j + 1); //Rank1=?(?に入った文字がname), Rank2=? ...と尋ねていく

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    
    for (int j = 0; j < candidate_count; j++)
    {
        if (strcmp(name, candidates[j].name) == 0)  //candidates[1]:Aliceを１位に投票したらj=1(インデックス)
        {
            preferences[voter][rank] = j ;  //インデックス番号を格納
            return true;
        }
    } 
        
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++) 
        {
            int k = preferences[i][j];
            
            //=候補がeliminateされてなかったらcandidates.votesを+1する。これを候補者それぞれで行う
            if (candidates[k].eliminated == 0) 
            { 
                candidates[k].votes++ ;
                break; //l.154のループへ戻る（l.156のループは中断）
            }
        }
    }   
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes >= (voter_count / 2) + 1)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int min_votes = candidates[0].votes ;
    
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes < min_votes && candidates[i].eliminated == 0)
        {
            min_votes = candidates[i].votes;
        }
        
    }
    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min) //minはmin_votesと同じもの
{
    // 以下のコードだと、除去されていない&minを持つ候補者を見つけた瞬間trueを返してしまう。
    //そうではなく、除去されていない全ての候補者が揃ってminを持つときにtrueにしたい。
    /*
    for (int i = 0; i < candidate_count; i++ ) 
    {
        if(candidates[i].eliminated == 0 && candidates[i].votes == min) 
        {
           return 0;
        }
    }
    */
    
    int num_minandremain = 0;
    int remain = 0;
    
    for (int i = 0; i < candidate_count; i++) //残っている候補者の数＝remain
    {
        if (candidates[i].eliminated == 0)
        {
            remain ++;
        }
    }
    
    for (int i = 0; i < candidate_count; i++) //残っている候補者の中で最小値を持つ人数＝num_minandremain
    {
        if (candidates[i].eliminated == 0 && candidates[i].votes == min) 
        {
            num_minandremain ++;
        }
    }
    
    if (remain == num_minandremain)
    {
        return true;
    }
    

    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
        
    }
    
    return;
}
