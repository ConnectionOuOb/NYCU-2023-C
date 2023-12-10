#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxPlay 4
#define maxGain 13
#define maxCard 52
#define maxScore 16

int winner = 0;
int numCanPlay = -1;
int nowCardType = -1;
int Points[maxPlay] = {0};
int CardPlayWins[maxGain];
int CardGets[maxPlay][maxGain] = {0};
int CardPlays[maxPlay][maxGain];
int CardCanPlays[maxPlay][maxGain] = {0};
char CardCanPlay[50];
char *CardTypes = "SHDC";
char *CardNumbs = "A23456789TJQK";
char *CardEncodes = "abcdefghijklm";
char *Names[maxPlay] = {"You", "Player1", "Player2", "Player3"};

// Scoring method = SQ, HA, H2, H3, H4, H5, H6, H7, H8, H9, HT, HJ, HQ, HK,  DJ, CT
int ScoreIndexs[maxScore] = {11, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 36, 48};
int BasicValues[maxScore] = {-100, -50, -2, -3, -10, -5, -6, -7, -8, -9, -10, -20, -30, -40, 100, 50};
int TransValues[maxScore] = {100, -50, -2, -3, -10, -5, -6, -7, -8, -9, -10, -20, -30, -40, -100, 50};

void DebugPrint()
{
    printf("############ DebugPrint ############\n");
    printf("### winner     : %d\n", winner);
    printf("### numCanPlay : %d\n", numCanPlay);
    printf("### CardCanPlay : %s\n", CardCanPlay);
    printf("### nowCardType : %d\n", nowCardType);

    printf("### Points\n");
    for (int i = 0; i < maxPlay; i++)
        printf("# %d -> %d\t", i, Points[i]);

    printf("\n### CardGets\n");
    for (int i = 0; i < maxPlay; i++)
    {
        printf("#Row %d ->", i);
        for (int j = 0; j < maxGain; j++)
            printf("%4d", CardGets[i][j]);
        printf("\n");
    }

    printf("### CardCanPlays\n");
    for (int i = 0; i < maxPlay; i++)
    {
        printf("#Row %d ->", i);
        for (int j = 0; j < maxGain; j++)
            printf("%4d", CardCanPlays[i][j]);
        printf("\n");
    }

    printf("### CardPlays\n");
    for (int i = 0; i < maxPlay; i++)
    {
        printf("#Row %d ->", i);
        for (int j = 0; j < maxGain; j++)
            printf("%4d", CardPlays[i][j]);
        printf("\n");
    }

    printf("### CardPlayWins\n# ");
    for (int j = 0; j < maxGain; j++)
        printf("%4d", CardPlayWins[j]);
    printf("\n");

    printf("############ DebugPrint ############\n\n\n");
}

int CheckCardSelecable(char sel)
{
    int i, isOk = 0;

    for (i = 0; i < maxGain; i++)
    {
        if (sel == CardEncodes[i] && CardCanPlays[0][i])
        {
            if (nowCardType == -1)
            {
                isOk = 1;
            }
            else
            {
                int start = nowCardType * maxGain;
                if (CardGets[0][i] >= start && CardGets[0][i] < start + maxGain)
                    isOk = 1;
            }
        }
    }

    return isOk;
}

int PlayCard(int who, int isFirst)
{
    int i, nCanPlay = 0, selFromAll = 0;
    int cardStart = nowCardType * maxGain;

    for (i = 0; i < maxGain; i++)
    {
        if (CardCanPlays[who][i])
        {
            if (CardGets[who][i] >= cardStart && CardGets[who][i] < cardStart + maxGain)
                nCanPlay++;
        }
    }

    if (nCanPlay == 0 || isFirst)
    {
        selFromAll = 1;
        nCanPlay = 0;
        for (i = 0; i < maxGain; i++)
            if (CardCanPlays[who][i])
                nCanPlay++;
    };

    int rng = rand() % nCanPlay;
    int sel, pin = 0;

    for (i = 0; i < maxGain; i++)
    {
        if (CardCanPlays[who][i])
        {
            if (selFromAll)
            {
                if (pin == rng)
                {
                    sel = CardGets[who][i];
                    break;
                }
                else
                {
                    pin++;
                }
            }
            else
            {
                if (CardGets[who][i] >= cardStart && CardGets[who][i] < cardStart + maxGain)
                {
                    if (pin == rng)
                    {
                        sel = CardGets[who][i];
                        break;
                    }
                    else
                    {
                        pin++;
                    }
                }
            }
        }
    }

    return sel;
}

char *GetCardEncode(int cardId)
{
    char *card = (char *)malloc(3 * sizeof(char));

    card[0] = CardTypes[cardId / maxGain];
    card[1] = CardNumbs[cardId % maxGain];
    card[2] = '\0';

    return card;
}

void UpdateCardCanPlay()
{
    int startPos = 0;
    numCanPlay = maxGain;

    for (int player = 0; player < maxPlay; player++)
    {
        for (int i = 0; i < maxGain; i++)
        {
            for (int j = 0; j < maxGain; j++)
            {
                if (CardGets[player][i] == CardPlays[player][j])
                {
                    CardCanPlays[player][i] = 0;
                    if (player == 0)
                        numCanPlay--;
                    break;
                }
                else
                {
                    CardCanPlays[player][i] = 1;
                }
            }
        }
    }

    for (int i = 0; i < maxGain; i++)
    {
        if (!CardCanPlays[0][i])
            continue;
        if (nowCardType != -1)
        {
            int start = nowCardType * maxGain;
            if (CardGets[0][i] < start || CardGets[0][i] >= start + maxGain)
                continue;
        }

        if (startPos == 0)
        {
            CardCanPlay[0] = CardEncodes[i];
            startPos++;
        }
        else
        {
            CardCanPlay[startPos] = ',';
            CardCanPlay[startPos + 1] = ' ';
            CardCanPlay[startPos + 2] = CardEncodes[i];
            startPos += 3;
        }
    }

    CardCanPlay[startPos] = '\0';
}

void UpdateWinner(int r)
{
    int p;

    winner = 0;

    for (p = 0; p < maxPlay; p++)
    {
        int num = CardPlays[p][r] % maxGain == 0 ? CardPlays[p][r] + maxGain : CardPlays[p][r];
        int win = CardPlays[winner][r] % maxGain == 0 ? CardPlays[winner][r] + maxGain : CardPlays[winner][r];
        if (win < num)
            winner = p;
        printf("%d\t%d\n", num, winner);
    }

    CardPlayWins[r] = winner;
}

void InitCards()
{
    int i, j, rnd, repeat;
    int RngCards[maxCard];

    for (i = 0; i < maxCard; i++)
        RngCards[i] = -1;

    for (i = 0; i < maxGain; i++)
        CardPlayWins[i] = -1;

    for (i = 0; i < maxCard; i++)
    {
        repeat = 1;

        while (repeat)
        {
            repeat = 0;
            rnd = rand() % 52;

            for (j = 0; j < maxCard; j++)
            {
                if (RngCards[j] == rnd)
                {
                    repeat = 1;
                    break;
                }
            };

            if (!repeat)
                RngCards[i] = rnd;
        }
    }

    for (i = 0; i < maxPlay; i++)
    {
        int start = maxGain * i, end = maxGain * (i + 1);

        for (j = start; j < end; j++)
        {
            for (int k = start; k < end - 1; k++)
            {
                if (RngCards[k] > RngCards[k + 1])
                {
                    int tmp = RngCards[k];
                    RngCards[k] = RngCards[k + 1];
                    RngCards[k + 1] = tmp;
                }
            }
        }

        for (j = 0; j < maxGain; j++)
        {
            CardGets[i][j] = RngCards[start + j];
            CardPlays[i][j] = -1;
        }
    }
}

void PrintPlayersCards(int round)
{
    int i, j, isPlay;

    printf("Round  : ");
    for (i = 1; i < maxGain + 1; i++)
        printf("%-4d", i);
    printf("Score\n");

    for (i = 0; i < maxPlay; i++)
    {
        j = 0;

        printf("%-7s: ", Names[i]);

        for (j = 0; j < maxGain; j++)
        {
            if (CardPlays[i][j] == -1)
            {
                printf("    ");
            }
            else
            {
                // printf("!!!! %d\t%d\t%d\n", i, j, CardPlayWins[j]);
                if (CardPlayWins[j] == i)
                    printf("%s# ", GetCardEncode(CardPlays[i][j]));
                else
                    printf("%-4s", GetCardEncode(CardPlays[i][j]));
            }
        }
        printf("(%4d)\n", Points[i]);
    }

    printf("\n");
    for (i = 0; i < maxGain; i++)
    {
        isPlay = 0;
        for (j = 0; j < maxGain; j++)
            if (CardPlays[0][j] == CardGets[0][i])
                isPlay = 1;

        if (isPlay)
            printf("%c* ", CardEncodes[i]);
        else
            printf("%-3c", CardEncodes[i]);
    }

    printf("\n");
    for (i = 0; i < maxGain; i++)
        printf("%-3s", GetCardEncode(CardGets[0][i]));
    printf("\n\n");
}

/*
char * CardTypes="SHDC";
char * CardNumbs="A23456789TJQK";
// Scoring method =    SQ, HA, H2, H3, H4, H5, H6, H7, H8, H9, HT, HJ, HQ, HK,  DJ, CT
int ScoreIndexs[16] = {  11, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  36, 48};
int BasicValues[16] = {-100,-50, -2, -3,-10, -5, -6, -7, -8, -9,-10,-20,-30,-40, 100, 50};
int TransValues[16] = { 100,-50, -2, -3,-10, -5, -6, -7, -8, -9,-10,-20,-30,-40,-100, 50};
*/
void CalScore()
{
    int i, j, k, trans = 0, whoGetCT = -1, goCT = -1;
    int heartStart = maxGain, heartEnd = maxGain * 2;
    int ScoreTimes[maxPlay] = {0};
    int HeartTimes[maxPlay] = {0};

    // 1. Make statistic
    for (i = 0; i < maxGain; i++)
    {
        // 1.1. Skip non-played
        if (CardPlayWins[i] == -1)
            continue;

        // 1.2. Calculate each round winner's card
        for (j = 0; j < maxPlay; j++)
        {
            // 1.2.1 Detect who get CT
            if (CardPlays[j][i] == ScoreIndexs[15])
                whoGetCT = CardPlayWins[i];

            // 1.2.2 Calculate number of heart cards
            if (CardPlays[j][i] >= heartStart && CardPlays[j][i] < heartEnd)
                HeartTimes[CardPlayWins[i]]++;

            // 1.2.3 Calculate number of score cards
            for (k = 0; k < maxScore; k++)
            {
                if (ScoreIndexs[k] == CardPlays[j][i])
                    ScoreTimes[CardPlayWins[i]]++;
            }
        }
    }

    // 2. Processing CT type
    if (whoGetCT != -1)
        goCT = ScoreTimes[whoGetCT] > 1;

    // 3. Processing All heart
    for (i = 0; i < maxPlay; i++)
        trans = HeartTimes[i] == 13;

    // 4. Precessing who get all score cards
    for (i = 0; i < maxPlay; i++)
        if (ScoreTimes[i] == maxScore)
            Points[i] += 1000;

    // 5. Final score calculation
    for (i = 0; i < maxGain; i++)
    {
        // 5.1. Skip non-played
        if (CardPlayWins[i] == -1)
            continue;

        // 5.2. Calculate each round winner's card
        for (j = 0; j < maxPlay; j++)
        {
            // 5.2.1 Iterate and add score
            for (k = 0; k < maxScore; k++)
            {
                if (ScoreIndexs[k] == CardPlays[j][i])
                {
                    if (k == 15 && goCT)
                        continue;
                    else
                        Points[CardPlayWins[i]] += trans ? TransValues[k] : BasicValues[k];

                    break;
                }
            }
        }
    }
}

void GongZhu()
{
    // 0. Declare initial vars
    int i, userWin = 0;
    int roundNow;
    char isOk, userSelCard;

    // 1. Initialize cards and soring by type & num
    InitCards();

    // 2. Start 13 Round
    for (roundNow = 0; roundNow < maxGain; roundNow++)
    {
        // 2.1. Clear standard screen output (Linux)
        system("clear");

        printf("############ Round: %d ############\n", roundNow);
        DebugPrint();

        if (winner != 0)
        {
            int firstCard = PlayCard(winner, 1);
            CardPlays[winner][roundNow] = firstCard;
            nowCardType = firstCard / maxGain;
            for (i = 1; i < maxPlay; i++)
            {
                if (i != winner)
                    CardPlays[i][roundNow] = PlayCard(i, 0);
            }
        }

        UpdateCardCanPlay();

        // 2.2. Print players & cards
        PrintPlayersCards(roundNow);

        // 2.3. Play cards
        if (winner == 0)
        {
            if (numCanPlay == maxGain)
                printf("Your turn, you can select all :");
            else
                printf("Your turn, you can select %s :", CardCanPlay);

            int outRange = 0;
            while (!CheckCardSelecable(userSelCard))
            {
                if (outRange)
                    printf("Out of Range ! Please select from %s: ", CardCanPlay);
                scanf(" %c", &userSelCard);
                outRange = 1;
            }

            for (i = 0; i < maxGain; i++)
            {
                if (CardEncodes[i] == userSelCard)
                    CardPlays[0][roundNow] = CardGets[0][i];
            }

            nowCardType = CardPlays[0][roundNow] / maxGain;
            for (i = 1; i < maxPlay; i++)
                CardPlays[i][roundNow] = PlayCard(i, 0);
        }
        else
        {
            printf("Your turn, you can select %s :", CardCanPlay);

            int outRange = 0;
            while (!CheckCardSelecable(userSelCard))
            {
                if (outRange)
                    printf("Out of Range ! Please select from %s: ", CardCanPlay);
                scanf(" %c", &userSelCard);
                outRange = 1;
            }

            for (i = 0; i < maxGain; i++)
            {
                if (CardEncodes[i] == userSelCard)
                    CardPlays[0][roundNow] = CardGets[0][i];
            }
        }

        nowCardType = -1;

        // 2.5. Determine who win and save
        UpdateWinner(roundNow);

        // 2.6. Calculate points
        CalScore();
    }

    PrintPlayersCards(maxGain + 1);

    if (userWin)
        printf("\nYou win! New Game?(Y/N):");
    else
        printf("\nYou lose! New Game?(Y/N):");

    scanf(" %c", &isOk);

    if (isOk == 'Y' || isOk == 'y')
        GongZhu();
}

int main()
{
    srand((unsigned)time(NULL));

    GongZhu();

    return 0;
}