#ifndef MAP_HANDLER
#define MAP_HANDLER
typedef struct Charge
{
    int x;
    int y;
    int weight;

} Charge;
typedef struct Wall
{
    int x;
    int y;
    int w;
    int h;
} Wall;

typedef struct Map
{

    int startPointX;
    int startPointY;
    int endPointX;
    int endPointY;
    int startVelocityX;
    int startVelocityY;

    int chargeCount;
    int wallCount;
    Wall* walls;
    Charge* charges;

} Map;

typedef struct MainCharge
{
    double x;
    double y;
    double xVelocity;
    double yVelocity;
    double weight;
} MainCharge;

Map loadMap(char mapName[]);
bool addCharge(Map* asd, Charge charge);
void deleteCharge(Map* map);
void freeCharges(Map* map);
void freeWalls(Map* map);
#endif // MAP_HANDLER
