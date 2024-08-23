
#include "Game.h"

Game::Game() {
    SDL_Init(SDL_INIT_VIDEO);

    this->window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    this->canvas = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    this->ball = Ball(this);
    this->paddles[0] = Paddle(this, true);
    this->paddles[1] = Paddle(this, false);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    this->effects[0] = Mix_LoadWAV("assets/hit-wall.wav");
    this->effects[1] = Mix_LoadWAV("assets/hit-paddle.wav");
    this->effects[2] = Mix_LoadWAV("assets/score.wav");

    this->solo = true;
}

Game::~Game() {
    Mix_FreeChunk(this->effects[0]);
    Mix_FreeChunk(this->effects[1]);
    Mix_FreeChunk(this->effects[2]);

    Mix_Quit();

    SDL_DestroyRenderer(this->canvas);
    SDL_DestroyWindow(this->window);

    SDL_Quit();
}

int Game::run() {
    auto active = true;
    SDL_Event event;

    auto past_update = SDL_GetTicks();
    while (active) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    active = false;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.repeat == 0)
                        this->keys[event.key.keysym.sym] = true;
                    break;
                case SDL_KEYUP:
                    this->keys[event.key.keysym.sym] = false;
                    break;
                default:
                    continue;
            }
        }

        auto last_update = SDL_GetTicks();

        this->deltatime = std::min((last_update - past_update) / 1000.0, 1.0);

        past_update = last_update;

        this->update();
        this->render();
    }

    return 0;
}

void renderScore(int score, bool left, SDL_Window* window, SDL_Renderer* canvas) {
    if (score > 0) {
        bool numbers[10][15] = {
            {
                0, 1, 0,
                1, 0, 1,
                1, 0, 1,
                1, 0, 1,
                0, 1, 0,
            },
            {
                1, 1, 0,
                0, 1, 0,
                0, 1, 0,
                0, 1, 0,
                1, 1, 1
            },
            {
                0, 1, 0,
                1, 0, 1,
                0, 0, 1,
                0, 1, 0,
                1, 1, 1,
            },
            {
                1, 1, 0,
                0, 0, 1,
                0, 1, 1,
                0, 0, 1,
                1, 1, 0,
            },
            {
                1, 0, 1,
                1, 0, 1,
                1, 1, 1,
                0, 0, 1,
                0, 0, 1,
            },
            {
                1, 1, 1,
                1, 0, 0,
                1, 1, 1,
                0, 0, 1,
                1, 1, 0,
            },
            {
                0, 1, 1,
                1, 0, 0,
                1, 1, 1,
                1, 0, 1,
                0, 1, 0,
            },
            {
                1, 1, 1,
                0, 0, 1,
                0, 0, 1,
                0, 1, 0,
                0, 1, 0,
            },
            {
                0, 1, 0,
                1, 0, 1,
                0, 1, 0,
                1, 0, 1,
                0, 1, 0,
            },
            {
                0, 1, 0,
                1, 0, 1,
                0, 1, 1,
                0, 0, 1,
                0, 0, 1,
            }
        };

        int win_w, win_h;

        SDL_GetWindowSize(window, &win_w, &win_h);

        int digits = std::log10(score) + 1;
        
        auto goffset_x = !left * (win_w / 2) + (win_w / 2) / 2 - ((32 * 3) * digits + 16 * (digits - 1)) / 2;
        auto goffset_y = win_h / 4;

        SDL_SetRenderDrawColor(canvas, 0x88, 0x88, 0x88, 0x88);

        for (int digit = 0; digit < digits; digit += 1) {
            auto n = numbers[score % 10];
            
            auto offset = (digits - (digit + 1)) * (32 * 3 + 16);

            for (int i = 0; i < 15; i += 1) {
                if (!n[i])
                    continue;

                int x = i % 3;
                int y = i / 3;

                SDL_Rect rect = {
                    goffset_x + offset + x * 32,
                    goffset_y + y * 32,
                    32,
                    32
                };

                SDL_RenderFillRect(canvas, &rect);
            }

            score = score / 10;
        }
    }

}

void Game::update() {
    this->ball.update(this);
    this->paddles[0].update(this);
    this->paddles[1].update(this);
}

void Game::score(bool left) {
    if (left) {
        this->paddles[0].addScore();
    } else {
        this->paddles[1].addScore();
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(this->canvas, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(this->canvas);

    renderScore(this->paddles[0].getScore(), true, this->window, this->canvas);
    renderScore(this->paddles[1].getScore(), false, this->window, this->canvas);

    int win_w, win_h;

    SDL_GetWindowSize(this->window, &win_w, &win_h);

    SDL_SetRenderDrawColor(canvas, 0x88, 0x88, 0x88, 0x88);
    SDL_RenderDrawLine(this->canvas, win_w / 2, 0, win_w / 2, win_h);

    this->ball.render(this);
    this->paddles[0].render(this);
    this->paddles[1].render(this);

    SDL_RenderPresent(this->canvas);
}
