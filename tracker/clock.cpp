#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  secPos(Gamedata::getInstance().getXmlInt("clock/slocX"),
         Gamedata::getInstance().getXmlInt("clock/slocY")), 
  fpsPos(Gamedata::getInstance().getXmlInt("clock/flocX"),
         Gamedata::getInstance().getXmlInt("clock/flocY")), 
  delayTime(Gamedata::getInstance().getXmlInt("frameCap")), 

  totalTick(0),
  sdlTicks(0),
  ticks(0),

  started(false), 
  paused(false), 
  sloMo(false),
  delayed(false),
  
  sumOfTicks(SDL_GetTicks()){
    start();
}

void Clock::update() { 
  if(delayed){
    delay();
  }
  else if(paused){
    ticks = 0;
  }
  else if (sloMo){
     ticks = 1;    
  }

  else{
    totalTick = SDL_GetTicks();
    ticks = (totalTick - sumOfTicks);
    sumOfTicks += ticks;
  }
}

unsigned int Clock::getTicksSinceLastFrame() const {
  return ticks;
}

void Clock::toggleSloMo() {
  sloMo = !sloMo;
}

void Clock::unSloMo() {
  totalTick = SDL_GetTicks();
  ticks = sumOfTicks;
  sumOfTicks += ticks;
  sloMo = false;
  update();
}

int Clock::getFps() const { 
  if (ticks>0){
    return (1000/ticks);  
  }
  else return 0;
}

void Clock::start() { 
  started = true; 
  paused = false; 
  sloMo = false;
}

void Clock::pause() { 
  paused = true;
}

void Clock::unpause() {
  paused = false;
  update();
}

void Clock::delay(){
  delayed = true;
  totalTick = SDL_GetTicks();
  ticks = (totalTick - sumOfTicks);
  sumOfTicks += ticks;
  if(ticks < (1000/delayTime)) {
    SDL_Delay(1000/delayTime);
  }
}
