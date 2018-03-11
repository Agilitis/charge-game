#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mapHandler.h"

void freeCharges(Map* map)
{
    free(map->charges);
}

bool addCharge(Map* map, Charge charge)
{
    Charge *chargesBuffer = NULL;
    map->chargeCount++;

    if (map->charges != NULL)
    {
        chargesBuffer = realloc(map->charges, sizeof(Charge)*(map->chargeCount+1));
        }
    else
    {
        chargesBuffer = malloc(sizeof(Charge));
    }
        if(chargesBuffer!=NULL){
            map->charges = chargesBuffer;
            map->charges[map->chargeCount-1].x = charge.x;
            map->charges[map->chargeCount-1].y = charge.y;
            map->charges[map->chargeCount-1].weight = charge.weight;
            return true;
        }else{
           return false;
        }

}
void deleteCharge(Map* map)
{
    if(map->chargeCount > 0)
    {
        map->chargeCount--;
    }


}
void freeWalls(Map* map)
{
    free(map->walls);
}

Map loadMap(char mapName[])
{
    Map map;
    map.startPointX = 0;
    map.startPointY = 0;
    map.endPointX = 0;
    map.endPointY = 0;
    map.chargeCount = 0;
    map.walls = NULL;
    map.charges = NULL;

    FILE *f = fopen(mapName, "r");
    //File open was not successful.
    if (f == NULL)
    {
        return map;
    }

    fscanf(f, "%d %d %d %d", &map.startPointX, &map.startPointY, &map.endPointX, &map.endPointY);
    fscanf(f, "%d %d", &map.startVelocityX, &map.startVelocityY);
    map.wallCount = 0;
    Wall temp;

    while (fscanf(f, "%d %d %d %d", &temp.x, &temp.y, &temp.w, &temp.h) != -1)
    {
        map.wallCount++;
        if (map.walls != NULL)
        {
            map.walls = (Wall*) realloc(map.walls, sizeof(Wall)*(map.wallCount+1));
        }
        else
        {
            map.walls = (Wall*)malloc(sizeof(Wall));
        }

        map.walls[map.wallCount-1].x = temp.x;
        map.walls[map.wallCount-1].y = temp.y;
        map.walls[map.wallCount-1].w = temp.w;
        map.walls[map.wallCount-1].h = temp.h;
    }

    fclose(f);
    return map;
}
