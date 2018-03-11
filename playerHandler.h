#include <stdbool.h>
#ifndef PLAYER_HANDLER
#define PLAYER_HANDLER


typedef struct Player
{
    char name[51];
    int score;
    int level;
    int polaritiy;
    bool started;
    bool won;
} Player;

typedef struct PlayerList
{
    Player player;
    struct PlayerList* next_player;
}PlayerList;

bool refreshPlayers(PlayerList* firstPlayer);
Player initPlayer();
bool savePlayer(Player player);
void playerWins(Player* player);
PlayerList* loadPlayers();
void freePlayers(PlayerList *firstPlayer);


#endif // PLAYER_HANDLER
