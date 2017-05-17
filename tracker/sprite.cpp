#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

Sprite::Sprite(const string& n):
  Drawable(n,
           Vector2f(rand()%10000, 
                    Gamedata::getInstance().getXmlInt(n+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(n+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(n+"/speedY")) 
           ),
  explosion(NULL),
  upperLimit(Gamedata::getInstance().getXmlInt(n+"/upperLimit")),
  lowerLimit(Gamedata::getInstance().getXmlInt(n+"/lowerLimit")),
  frame(FrameFactory::getInstance().getFrame(n)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  isExploding(false),
  turn(false)
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel), 
  explosion(NULL),
  upperLimit(Gamedata::getInstance().getXmlInt(n+"/upperLimit")),
  lowerLimit(Gamedata::getInstance().getXmlInt(n+"/lowerLimit")),
  frame( FrameFactory::getInstance().getFrame(n) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  isExploding(false),
  turn(false)
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel), 
  explosion(NULL),
  upperLimit(Gamedata::getInstance().getXmlInt("world/width")),
  lowerLimit(Gamedata::getInstance().getXmlInt("world/height")),
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  isExploding(false),
  turn(false)
{ }


Sprite::Sprite(const Sprite& s) :
  Drawable(s),
  explosion(s.explosion),
  upperLimit(s.upperLimit),
  lowerLimit(s.lowerLimit),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  isExploding(s.isExploding),
  turn(false)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  explosion = rhs.explosion;
  upperLimit = rhs.upperLimit;
  lowerLimit = rhs.lowerLimit;
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  isExploding = rhs.isExploding;
  return *this;
}

void Sprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if (explosion) {
    explosion->draw();
    return;
  }
  if (turn == false)
  {
    frame->draw(x, y);
  }
  else 
    frame->drawTurn(x, y);
}

void Sprite::explode() { 
  isExploding = true;
  if ( explosion ) return;
  explosion = new ExplodingSprite(*this); 
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

bool Sprite::moeDies(){
  return isExploding;
}

void Sprite::update(Uint32 ticks) { 
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      isExploding = false;
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < upperLimit) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight-lowerLimit) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
    turn =false;
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
    turn =true;
  }  
}
