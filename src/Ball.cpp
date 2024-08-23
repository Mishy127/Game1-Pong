#include "Ball.h"

#include "Game.h"

Ball::Ball(Game* game): speed{ 300, 300 }, size{ 10, 10 } {
    int x, y;

    SDL_GetWindowSize(game->getWindow(), &x, &y);

    this->pos = Vec2{ x / 2.0f, y / 2.0f };
    this->old = pos;
}

Ball::~Ball() {}

void Ball::update(Game* game) {
    this->old = this->pos;

    this->pos.x += this->speed.x * game->getDeltaTime();
    this->pos.y += this->speed.y * game->getDeltaTime();

    int win_w, win_h;

    SDL_GetWindowSize(game->getWindow(), &win_w, &win_h);

    if ((int) (this->pos.y + this->size.y / 2) > win_h) {
        this->pos.y = win_h - this->size.y / 2;
        this->speed.y = -std::abs(this->speed.y);

        Mix_PlayChannel(-1, game->getEffect(0), 0);
    }

    if ((int) (this->pos.y - this->size.y / 2) < 0) {
        this->pos.y = 0 + this->size.y / 2;
        this->speed.y = std::abs(this->speed.y);

        Mix_PlayChannel(-1, game->getEffect(0), 0);
    }

    if ((int) (this->pos.x - this->size.x / 2) < 0) {
        game->score(true);

        this->pos = Vec2{
            (float) win_w / 2,
            (float) win_h / 2
        };
        this->speed = Vec2{ -this->speed.x, -this->speed.y };

        Mix_PlayChannel(-1, game->getEffect(2), 0);
    } else if ((int) (this->pos.x + this->size.x / 2) > win_w) {
        game->score(false);

        this->pos = Vec2{
            (float) win_w / 2,
            (float) win_h / 2
        };
        this->speed = Vec2{ -this->speed.x, -this->speed.y };

        Mix_PlayChannel(-1, game->getEffect(2), 0);
    }
}

void Ball::render(Game* game) {
    SDL_SetRenderDrawColor(game->getCanvas(), 0xFF, 0xFF, 0xFF, 0xFF);

    SDL_FRect rect = {
        this->pos.x - this->size.x / 2,
        this->pos.y - this->size.y / 2,
        this->size.x,
        this->size.y
    };

    SDL_RenderFillRectF(game->getCanvas(), &rect);
}
