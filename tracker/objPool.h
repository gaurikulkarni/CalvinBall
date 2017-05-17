#ifndef OBJPOOL__H
#define OBJPOOL__H
#include <list>
#include "ball.h"
#include <vector>

class ObjPool{

public:
  ObjPool();
  ObjPool(const std::string&);
  ObjPool(const ObjPool&);
  virtual ~ObjPool();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void shoot(const Vector2f& pos , const Vector2f& objVel);
  
  unsigned int ballCount() const{
    return ballList.size();
  }
  unsigned int freeCount() const{
    return freeList.size();
  }
  bool shooting() const {
    return !ballList.empty();
  }

  ObjPool& operator=(const ObjPool&);
  void reset();

  bool checkForCollisions(const Drawable* d){
    std::list<Ball>::iterator ptr = ballList.begin();
    while (ptr != ballList.end()){
    if(ptr->collidedWith(d)){
      freeList.push_back(*ptr);
      ptr = ballList.erase(ptr);
      return true;
    }
    else ++ptr;
  }
  return false;
}

private:
  std::string name;
  Vector2f myVelocity;
  std::list<Ball> ballList;
  std::list<Ball> freeList;
};

#endif
