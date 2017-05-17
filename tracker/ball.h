#ifndef BALL__H
#define BALL__H
#include <iostream>
#include "sprite.h"
#include "collisionStrategy.h"

class Ball : public Sprite {
public:
  Ball(const std::string& name , const Vector2f& ,  const Vector2f&);
  Ball(const Ball&);
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset();
  Ball& operator=(const Ball&);
  bool collidedWith(const Drawable* d) const {
    return strategy->execute(*this, *d);
  }

private:
  float distance;
  float maxDistance;
  bool tooFar;
  CollisionStrategy * strategy;
};

#endif
