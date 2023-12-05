#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define maxType 4
#define maxPlay 4
#define maxGain 13
#define maxCard 52

struct Card {
    int isUsed;
    int cardType;
    int cardNum;
};

struct Player {
    char * name;
    int point;
    struct Card * Cards;
};

char CardNumbs[maxGain]=["A", "1", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K"];
char CardTypes[maxType]=["S", "H", "D", "C"];
struct Player Ps[maxPlay];

void InitPlayers() {
    struct Card IniCards[maxGain];

    for (var i=0; i<maxGain; i++) IniCards[i] = {0, -1, -1};

    Ps=[
        {"You", 0, IniCards},
        {"Player1", 0, IniCards},
        {"Player2", 0, IniCards},
        {"Player3", 0, IniCards},
    ];
}

void PrintPlayersCards() {
    for (var i=0; i<maxPlay; i++) {
        printf("%-7s: ", Ps[i].name);
        for (int j=0; j<maxGain; j++) printf("%-5d", Ps[i].Cards[j]);
        printf("(%4d)\n", Ps[i].point);
    }
}

int GongZhu() {
    int i;
    int roundNow;

    InitPlayers();

    for (roundNow=0; roundNow<maxGain; roundNow++) {
        system("clear");

        printf("Round  : ");
        for (i=1; i<maxGain+1; i++) printf("%-5d", i);
        printf("Score\n");

        PrintPlayersCards()

        printf("\n");
//        for (i=1; i<maxGain+1; i++) printf("     ");



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