#include <iostream>
#include "hud.h"

HUD::~HUD( ) { }

HUD::HUD() :
  width(Gamedata::getInstance().getXmlInt("Hud/width")),
  height(Gamedata::getInstance().getXmlInt("Hud/height")),
  x(Gamedata::getInstance().getXmlInt("Hud/StartX")),
  y(Gamedata::getInstance().getXmlInt("Hud/StartY"))
{}
 
void HUD::drawHUD(SDL_Surface* const screen,const int seconds,const int fps) const {
  
  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, x, y+height/2, 
                      x+width,y+height/2, 
                      height, 0xff, 0xff, 0xff, 0xff/2);
  IOManager::getInstance().printMessageAt("Press F1 to show instructions", x+10, y+10);
  IOManager::getInstance().printMessageAt("Controls:", x+10, y+30);
  IOManager::getInstance().printMessageAt("Hold SpaceBar -> Jump",x+10,y+50);
  IOManager::getInstance().printMessageAt("W -> UP     S -> DOWN", x+10, y+70);
  IOManager::getInstance().printMessageAt("A -> LEFT  D -> RIGHT ", x+10, y+90);
  IOManager::getInstance().printMessageAt("B -> Shoot balls", x+10, y+110);
  IOManager::getInstance().printMessageAt("G -> God Mode", x+180, y+110);
  IOManager::getInstance().printMessageAt("Player can also use arrow keys", x+10, y+130);
  IOManager::getInstance().printMessageAt("Reach the end to win the game",x+10,y+150);
  IOManager::getInstance().printMessageAt("You get 3 lives to win the game!",x+10,y+170);
  IOManager::getInstance().printMessageAt("Colliding Moe will kill you!",x+10,y+190);
  IOManager::getInstance().printMessageAt("You can shoot Moe. Shoot to kill!",x+10,y+210);
  IOManager::getInstance().printMessageAt("If I were you, I would run!",x+10,y+230);
  IOManager::getInstance().printMessageValueAt("Seconds: ",seconds, x+10, y+250);
  IOManager::getInstance().printMessageValueAt("FPS: ", fps,x+180,y+250);
  Draw_AALine(screen, x,y, x+width,y, RED);
  
}

