#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include "vector2f.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "extractSurface.h"
#include "scaledSprite.h"
#include "smartSprite.h"
#include <algorithm>

class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }
};

Manager::~Manager() { 
  for (unsigned i = 0; i < sprites.size(); ++i) { 
    delete sprites[i];
    }
  sprites.clear();
  for (unsigned i = 0; i < leaves.size(); ++i) {
    delete leaves[i];
  }
  leaves.clear();
  delete hobbes;
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  factory(FrameFactory::getInstance()),
  clock( Clock::getInstance() ),
  display(),
  screen( io.getScreen() ),
  numberOfLeaves(Gamedata::getInstance().getXmlInt("leaves/numberOfLeaves")),
  leaves(),
  hudTime(Gamedata::getInstance().getXmlInt("Hud/time") ),
  mountains("mountains", Gamedata::getInstance().getXmlInt("mountains/factor") ),
  trees("trees", Gamedata::getInstance().getXmlInt("trees/factor") ),
  grass("grass", Gamedata::getInstance().getXmlInt("grass/factor") ),
  viewport( Viewport::getInstance() ),
  player("calvin"),
  sound(),
  sprites(),
  hobbes(new MultiSprite("hobbes")),
  delay( Gamedata::getInstance().getXmlBool("framesAreCapped")),
  showHud(false),
  f1Pressed(false),
  makeVideo( false ),
  godMode(false),
  end(false),
  frameCount( 0 ),
  kill(0),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  gameTitle(Gamedata::getInstance().getXmlStr("gameTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") )
  {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      throw string("Unable to initialize SDL: ");
    }
    SDL_WM_SetCaption(title.c_str(), NULL);
    atexit(SDL_Quit);
    leaves.reserve(numberOfLeaves);
    makeLeaves();
    sprites.push_back(new Sprite("moe"));
    sprites.push_back(new Sprite("moe"));
    sprites.push_back(new Sprite("moe"));
    sprites.push_back(new Sprite("moe"));
    sprites.push_back(new Sprite("moe"));
    sprites.push_back(new Sprite("moe"));
    sprites.push_back(new Sprite("moe"));
    //sprites.push_back(new MultiSprite("bird"));
    viewport.setObjectToTrack(&player);
    for (unsigned i = 1; i < sprites.size(); ++i) {
    Sprite* sprite = static_cast<Sprite*>( sprites[i] );
    sprites[i] = new SmartSprite(sprite->getName(), 
        sprite->getPosition(), player); 
      delete sprite;
    }    
}

void Manager::makeLeaves() {
  for (unsigned i = 0; i < numberOfLeaves ; ++i) {
    leaves.push_back( new ScaledSprite("leaves") );
  }
  sort(leaves.begin(), leaves.end(), ScaledSpriteCompare());
}

void Manager::checkForCollisions(){
std::vector<Drawable*>::const_iterator sprite = sprites.begin();
  while ( sprite != sprites.end() ){
    if(player.ballCollides(*sprite) && (static_cast<Sprite*>(*sprite)->moeDies() == false)){
      (*sprite)->explode();
      ++kill;
      sound[1];
    }
    if(player.collidedWith(*sprite) && (static_cast<Sprite*>(*sprite)->moeDies() == false) 
        && (player.calvinDies() == false) && godMode == false){
      player.explode();
      sound[0];
    }
    ++sprite;
  }
}

void Manager::draw() const {
  if(delay) clock.delay(); 
  mountains.draw();
  for (unsigned i = 0; i < (leaves.size()/3) ; ++i) {
    leaves[i]->draw();
  }
  trees.draw();
  for (unsigned i = (leaves.size()/3); i < (2*leaves.size()/3) ; ++i) {
    leaves[i]->draw();
  }
  grass.draw();
  for (unsigned i = (2*leaves.size()/3); i < leaves.size() ; ++i) {
    leaves[i]->draw();
  }
  clock.draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  io.printMessageAt(title, 280, 10);
  io.printMessageAt(gameTitle, 10, 440);
  io.printMessageValueAt("Lives lost ",player.getLives(), 720, 10);
  player.draw();
  if(player.won() == true && player.getLives() <= 3){
    io.printMessageAt("YOU HAVE WON!", 280, 220);
    hobbes->draw();
  }
  else if (player.won() == true && player.getLives() > 3){
    io.printMessageAt("YOU HAVE LOST... TRY AGAIN...", 280, 220);
  }
  if(godMode == true && end == false){
    io.printMessageAt("God Mode is on", 680, 40);
  }
  if(clock.getSeconds() < hudTime && !f1Pressed){
    display.drawHUD(io.getScreen(),clock.getSeconds(),clock.getFps());
  }
  else if (showHud==true){
    display.drawHUD(io.getScreen(),clock.getSeconds(),clock.getFps());
  }
  viewport.draw();
  SDL_Flip(screen);
}

void Manager::update() {
  clock.update();
  if(delay) clock.delay();
  Uint32 ticks = clock.getTicksSinceLastFrame();
  for (unsigned i = 0; i < leaves.size(); ++i) {
    leaves[i]->update(ticks);
  }
  
  if ( makeVideo && frameCount < frameMax ) {
    io.makeFrame();
  }
  mountains.update();
  trees.update();
  grass.update();
  for (unsigned i = 0; i < sprites.size(); ++i) { 
    sprites[i]->update(ticks);
  }
  hobbes->update(ticks);
  player.update(ticks);
  checkForCollisions();
  if(player.won() == true && player.getLives() <= 3){
    playEndMusic();
  }
  if (player.won() == true && player.getLives() > 3){
    playEndMusic();
  }
  viewport.update(); // always update viewport last
}

void Manager::playEndMusic(){
  if (end == false  && player.won() == true && player.getLives() <= 3){
    sound[2];
    godMode = true;
    end = true;
  }
  else if (end == false && player.won() == true && player.getLives() > 3){
    sound[4];  
    godMode = true;
    end = true; 
  }
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  bool keyPressed = false;
  clock.start();
  while ( not done ) {
    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      player.stop(); keyPressed = false;
    }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
        done = true;
        sound.toggleMusic();
        break;
      }
      if (keystate[SDLK_r]){
        keyPressed = true;
        player.reset();
        player.resetLives();
        kill = 0;
        godMode = false;
        end = false;
      }
      if (keystate[SDLK_g] && !keyPressed){
        keyPressed = true;
        if(godMode==true) {godMode = false;}
        else if(godMode == false) {godMode = true;}
      }
      if ( keystate[SDLK_p]) {
        keyPressed = true;
        if ( clock.isPaused()) clock.unpause();
        else clock.pause();
      }
      if (keystate[SDLK_F4] && !makeVideo) {
        keyPressed = true;
        std::cout << "Making video frames" << std::endl;
        makeVideo = true;
      }
      if (keystate[SDLK_b] && !keyPressed) {
        keyPressed = true;
        sound[3];
        player.shoot(); 
      }
      if ( keystate[SDLK_F1] && !keyPressed) {
        if(clock.getSeconds()< hudTime && !f1Pressed)
        showHud = true; 
        keyPressed = true; f1Pressed = true;
        if(showHud==true) {showHud = false;}
        else if(showHud == false) {showHud = true;}
        draw();
        }
      }
      if (keystate[SDLK_s] || keystate[SDLK_DOWN] ) {
	player.down();
      }
      if (keystate[SDLK_w] || keystate[SDLK_UP]) {
        player.up();  
      }
      if (keystate[SDLK_a] || keystate[SDLK_LEFT] ) {
        player.left();  
      }

      if (keystate[SDLK_d] || keystate[SDLK_RIGHT] ) {
        player.right();
      }
      if (keystate[SDLK_SPACE]) {
        player.jumping();  
      } 
    draw();
    update();
  }
}
