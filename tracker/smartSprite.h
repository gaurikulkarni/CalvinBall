#include <iostream>
#include "sprite.h"
#include "player.h"

class SmartSprite : public Sprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, const Player& p);
  void draw() const;
  void update(Uint32 ticks);
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
  virtual ~SmartSprite() { } 

  void incrSafeDistance() {++safeDistance; }
  void decrSafeDistance() { --safeDistance; }
  float getSafeDistance() { return safeDistance; }

private:	
  enum MODE {NORMAL, ATTACK};	
  const Player & calvin;
  float safeDistance;
  MODE currentMode;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
};
