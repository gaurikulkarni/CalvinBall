#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"

class ExplodingSprite;
class Sprite : public Drawable {
public:
  Sprite(const std::string& n, const Vector2f& pos, const Vector2f& vel);
  Sprite(const std::string&);
  Sprite(const std::string&, 
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  Sprite(const Sprite& s);
  virtual ~Sprite() { } 
  Sprite& operator=(const Sprite&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;

  virtual void update(Uint32 ticks);
  void explode();
  bool moeDies();

protected:
  ExplodingSprite* explosion;
  int upperLimit;
  int lowerLimit;
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  bool isExploding;
  int getDistance(const Sprite*) const;
  bool turn;
};
#endif
