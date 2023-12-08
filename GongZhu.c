#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxPlay 4
#define maxGain 13
#define maxCard 52

int winner=0;
int carType=-1;
int numCanPlay=-1;
int Points[maxPlay]={0};
int CardGets[maxPlay][maxGain]={0};
int CardPlays[maxPlay][maxGain];
int CardPlayWins[maxGain]={0};
int CardCanPlays[maxGain]={0};
char CardCanPlay[50];
char * CardTypes="SHDC";
char * CardNumbs="A23456789TJQK";
char * CardEncodes="abcdefghijklm";
char * Names[maxPlay]={"You", "Player1", "Player2", "Player3"};

void DebugPrint() {
    printf("############ DebugPrint ############\n");
    printf("### winner     : %d\n",  winner);
    printf("### numCanPlay : %d\n",  numCanPlay);
    printf("### CardCanPlay : %s\n",  CardCanPlay);
    
    printf("### Points\n");
    for (int i=0; i<maxPlay; i++) printf("# %d -> %d\n", i, Points[i]);

    printf("### CardGets\n");
    for (int i=0; i<maxPlay; i++) {
        printf("#Row %d ->", i);
        for (int j=0; j<maxGain; j++) printf("%4d", CardGets[i][j]);
        printf("\n");
    }

    printf("### CardPlays\n");
    for (int i=0; i<maxPlay; i++) {
        printf("#Row %d ->", i);
        for (int j=0; j<maxGain; j++) printf("%4d", CardPlays[i][j]);
        printf("\n");
    }

    printf("### CardPlayWins\n# ");
    for (int j=0; j<maxGain; j++) printf("%4d", CardPlayWins[j]);
    printf("\n");

    printf("### CardCanPlays\n# ");
    for (int j=0; j<maxGain; j++) printf("%4d", CardCanPlays[j]);

    printf("\n############ DebugPrint ############\n\n\n");
}

int CheckCardSelecable(char sel) {
    int i, isOk = 0;

    for (i=0; i<maxGain; i++) {
        if (sel == CardEncodes[i] && CardCanPlays[i]) isOk = 1;
    }

    return isOk;
}

char * GetCardEncode(int cardId) {
    char * card = (char *)malloc(3 * sizeof(char));

    card[0] = CardTypes[cardId/maxGain];
    card[1] = CardNumbs[cardId%maxGain];
    card[2] = '\0';

    return card;
}

void UpdateCardCanPlay() {
    int startPos=0;
    numCanPlay=maxGain;

    for (int i=0; i<maxGain; i++) {
        for (int j=0; j<maxGain; j++) {
            if (CardGets[0][i] == CardPlays[0][j]) {
                CardCanPlays[i] = 0;
                numCanPlay--;
                break;
            } else {
                CardCanPlays[i] = 1;
            }
        }
    }

    for (int i=0; i<maxGain; i++) {
        if (!CardCanPlays[i]) continue;

        if (startPos==0) {
            CardCanPlay[0] = CardEncodes[i];
            startPos++;
        } else {
            CardCanPlay[startPos] = ',';
            CardCanPlay[startPos+1] = ' ';
            CardCanPlay[startPos+2] = CardEncodes[i];
            startPos+=3;
        }
    }

    CardCanPlay[startPos] = '\0';
}

void InitCards() {
    int i, j, rnd, repeat;
    int RngCards[maxCard];

    for (i=0; i<maxCard; i++) RngCards[i] = -1;

    for (i=0; i<maxCard; i++) {
        repeat = 1;

        while (repeat) {
            repeat = 0;
            rnd = rand() % 52;

            for (j=0; j<maxCard; j++) {
                if (RngCards[j] == rnd) {
                    repeat = 1;
                    break;
                }
            };

            if (!repeat) RngCards[i] = rnd;
        }
    }

    for (i=0; i<maxPlay; i++) {
        int start=maxGain*i, end=maxGain*(i+1);

        for (j=start; j<end; j++) {
            for (int k=start; k<end-1; k++) {
                if (RngCards[k] > RngCards[k+1]) {
                    int tmp = RngCards[k];
                    RngCards[k] = RngCards[k+1];
                    RngCards[k+1] = tmp;
                }
            }
        }

        for (j=0; j<maxGain; j++) {
            CardGets[i][j] = RngCards[start+j];
            CardPlays[i][j] = -1;
        }
    }
}

void PrintPlayersCards() {
    int i, j, isPlay;

    printf("Round  : ");
    for (i=1; i<maxGain+1; i++) printf("%-4d", i);
    printf("Score\n");

    for (i=0; i<maxPlay; i++) {
        j=0;

        printf("%-7s: ", Names[i]);

        for (j=0; j<maxGain; j++) {
            if (CardPlays[i][j] == -1) printf("    ");
            else printf("%-4s", GetCardEncode(CardPlays[i][j]));
        }
        printf("(%4d)\n", Points[i]);
    }

    printf("\n");
    for (i=0; i<maxGain; i++) {
        isPlay=0;
        for (j=0; j<maxGain; j++) if (CardPlays[0][j] == CardGets[0][i]) isPlay=1;

        if (isPlay) printf("%c* ", CardEncodes[i]);
        else printf("%-3c", CardEncodes[i]);
    }

    printf("\n");
    for (i=0; i<maxGain; i++) printf("%-3s", GetCardEncode(CardGets[0][i]));
    printf("\n\n");
}

void GongZhu() {
    // 0. Declare initial vars
    int i, userWin=0;
    int roundNow;
    char isOk, userSelCard;

    // 1. Initialize cards and soring by type & num
    InitCards();

    // 2. Start 13 Round
    for (roundNow=0; roundNow<maxGain; roundNow++) {
        // 2.0. Debug
        printf("\n\n\n############ Round: %d ############\n", roundNow);
        //DebugPrint();

        // 2.1. Clear standard screen output
        system("clear");

        // 2.2. Print players & cards
        PrintPlayersCards();

        // 2.3. Get first card
        if (winner == 0) {
            UpdateCardCanPlay();

            if (numCanPlay == maxGain) printf("Your turn, you can select all :");
            else printf("Your turn, you can select %s :", CardCanPlay);

            int outRange=0;
            while (!CheckCardSelecable(userSelCard)) {
                if (outRange) printf("Out of Range ! Please select from %s: ", CardCanPlay);
                scanf(" %c", &userSelCard);
                outRange=1;
            }

            for (i=0; i<maxGain; i++) {
                if (CardEncodes[i] == userSelCard) {
                    CardPlays[0][roundNow] = CardGets[0][i];
                }
            }
        }

        // 2.4. Follow first card, Get player 2~4 card

        // 2.5. Determine who win and save

        // 2.6. Calculate points
    }

    if (userWin) printf("\nYou win! New Game?(Y/N):");
    else printf("\nYou lose! New Game?(Y/N):");

    scanf(" %c", &isOk);

    if (isOk == 'Y' || isOk == 'y') GongZhu();
}

int main()
{
    srand((unsigned)time(NULL));

    GongZhu();
    
    return 0;
}