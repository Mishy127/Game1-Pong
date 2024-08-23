#ifndef SRC_GAME_h_
#define SRC_GAME_h_

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>

#include "Ball.h"
#include "Paddle.h"

class Game {
    SDL_Window* window;
    SDL_Renderer* canvas;
    std::map<int, bool> keys;
    Mix_Chunk* effects[3];

    float deltatime;

    Ball ball;
    Paddle paddles[2];
public:
    bool solo;

    Game();
    ~Game();
    int run();
    void score(bool left);

    inline bool isKeyDown(int key) {
        return this->keys[key];
    }

    inline SDL_Window* getWindow() const {
        return this->window;
    };

    inline SDL_Renderer* getCanvas() const {
        return this->canvas;
    }

    inline float getDeltaTime() const {
        return this->deltatime;
    }

    inline Ball* getBall() {
        return &this->ball;
    }

    inline Mix_Chunk* getEffect(int i) {
        return this->effects[i];
    }

private:
    void update();
    void render();
};

#endif
