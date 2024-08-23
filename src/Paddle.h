#ifndef SRC_PADDLE_h_
#define SRC_PADDLE_h_

#include "types.h"

class Game;

class Paddle {
public:
    Paddle() = default;
    Paddle(Game* game, bool is_left);
    ~Paddle();

    void update(Game* game);
    void render(Game* game);

    inline void addScore() {
        this->score += 1;
    }

    inline int getScore() const {
        return this->score;
    }

private:
    Vec2 pos;
    Vec2 size;
    bool left;
    int score;
};

#endif
