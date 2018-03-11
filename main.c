#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graphics.h"
#include "constants.h"
#include "playerHandler.h"
#include "helperMethods.h"
#include "mapHandler.h"
#include "debugmalloc.h"
bool testMode = false;

int main(void)
{

    int menuPicker = -1;
    int i;
    Player player;
    Map map;
  //  PlayerList *players;
   // players = loadPlayers();
    for(i=0; i<8; i++)
    {
        printf("%s\n", LOGO[i]);
    }

    printConsoleMenu(testMode);
    map = loadMap("map1.txt");
    scanf("%d", &menuPicker);
    while(menuPicker != EOF)
    {
        switch(menuPicker)
        {
        case 1:
            printf("What's your name adventurer?\n");
            player = initPlayer();
            //making sure it is initalized
            player.started = false;
            //Saving player, reloading list, ordering the players in the file
            savePlayer(player);
            PlayerList* players = NULL;
            players = loadPlayers();
            refreshPlayers(players);
            freePlayers(players);
            initGraphicsWindow(player, map);
            break;
        case 2:
            //initHighschoreGraphicsWindow
            printf("The wall of fame: \n");
            PlayerList *moving = NULL;
            players = loadPlayers();
            moving = players;
            while(moving != NULL)
            {
                printf("%s: ", moving->player.name);
                printf("%d \n", moving->player.score);
                moving = moving->next_player;
            }
            printf("\n\n\n");
            freePlayers(players);
            break;
        case 3:
            if(testMode)
            {
            savePlayer(player);
            freePlayers(players);
            players = loadPlayers();
            refreshPlayers(players);
            }
            break;
        case 0:
            freeCharges(&map);
            freeWalls(&map);
            exit(0);
            return 0;
            break;
        default:
            break;
        }
        printConsoleMenu(testMode);
        scanf("%d", &menuPicker);
    }
    return 0;
    //initGraphicsWindow();

}
