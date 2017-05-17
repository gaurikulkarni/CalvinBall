#include "objPool.h"
#include "gamedata.h"
#include "frameFactory.h"

ObjPool::~ObjPool (){

}

ObjPool::ObjPool() :
  name(),
  myVelocity(),
  ballList(),
  freeList(){
}

ObjPool::ObjPool(const std::string& n) :
  name(n), 
  myVelocity(
  Gamedata::getInstance().getXmlInt(n + "/speed/x") ,
  Gamedata::getInstance().getXmlInt(n + "/speed/y") 
  ),
  ballList(),
  freeList()
{}

ObjPool::ObjPool(const ObjPool& b) :
  name(b.name),
  myVelocity(b.myVelocity),
  ballList(b.ballList),
  freeList(b.freeList)
{}

ObjPool& ObjPool::operator=(const ObjPool & rhs) {
  name = rhs.name;
  myVelocity = rhs.myVelocity;
  ballList = rhs.ballList;
  freeList = rhs.freeList;
  return *this;
}

void ObjPool::reset(){
  std::list<Ball>::iterator bitr = ballList.begin();
  while(bitr!=ballList.end()){
     freeList.push_back(*bitr);
     bitr = ballList.erase(bitr);
  }     
}

void ObjPool::shoot(const Vector2f& pos , const Vector2f& objVel) {
  if (freeList.empty()){
    Ball b(name , pos , objVel);
    ballList.push_back(b);
  }
  else{
    Ball b = freeList.front();
    b.reset();
    b.setVelocity(objVel);
    b.setPosition(pos);
    ballList.push_back(b);
    freeList.pop_front();
  }
}

void ObjPool::draw() const {
  std::list<Ball>::const_iterator ptr = ballList.begin();
  while (ptr != ballList.end()){
    ptr->draw();
    ++ptr;
  }
}

void ObjPool::update(Uint32 ticks){
  std::list<Ball>::iterator ptr = ballList.begin();
  while (ptr != ballList.end()){
    ptr->update(ticks);
    if (ptr->goneTooFar()){
      freeList.push_back(*ptr);
      ptr = ballList.erase(ptr);
   }
   else ++ptr;
  }
}

