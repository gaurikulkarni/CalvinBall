#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"

Player::~Player() {
  for (unsigned i = 0; i < strategies.size(); ++i) {
    delete strategies[i];
  }
}

void Player::advanceFrame(Uint32 ticks) {
  if (jump==true) currentFrame = 4;
  else if (stay == false){
    timeSinceLastFrame += ticks;
    if (timeSinceLastFrame > frameInterval) {
      currentFrame = ((currentFrame+1) % (numberOfFrames-1));
      timeSinceLastFrame = 0;
    }
  }
  else currentFrame = 0;
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))),
  balls("ball"),
  explosion(NULL),
  frames(FrameFactory::getInstance().getFrames(name)),
  upperLimit(Gamedata::getInstance().getXmlInt(name+"/upperLimit")),
  lowerLimit(Gamedata::getInstance().getXmlInt(name+"/lowerLimit")),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  startX(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")), 
  startY(Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
  lives(0),
  strategies(),
  strategy( NULL ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  shootInterval(Gamedata::getInstance().getXmlFloat("ball/Interval")),
  timeSinceLastFrame(0),
  minimumSpeed(Gamedata::getInstance().getXmlFloat("ball/speedX")),
  frameWidth(getFrame()->getWidth()),
  frameHeight(getFrame()->getHeight()),
  turn(false),
  stay(false),
  jump(false),
  isExploding(false),
  velocity(getVelocity()),
  win(false),
  keyPressedX(false),
  keyPressedY(false)
  {  
    strategies.push_back( new PerPixelCollisionStrategy );
    strategy = strategies[0];
}

Player::Player(const Player& s) :
  Drawable(s),
  balls(s.balls), 
  explosion(s.explosion),
  frames(s.frames),
  upperLimit(s.upperLimit),
  lowerLimit(s.lowerLimit),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  startX(s.startX),
  startY(s.startY),
  lives(s.lives),
  strategies(s.strategies),
  strategy(s.strategy),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ), 
  frameInterval( s.frameInterval ),
  shootInterval(s.shootInterval),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  minimumSpeed(s.minimumSpeed),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  turn(s.turn),
  stay(s.stay),
  jump(s.jump),
  isExploding(s.isExploding),
  velocity(s.velocity),
  win(s.win),
  keyPressedX(s.keyPressedX),
  keyPressedY(s.keyPressedY)  
  { }

Player& Player::operator=(const Player & rhs) {
  Drawable::operator=( rhs );
  balls= rhs.balls;
  explosion = rhs.explosion;
  upperLimit = rhs.upperLimit;
  lowerLimit = rhs.lowerLimit;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  startX = rhs.startX;
  startY = rhs.startY;
  lives = rhs.lives;
  strategies = rhs.strategies;
  strategy = rhs.strategy;
  currentFrame = rhs.currentFrame;
  numberOfFrames = rhs.numberOfFrames;
  frameInterval = rhs.frameInterval;
  shootInterval = rhs.shootInterval;
  timeSinceLastFrame = rhs.timeSinceLastFrame;
  minimumSpeed= rhs.minimumSpeed;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  turn = rhs.turn;
  stay = rhs.stay;
  velocity = rhs.velocity;
  win = rhs.win;
  jump = rhs.jump;
  isExploding = rhs.isExploding;
  keyPressedX = rhs.keyPressedX;
  keyPressedY = rhs.keyPressedY;
  return *this;
}

void Player::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  if (explosion) {
    explosion->draw();
    return;
  }
  if (turn == false){
    frames[currentFrame]->draw(x, y);
  }
  else frames[currentFrame]->drawTurn(x, y);
  balls.draw();
}

void Player::explode() { 
  isExploding = true;
  if(explosion) return;
  Sprite s(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(s);
  ++lives;
}

bool Player::calvinDies(){
  return isExploding;
}

void Player::update(Uint32 ticks) { 
  if ( explosion ) { 
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      isExploding = false;
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  advanceFrame(ticks);
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
    turn=true; win = true;
  }
  if(!keyPressedX) stop();
  if(!keyPressedY) stop();
  keyPressedX = false;
  keyPressedY = false;
  timeSinceLastFrame += ticks; 
  balls.update(ticks);
}

void Player::reset(){
X(startX);
Y(startY);
win =false;
}

void Player::stop(){
  stay = true;
  jump=false;
  if(!keyPressedX) velocityX(0);
  if(!keyPressedY) velocityY(0);
}

void Player::right(){
  stay =false;
  keyPressedX= true;
  jump=false;
  if( X() < worldWidth-frameWidth){
    velocityX(velocity[0]);
    turn=false;
  }
}

void Player::left(){
  stay =false;
  keyPressedX = true;
  jump=false;
  if( X() > 0){
    velocityX(-velocity[0]);
    turn = true;
  }
}

void Player::up(){
  stay = false;
  keyPressedY = true;
  jump=false;
  if( Y() > upperLimit){
    velocityY(-velocity[1]);
  }
}

void Player::down(){
  stay = false;
  keyPressedY = true;
  jump=false;
  if( Y() < worldHeight-frameHeight-lowerLimit){
    velocityY(velocity[1]);
  }
}

void Player::jumping(){
  stay = false;
  keyPressedY = false;
  jump=true;
}

void Player::shoot(){
    Vector2f vel = getVelocity();
    float x;
    float y = Y() + frameHeight/2.3;
    if (turn == false) { 
      x = X() + frameWidth;
      vel[0] += minimumSpeed;
    }
    else {
      x = X();
      y = Y();
      vel[0] -= minimumSpeed;
    }
    balls.shoot(Vector2f(x,y) , vel);
    timeSinceLastFrame = 0;
}



