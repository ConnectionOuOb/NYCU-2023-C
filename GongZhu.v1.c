#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxPlay 4
#define maxGain 13
#define maxCard 52

struct Card {
    int isUsed;
    int cardType;
    int cardNumb;
};

struct Player {
    char * name;
    int point;
    struct Card * Cards;
};

char * CardNumbs[maxGain]={"A", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K"};
char * CardTypes[maxPlay]={"S", "H", "D", "C"};
struct Player Ps[maxPlay]={
    {"You", 0, NULL},
    {"Player1", 0, NULL},
    {"Player2", 0, NULL},
    {"Player3", 0, NULL},
};

void InitPlayers() {
    int i, j, rnd, repeat;
    int NewCards[maxCard];
    struct Card temp[maxGain];

    for (i=0; i<maxCard; i++) NewCards[i] = -1;

    for (i=0; i<maxCard; i++) {
        repeat = 1;

        while (repeat) {
            repeat = 0;
            rnd = rand() % 52;

            for (j=0; j<maxCard; j++) {
                if (NewCards[j] == rnd) {
                    repeat = 1;
                    break;
                }
            };

            if (!repeat) NewCards[i] = rnd;
        }
    }

    for (i=0; i<maxPlay; i++) {
        int start=maxGain*i, end=maxGain*(i+1);
        for (j=start; j<end; j++) {
            for (int k=start; k<end-1; k++) {
                if (NewCards[k] > NewCards[k+1]) {
                    int tmp = NewCards[k];
                    NewCards[k] = NewCards[k+1];
                    NewCards[k+1] = tmp;
                }
            }
        }

        for (j=0; j<maxGain;j++) {
            rnd = NewCards[i*maxGain+j];
            struct Card card = {0, rnd/maxGain, rnd%maxGain};
            temp[j] = card;
        }

        Ps[i].Cards = malloc(maxGain * sizeof(struct Card));
        memcpy(Ps[i].Cards, temp, maxGain * sizeof(struct Card));
    }
}

void PrintPlayersCards() {
    int i, a=97;

    printf("Round  : ");
    for (i=1; i<maxGain+1; i++) printf("%-4d", i);
    printf("Score\n");

    for (i=0; i<maxPlay; i++) {
        printf("%-7s: ", Ps[i].name);
        for (int j=0; j<maxGain; j++) {
            char * type = Ps[i].Cards[j].cardType==-1 ? "-" : CardTypes[Ps[i].Cards[j].cardType];
            char * numb = Ps[i].Cards[j].cardNumb==-1 ? "-" : CardNumbs[Ps[i].Cards[j].cardNumb];

            printf("%s%-3s", type, numb);
        }
        printf("(%4d)\n", Ps[i].point);
    }

    printf("\n");
    for (i=0; i<maxGain; i++) {
        printf("%-3c", a+i);
    }

    printf("\n");
    for (i=0; i<maxGain; i++) {
        char * type = Ps[0].Cards[i].cardType==-1 ? "-" : CardTypes[Ps[0].Cards[i].cardType];
        char * numb = Ps[0].Cards[i].cardNumb==-1 ? "-" : CardNumbs[Ps[0].Cards[i].cardNumb];

        printf("%s%-2s", type, numb);
    }
    printf("\n");
}

int GongZhu() {
    int i;
    int roundNow;

    InitPlayers();

    for (roundNow=0; roundNow<maxGain; roundNow++) {
        system("clear");

        PrintPlayersCards();

        break;
    }

    return 0;
}

int main()
{
    srand((unsigned)time(NULL));

    GongZhu();
    
    return 0;
}