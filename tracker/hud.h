//#ifndef HUD__H
#define HUD__H
#include <string>
#include <SDL.h>
#include "ioManager.h"
#include "gamedata.h"
#include "aaline.h"


class HUD {
public:
  ~HUD(); 
  HUD();
  void drawHUD(SDL_Surface* const screen, const int seconds, const int fps) const;

private:
  HUD(const HUD&);
  const int width;
  const int height;
  const int x;
  const int y;
};

//#endif
