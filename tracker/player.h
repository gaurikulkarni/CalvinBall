#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "drawable.h"
#include "objPool.h"
#include "collisionStrategy.h"
#include "explodingSprite.h"

class Player : public Drawable {
public:
  virtual ~Player();
  Player(const std::string&);
  Player(const Player&);
  Player& operator=(const Player&);
  
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  bool collidedWith(const Drawable* d) const {
    return strategy->execute(*this, *d);
  }

  void setCollisionStrategy(int index) {
    strategy = strategies[index];
  }
  bool ballCollides(const Drawable* d) {
    return balls.checkForCollisions(d);
  }
  bool won() const{ return win;}
  void resetLives(){ lives = 0;}
  int getLives() const {return lives;}
  unsigned int freeCount() const{ return balls.freeCount();}
  unsigned int ballCount() const{ return balls.ballCount();}
  void left();
  void right();
  void up();
  void down();
  void stop();
  void jumping();
  void shoot();
  void explode();
  void reset();
  bool calvinDies();

protected:
  ObjPool balls;
  ExplodingSprite* explosion;
  const std::vector<Frame *> frames;
  int upperLimit;
  int lowerLimit;
  int worldWidth;
  int worldHeight;
  int startX;
  int startY;
  int lives;
  std::vector<CollisionStrategy*> strategies;
  CollisionStrategy * strategy;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float shootInterval;
  float timeSinceLastFrame;
  float minimumSpeed;
  int frameWidth;
  int frameHeight; 
  bool turn;
  bool stay;
  bool jump;
  bool isExploding;
  Vector2f velocity;
  bool win;
  bool keyPressedX;
  bool keyPressedY;
  void advanceFrame(Uint32 ticks);
};
#endif
