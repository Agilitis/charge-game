#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "playerHandler.h"

bool refreshPlayers(PlayerList * firstPlayer)
{
    PlayerList *moving = firstPlayer;
    FILE *f = fopen("highschore.txt", "w");
    if (f == NULL)
    {
        return false;
    }
    while(moving != NULL)
    {
        fprintf(f, "%s\n", moving->player.name);
        fprintf(f, "%d\n", moving->player.score);
        moving = moving->next_player;
    }

    fclose(f);
    return true;
}
PlayerList* loadPlayers()
{
    PlayerList* firstPlayer = NULL;
    char tempName[51];
    int tempScore;
    FILE *f = fopen("highschore.txt", "r");
    if (f == NULL)
    {
        return NULL;
    }
    //Going through the file with the players
    while (fscanf(f, " %50[^\n]%d", tempName, &tempScore) != -1)
    {
        PlayerList *newPlayer, *moving, *behind;
        newPlayer = (PlayerList*) malloc(sizeof(PlayerList));
        strcpy(newPlayer->player.name, tempName);
        newPlayer->player.score = tempScore;
        newPlayer->next_player = NULL;
        //first item of the list
        if(firstPlayer == NULL)
        {
            firstPlayer = newPlayer;
            firstPlayer->next_player = NULL;
        }
        //there are other items in the list
        else
        {
            moving = firstPlayer;
            behind = NULL;

            while(moving != NULL && newPlayer->player.score < moving->player.score)
            {
                behind = moving;
                moving = moving->next_player;
            }

            if(behind == NULL)
            {
                newPlayer->next_player = firstPlayer;
                firstPlayer = newPlayer;
            }

            else
            {
                behind->next_player = newPlayer;
                newPlayer->next_player = moving;
            }
        }

    }
    return firstPlayer;
}

bool savePlayer(Player player)
{
    bool ujJatekos = true;
    FILE *f = fopen("highschore.txt", "a");
    if (f == NULL)
    {
        return false;
    }

    PlayerList *firstPlayer = NULL;
    firstPlayer = loadPlayers();
    PlayerList *moving = firstPlayer;

    while(moving != NULL)
    {
        if(strcmp(moving->player.name, player.name) != 0 && strstr(moving->player.name, player.name) != NULL && moving->player.score < player.score)
        {
            moving->player.score = player.score;
            refreshPlayers(firstPlayer);
            return true;
        }
        else if((strcmp(moving->player.name, player.name) != 0 && strstr(moving->player.name, player.name) != NULL) || (strcmp(moving->player.name, player.name) < 0 && strstr(moving->player.name, player.name) != NULL))
        {
            ujJatekos = false;
        }
        moving = moving->next_player;
    }

    if(ujJatekos)
    {
        fprintf(f, "%s \n", player.name);
        fprintf(f, "%d \n", player.score);
        fclose(f);

    }
    freePlayers(firstPlayer);
    return true;
}

Player initPlayer()
{
    Player player;
    player.score = 100;
    player.polaritiy = 1;
    player.level = 1;
    player.won = false;
    player.started = false;
    scanf(" %50[^\n]s", player.name);
    return player;
}

void playerWins(Player* player)
{
    player->level++;
    player->score += 10*player->level;
    player->started=false;
    savePlayer(*player);
}

void freePlayers(PlayerList *firstPlayer)
{
    PlayerList *moving = firstPlayer;

    while(moving != NULL)
    {
        PlayerList *temp = moving->next_player;
        free(moving);
        moving = temp;
    }
}




