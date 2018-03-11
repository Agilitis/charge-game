#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "playerHandler.h"
#include "mapHandler.h"
#include <math.h>
#include <SDL_image.h>
#include "helperMethods.h"
#include "constants.h"
#include "graphics.h"

Uint32 idozit(Uint32 ms)

{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* ujabb varakozas */
}

void drawWall(SDL_Surface* screen, Wall wall)
{
    rectangleRGBA(screen, wall.x, wall.y, wall.x+wall.w, wall.y+wall.h, 255, 255, 255, 255);
}

void drawEndPoint(SDL_Surface* screen, Map map)
{
    rectangleRGBA(screen, map.endPointX-35, map.endPointY-35, map.endPointX+35, map.endPointY+35, 255, 0, 255, 255);
}

bool checkCollision(MainCharge* mainCharge, Wall wall)
{
    double r = MAIN_CHARGE_RADIUS+3;
    double rectWidth = (double)wall.w;
    double rectHeight = (double)wall.h;
    double circleDistanceX = fabs((double)(mainCharge->x - (wall.x+wall.w/2)));
    double circleDistanceY = fabs((double)(mainCharge->y - (wall.y+wall.h/2)));
    double cornerDistance_sq = fabs((circleDistanceX - rectWidth/2)*(circleDistanceX - rectWidth/2) +
                                    (circleDistanceY - rectHeight/2)*(circleDistanceY - rectHeight/2));


    if (circleDistanceX > (rectWidth/2 + r))
    {
        return false;
    }
    if (circleDistanceY >= (rectHeight/2 + r))
    {
        return false;
    }

    if (circleDistanceX <= (rectWidth/2))
    {
        return true;
    }

    if (circleDistanceY <= (rectHeight/2))
    {
        return true;
    }
    return (cornerDistance_sq >= (r*r));
}

void drawCharge(SDL_Surface *screen, int polarity, int x, int y)
{
    if(polarity > 0)
    {
        filledCircleRGBA(screen,x, y, 5, 0,0,255,240);
        stringRGBA(screen, x-3,y-3, "-", 255, 255, 255, 255);
    }
    else
    {
        filledCircleRGBA(screen, x, y, 5, 255,0,0,240);
        stringRGBA(screen, x-3, y-3, "+", 255, 255, 255, 255);
    }

}
void initializeMap(SDL_Surface* screen, Player player, Map map, MainCharge* mainCharge)
{
    SDL_FillRect(screen,NULL, 0x000000);

    /* szoveg */
    char caption[100] = "Welcome to the arena, ";
    strcat(caption, player.name);
    strcat(caption, "   Score: ");
    char buffer[20];
    sprintf(buffer, "%d map: %d", player.score, player.level);
    strcat(caption, buffer);
    stringRGBA(screen, 10, 10, caption, 255, 255, 255, 255);

    mainCharge->x = map.startPointX;
    mainCharge->y = map.startPointY;
    mainCharge->xVelocity = map.startVelocityX;
    mainCharge->yVelocity = map.startVelocityY;
    mainCharge->weight = 100;
    player.started = false;

    //Setting the map
    if(map.wallCount != 0)
    {
        int i;
        for(i=0; i < map.wallCount; i++)
        {
            drawWall(screen, map.walls[i]);
        }
    }
    drawEndPoint(screen, map);
}

void tick(SDL_Surface* screen, Player* player, Map* map, MainCharge* mainCharge)
{
    if(!player->started)
        return;
    // Charge movement
    mainCharge->x += mainCharge->xVelocity;
    mainCharge->y += mainCharge->yVelocity;

    // Player Annihilation
    if(mainCharge->x < -1*MAIN_CHARGE_RADIUS || mainCharge->y < -1*MAIN_CHARGE_RADIUS || mainCharge->x >WINDOW_WIDTH+MAIN_CHARGE_RADIUS || mainCharge->y > WINDOW_HEIGHT + MAIN_CHARGE_RADIUS)
    {
        player->score -= PENALTY;
        player->started = false;
        map->chargeCount = 0;
        free(map->charges);
        map->charges = NULL;
        initializeMap(screen, *player, *map, mainCharge);
        savePlayer(*player);
    }

    // Player Collistion to wall
    if(map->wallCount != 0)
    {
        int i;
        for(i=0; i< map->wallCount; i++)
        {
            if(checkCollision(mainCharge, map->walls[i]))
            {
                if(mainCharge->x < map->walls[i].x || mainCharge->x > map->walls[i].x+map->walls[i].w)
                {
                    mainCharge->xVelocity*=-1;
                }
                else
                {
                    mainCharge->yVelocity*=-1;
                }
            }
        }
    }

    //Charge movement calculations
    if(map->chargeCount != 0)
    {
        int i;
        for(i=0; i < map->chargeCount; i++)
        {
            double distance = sqrt(pow(map->charges[i].x-mainCharge->x,2)+pow(map->charges[i].y-mainCharge->y,2));
            if (distance > 0)
            {
                double xVelocityFactor = ((map->charges[i].x-mainCharge->x)/distance)*(mainCharge->weight*map->charges[i].weight)/(distance*distance);
                double yVelocityFactor = ((mainCharge->y-map->charges[i].y)/distance)*(mainCharge->weight*map->charges[i].weight)/(distance*distance);
                mainCharge->xVelocity+=xVelocityFactor;
                mainCharge->yVelocity-=yVelocityFactor;
                //Drawing charges again in case the MainCharge goes over them
                drawCharge(screen, map->charges[i].weight, map->charges[i].x, map->charges[i].y);
            }
        }
    }

    //Player wins
    if(mainCharge->x > map->endPointX-35 && mainCharge->y > map->endPointY-35 && mainCharge->x < map->endPointX+35 && mainCharge->y < map->endPointY+35)
    {
        if(player->level < 3)
        {
            playerWins(player);
            char mapNameBuffer[10];
            sprintf(mapNameBuffer, "map%d.txt", player->level);
            free(map->charges);
            map->charges = NULL;
            *map = loadMap(mapNameBuffer);
            initializeMap(screen, *player, *map, mainCharge);
        }
        else
        {
            player->won = true;
            SDL_Surface* winLogo = IMG_Load("victory.jpg");
            SDL_Rect celpozicio    = { 275, 125,100, 100 };
            SDL_BlitSurface(winLogo, NULL, screen, &celpozicio);
            stringRGBA(screen, 350, 100, "Press Q to exit!", 255, 255, 255, 255);
            player->started = false;
        }
    }
}

bool chargePlaceValidation(Map map, int x, int y)
{
    int i;
    //adjusments for radius of charges
    if (map.wallCount > 0)
    {
        for(i=0; i < map.wallCount; i++)
        {
            if((x > map.walls[i].x-10 && y > map.walls[i].y-10) && (x<map.walls[i].x+map.walls[i].w+10 && y <map.walls[i].y+map.walls[i].h+10))
            {
                return false;
            }
        }
    }
    return (x < map.endPointX-45 || y < map.endPointY-45) || (x > map.endPointX+45 || y > map.endPointY+45);
}

void initGraphicsWindow(Player player, Map map)
{
    SDL_Event ev;
    SDL_Surface *screen;
    SDL_TimerID id;
    MainCharge mainCharge;
    /* SDL inicializálása és ablak megnyitása */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_ANYFORMAT);

    if (!screen)
    {
        fprintf(stderr, "Nem sikerult megnyitni az ablakot!\n");
        exit(1);
    }

    SDL_WM_SetCaption("Charge game", "Charge game");
    initializeMap(screen, player, map, &mainCharge);

    /* eddig elvegzett rajzolasok a kepernyore */
    SDL_Flip(screen);
    id = SDL_AddTimer(1, idozit, NULL);

    /* varunk a kilepesre */
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT)
    {
        switch(ev.type)
        {

        case SDL_USEREVENT:
            SDL_Flip(screen);
            filledCircleRGBA(screen, mainCharge.x, mainCharge.y, MAIN_CHARGE_RADIUS, 0, 0, 0, 240);
            tick(screen, &player, &map, &mainCharge);
            filledCircleRGBA(screen, mainCharge.x, mainCharge.y, MAIN_CHARGE_RADIUS, 255, 0, 0, 240);
            break;

        case SDL_KEYDOWN:
            if(ev.key.keysym.sym == SDLK_SPACE)
            {
                player.started = true;
            }

            else if(ev.key.keysym.sym == SDLK_1)
            {
                player.polaritiy = 1;
            }

            else if(ev.key.keysym.sym == SDLK_2)
            {
                player.polaritiy = -1;
            }

            else if(ev.key.keysym.sym == SDLK_r)
            {
                player.started=false;
                player.score -= PENALTY;
                savePlayer(player);
                initializeMap(screen, player, map, &mainCharge);
                free(map.charges);
                map.chargeCount = 0;
                map.charges = NULL;
            }

            else if(ev.key.keysym.sym == SDLK_o)
            {
                //Player wins

                playerWins(&player);
                char mapNameBuffer[10];
                sprintf(mapNameBuffer, "map%d.txt", player.level);
                map = loadMap(mapNameBuffer);
                initializeMap(screen, player, map, &mainCharge);
            }

            else if(ev.key.keysym.sym == SDLK_q)
            {
                //Player exit on win
                if(player.won)
                    freeCharges(&map);
                freeWalls(&map);
                SDL_Quit();
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if(ev.button.button == SDL_BUTTON_LEFT)
            {
                if(chargePlaceValidation(map, ev.button.x, ev.button.y))
                {
                    Charge charge = {ev.button.x, ev.button.y, 2*player.polaritiy};
                    addCharge(&map, charge);
                    drawCharge(screen, player.polaritiy, ev.button.x, ev.button.y);
                }
            }

            else if(ev.button.button == SDL_BUTTON_RIGHT)
            {
                //delete a charge
                if(map.chargeCount > 0)
                {
                    filledCircleRGBA(screen, map.charges[map.chargeCount-1].x,map.charges[map.chargeCount-1].y, 5, 0,0,0,255);
                }
                if(map.chargeCount == 0)
                {
                    free(map.charges);
                    map.charges = NULL;
                }
                deleteCharge(&map);
            }
            break;
          }

    }

   /* ablak bezarasa */
    SDL_Quit();
}
