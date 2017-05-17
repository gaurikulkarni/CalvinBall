#include <cmath>
#include "viewport.h"
#include "smartSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartSprite::SmartSprite(const std::string& name, 
            const Vector2f& pos, const Player& p) :
 Sprite(name, pos, 
   Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
            Gamedata::getInstance().getXmlInt(name+"/speedY"))
 ),  
 calvin(p),
 safeDistance(Gamedata::getInstance().getXmlInt("safeDistance")),
 currentMode(NORMAL)
{ }

void SmartSprite::goLeft()  { 
  if (X() > 0) velocityX( -abs(velocityX()) ); 
}
void SmartSprite::goRight() { velocityX( fabs(velocityX()) ); }
void SmartSprite::goUp()    { velocityY( -fabs(velocityY()) ); }
void SmartSprite::goDown()  { velocityY( fabs(velocityY()) ); }

void SmartSprite::draw() const {
  Sprite::draw();
}

void SmartSprite::update(Uint32 ticks) {
  Sprite::update(ticks);
  float x= X()+getFrame()->getWidth()/2;
  float y= Y()+getFrame()->getHeight()/2;
  float cx= calvin.X()+calvin.getFrame()->getWidth()/2;
  float cy= calvin.Y()+calvin.getFrame()->getHeight()/2;
  float distanceToEnemy = ::distance( x, y, cx, cy );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy > safeDistance) currentMode = ATTACK;
  }
  else if  ( currentMode == ATTACK ) {
    if(distanceToEnemy < safeDistance) currentMode=NORMAL;
    else {
      if ( x < cx ) goRight();
      if ( x > cx ) goLeft();
      if ( y < cy ) goDown();
      if ( y > cy ) goUp();
    }
  }
}

