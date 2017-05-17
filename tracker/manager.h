#include <list>
#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "frameFactory.h"
#include "scaledSprite.h"
#include "hud.h"
#include "aaline.h"
#include "player.h"
#include "sound.h"
#include "multisprite.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();
  void checkForCollisions();

private:
  const bool env;
  IOManager& io;
  FrameFactory& factory;
  Clock& clock;
  HUD display;

  SDL_Surface * const screen;
  double numberOfLeaves;
  std::vector<ScaledSprite*> leaves;
  unsigned hudTime;
  World mountains;
  World trees;
  World grass;

  Viewport& viewport;
  Player player;
  SDLSound sound;
  std::vector<Drawable*> sprites;
  MultiSprite* hobbes;
  const bool delay;
  bool showHud;
  bool f1Pressed;
  bool makeVideo;
  bool godMode;
  bool end;
  int frameCount;
  int kill;
  const std::string username;
  const std::string title;
  const std::string gameTitle;
  const int frameMax;

  void playEndMusic();
  void draw() const;
  void update();
  void makeLeaves();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
};
