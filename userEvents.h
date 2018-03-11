typedef struct MainCharge
{
    double x,y,xVelocity,yVelocity, weight;
} MainCharge;

void handleEvents(SDL_Surface *screen, SDL_Event ev, Player player, Map map, MainCharge mainCharge);
