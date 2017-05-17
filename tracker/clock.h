#include <SDL.h>
#include "gamedata.h"
#include "ioManager.h"
#include "vector2f.h"
#include <string>
#include <deque>

class Manager;

class Clock {
public:
  static Clock& getInstance();  // This class is a Singleton
  unsigned int getTicksSinceInit() const { return sumOfTicks; }
  unsigned int getSeconds() const { return sumOfTicks/1000; }

  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  bool isSloMo() const  { return sloMo;  }
  int getFps() const;

  void start();
  void pause();
  void toggleSloMo();
  void delay();
  void unpause();
  void unSloMo();
  void draw() const{} // For debugging

private:
  Vector2f secPos;
  Vector2f fpsPos;
  
  unsigned int delayTime;
  unsigned int totalTick;
  unsigned int sdlTicks;
  unsigned int ticks;
  
  bool started;
  bool paused;
  bool sloMo;
  bool delayed;
  unsigned int sumOfTicks;
 
  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);

friend class Manager;
  unsigned int getTicksSinceLastFrame() const;
  void update();  // Increments time/ticks
};
