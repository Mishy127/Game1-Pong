#include "Paddle.h"

#include "Game.h"

Paddle::Paddle(Game* game, bool l): size{ 10, 150 }, left(l), score(0) {
    int win_w, win_h;

    SDL_GetWindowSize(game->getWindow(), &win_w, &win_h);

    this->pos.x = (l)
        ? this->size.x
        : win_w - this->size.x;
    this->pos.y = win_h / 2;
}

Paddle::~Paddle() {}

void Paddle::update(Game* game) {
    bool go_up, go_down;

    if (this->left) {
        go_up = game->isKeyDown(SDLK_w);
        go_down = game->isKeyDown(SDLK_s);
    } else {
        go_up = game->isKeyDown(SDLK_UP);
        go_down = game->isKeyDown(SDLK_DOWN);
    }

    if (go_up and this->pos.y > this->size.y / 2)
        this->pos.y = std::max(this->pos.y - 1000 * game->getDeltaTime(), this->size.y / 2);
    else if (go_down) {
        int win_h;

        SDL_GetWindowSize(game->getWindow(), nullptr, &win_h);

        this->pos.y = std::min(this->pos.y + 1000 * game->getDeltaTime(), win_h - this->size.y / 2);
    }

    auto ball = game->getBall();

    auto ball_pos = ball->getPos();
    auto ball_size = ball->getSize();

    if (
        // Ball right  greater than Paddle left
        ((ball_pos.x + ball_size.x / 2) > (this->pos.x - this->size.x / 2)) and
        // Bal left       less than Paddle right
        ((ball_pos.x - ball_size.x / 2) < (this->pos.x + this->size.x / 2)) and
        ((ball_pos.y + ball_size.y / 2) > (this->pos.y - this->size.y / 2)) and
        ((ball_pos.y - ball_size.y / 2) < (this->pos.y + this->size.y / 2))
    ) {
        auto ball_old = ball->getOldPos();
        auto ball_speed = ball->getSpeed();

        if (!(ball_old.x + ball_size.x / 2 > this->pos.x - this->size.x / 2)) {
            ball_pos.x = (this->pos.x + this->size.x / 2) - ball_size.x;
            ball_speed.x = -std::abs(ball_speed.x);
        } else
        if (!(ball_old.x - ball_size.x / 2 < this->pos.x + this->size.x / 2)) {
            ball_pos.x = (this->pos.x - this->size.x / 2) + ball_size.x;
            ball_speed.x = std::abs(ball_speed.x);
        }

        if (!(ball_old.y + ball_size.y / 2 > this->pos.y - this->size.y / 2)) {
            ball_pos.y = (this->pos.y - this->size.y / 2) + ball_size.y;
            ball_speed.y = -std::abs(ball_speed.y);
        } else
        if (!(ball_old.y - ball_size.y / 2 < this->pos.y + this->size.y / 2)) {
            ball_pos.y = (this->pos.y + this->size.y / 2) - ball_size.y;
            ball_speed.y = std::abs(ball_speed.y);
        }

        ball->setPos(ball_pos);
        ball->setSpeed(ball_speed);

        Mix_PlayChannel(-1, game->getEffect(1), 0);
    }
}

void Paddle::render(Game* game) {
    SDL_FRect rect = {
        this->pos.x - this->size.x / 2,
        this->pos.y - this->size.y / 2,
        this->size.x,
        this->size.y
    };

    SDL_SetRenderDrawColor(game->getCanvas(), 0xFF, 0xFF, 0xFF, 0xFF);

    SDL_RenderFillRectF(game->getCanvas(), &rect);
}
