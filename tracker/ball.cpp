#include <iostream>
#include <cmath>
#include "ball.h"
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"

Ball::Ball(const std::string& name , const Vector2f& pos,  const Vector2f& vel):
  Sprite(name, pos, vel),
  distance(0),
  maxDistance(Gamedata::getInstance().getXmlInt(name + "/distance")),
  tooFar(false),
  strategy(new MidPointCollisionStrategy)
{ }

Ball::Ball(const Ball& b):
  Sprite(b),
  distance(b.distance),
  maxDistance(b.maxDistance),
  tooFar(b.tooFar),
  strategy(b.strategy)
{ }

Ball& Ball::operator=(const Ball & rhs) {
  Sprite::operator=(rhs);
  distance = rhs.distance;
  maxDistance = rhs.maxDistance;
  tooFar = rhs.tooFar;
  strategy = rhs.strategy;
  return *this;
}

void Ball::reset() {
  tooFar = false;
  distance = 0;
}

void Ball::update(Uint32 ticks){ 
  Vector2f pos = getPosition();
  Vector2f incr = getVelocity() * static_cast<float> (ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( Y() + frameHeight < 0 || Y() > worldHeight) { tooFar = true; }
  if (X() < 0 || X() > worldWidth) { tooFar = true; }
  distance += (hypot(X() - pos[0] , Y()-pos[1]));
  if (distance > maxDistance) tooFar = true; 
}
  
