#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxPlay 4
#define maxGain 13
#define maxCard 52

int winner=0;
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

int CheclCardSelecable(char sel) {
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
    numCanPlay=-1;

    for (int i=0; i<maxGain; i++) {
        for (int j=0; j<maxGain; j++) {
            int played=0;

            if (CardGets[0][i] == CardPlays[0][j]) {
                CardCanPlays[i] = 0;
                break;
            } else {
                CardCanPlays[i] = 1;
            }

            if (numCanPlay==-1) {
                CardCanPlay[0] = CardEncodes[i];
            } else {
                int startPos=3*numCanPlay;
                CardCanPlay[startPos+1] = ',';
                CardCanPlay[startPos+2] = ' ';
                CardCanPlay[startPos+3] = CardEncodes[i];
            }

            numCanPlay++;
            break;
        }
    }

    numCanPlay++;
    CardCanPlay[3*numCanPlay+1] = '\0';
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
            else printf("%-4s", GetCardEncode(CardGets[i][j]));
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
    int i, userWin=0;
    int roundNow;
    char isOk, userSelCard;

    InitCards();

    for (roundNow=0; roundNow<maxGain; roundNow++) {
        system("clear");

        PrintPlayersCards();

        if (winner == 0) {
            UpdateCardCanPlay();

            if (numCanPlay == maxGain) printf("Your turn, you can select all :");
            else printf("Your turn, you can select %s :", CardCanPlay);

            int outRange=0;
            while (!CheclCardSelecable(userSelCard)) {
                if (outRange) printf("Out of Range ! Please select from %s: ", CardCanPlay);
                scanf(" %c", &userSelCard);
                outRange=1;
            }

            UpdateCardCanPlay();
        }

        break;
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