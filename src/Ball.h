#ifndef SRC_BALL_h_
#define SRC_BALL_h_

#include "types.h"

class Game;

class Ball {
public:
    Ball() = default;
    Ball(Game* game);
    ~Ball();
    
    void update(Game* game);
    void render(Game* game);

    inline Vec2 getPos() const {
        return this->pos;
    }

    inline void setPos(Vec2 pos) {
        this->pos = pos;
    }

    inline Vec2 getOldPos() const {
        return this->old;
    }

    inline Vec2 getSpeed() const {
        return this->speed;
    }

    inline void setSpeed(Vec2 speed) {
        this->speed = speed;
    }

    inline Vec2 getSize() const {
        return this->size;
    }

private:
    Vec2 pos;
    Vec2 old;
    Vec2 speed;
    Vec2 size;
};

#endif

