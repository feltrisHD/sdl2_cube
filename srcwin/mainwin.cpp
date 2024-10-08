#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "font.h"

extern unsigned char Font_ttf[];
extern unsigned int Font_ttf_len;

int buttonpress = 0;
std::string textToRender;
const int BUTTON_WIDTH = 400;
const int BUTTON_HEIGHT = 100;
const int BUTTON_WIDTH2 = 100;
const int BUTTON_HEIGHT2 = 50; 
const int BUTTON_X = 0;
const int BUTTON_Y = 100;
const int BUTTON_X2 = 0;
const int BUTTON_Y2 = 200;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int CUBE_SIZE = 30;
const int HOVER_CUBE_SIZE = 25;

bool isMouseOverButton(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight) {
    return x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight;
}

SDL_Texture* renderText(const std::string &message, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer) {
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return texture;
}

struct Cube {
    float x, y; 
    bool fromTop;
};

bool renderGameOverScreen(SDL_Renderer* renderer, TTF_Font* font, int score) {
    SDL_Color textColor = { 255, 0, 0, 255 }; 
    std::string gameOverText = "Game Over - Score: " + std::to_string(score);
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, gameOverText.c_str(), textColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);

    SDL_Rect gameOverRect;
    gameOverRect.x = (SCREEN_WIDTH - gameOverSurface->w) / 2;
    gameOverRect.y = (SCREEN_HEIGHT - gameOverSurface->h) / 2 - 50;
    gameOverRect.w = gameOverSurface->w;
    gameOverRect.h = gameOverSurface->h;

  SDL_RWops *rw = SDL_RWFromConstMem(Font_ttf, Font_ttf_len);
    TTF_Font *font2 = TTF_OpenFontRW(rw, 1, 50); 

    SDL_Color buttonTextColor = { 255, 255, 255, 255 };
    SDL_Texture* playAgainTexture = renderText("Play Again", font2, buttonTextColor, renderer);
    SDL_Rect playAgainRect = { (SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - BUTTON_HEIGHT2) / 2 + 50, 200, BUTTON_HEIGHT2 };

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
    SDL_RenderCopy(renderer, playAgainTexture, nullptr, &playAgainRect);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(gameOverSurface);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(playAgainTexture);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                return false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseOverButton(x, y, playAgainRect.x, playAgainRect.y, playAgainRect.w, playAgainRect.h)) {
                    return true;
                    return 0;
                }
            }
        }
    }
    return false;
}

int main(int argc, char* args[]) {
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("sdl2_cube starter",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       800, 600,
                                       SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_DestroyWindow(win);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_RWops *rw = SDL_RWFromConstMem(Font_ttf, Font_ttf_len);
    TTF_Font *font = TTF_OpenFontRW(rw, 1, 50); 
    if (font == nullptr) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                return 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseOverButton(x, y, BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
                    running = false;
                    buttonpress = 1;
                } else if (isMouseOverButton(x, y, BUTTON_X2, BUTTON_Y2, BUTTON_WIDTH2, BUTTON_HEIGHT2)) {
                    running = false;
                    return 0;
                    buttonpress = 2; // Quit button pressed
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_Rect buttonRect = { BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
        SDL_RenderFillRect(renderer, &buttonRect);

        SDL_Color textColor = {255, 255, 255};
        SDL_Texture *textTexture = renderText("Start sdl2_cube", font, textColor, renderer);
        if (textTexture != nullptr) {
            int texW = 0;
            int texH = 0;
            SDL_QueryTexture(textTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect renderQuad = {(BUTTON_WIDTH - texW) / 2 + BUTTON_X, (BUTTON_HEIGHT - texH) / 2 + BUTTON_Y, texW, texH};
            SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
            SDL_DestroyTexture(textTexture);
        }

        SDL_Color textColor2 = { 255, 255, 255, 255 }; 
        SDL_Texture *textTexture2 = renderText("Quit", font, textColor2, renderer);
        if (textTexture2 != nullptr) {
            int texW = 0;
            int texH = 0;
            SDL_QueryTexture(textTexture2, nullptr, nullptr, &texW, &texH);
            SDL_Rect renderQuad = {(BUTTON_WIDTH2 - texW) / 2 + BUTTON_X2, (BUTTON_HEIGHT2 - texH) / 2 + BUTTON_Y2, texW, texH};
            SDL_RenderCopy(renderer, textTexture2, nullptr, &renderQuad);
            SDL_DestroyTexture(textTexture2);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    if (buttonpress == 1) {
        while (true) {
            if (TTF_Init() == -1) {
                std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
                SDL_Quit();
                return 1;
            }

            rw = SDL_RWFromConstMem(Font_ttf, Font_ttf_len);
            font = TTF_OpenFontRW(rw, 1, 30);
            if (font == nullptr) {
                std::cerr << "TTF_OpenFontRW Error: " << TTF_GetError() << std::endl;
                SDL_Quit();
                return 1;
            }

            SDL_Window* window = SDL_CreateWindow("sdl2_cube",
                                                  SDL_WINDOWPOS_CENTERED,
                                                  SDL_WINDOWPOS_CENTERED,
                                                  SCREEN_WIDTH,
                                                  SCREEN_HEIGHT,
                                                  SDL_WINDOW_SHOWN);

            if (window == nullptr) {
                std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
                TTF_Quit();
                SDL_Quit();
                return 1;
            }

            SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == nullptr) {
                SDL_DestroyWindow(window);
                std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
                TTF_Quit();
                SDL_Quit();
                return 1;
            }

            bool quit = false;
            bool gameOver = false;
            int score = 0;
            float mainX = SCREEN_WIDTH / 2 - CUBE_SIZE / 2;
            float mainY = SCREEN_HEIGHT / 2 - CUBE_SIZE / 2;

            std::vector<Cube> hoverCubes;

            Uint32 lastFrameTime = SDL_GetTicks();
            Uint32 targetFrameTime = 1000 / 60;

            bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;

            Uint32 FrameTime = 1000 / 60; // 60 FPS

            while (!quit) {
                Uint32 currentTime = SDL_GetTicks();
                Uint32 deltaTime = currentTime - lastFrameTime;
                lastFrameTime = currentTime;

                SDL_Event e;
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                        return 0;
                    } else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_w:
                                moveUp = true;
                                break;
                            case SDLK_s:
                                moveDown = true;
                                break;
                            case SDLK_a:
                                moveLeft = true;
                                break;
                            case SDLK_d:
                                moveRight = true;
                                break;
                        }
                    } else if (e.type == SDL_KEYUP) {
                        switch (e.key.keysym.sym) {
                            case SDLK_w:
                                moveUp = false;
                                break;
                            case SDLK_s:
                                moveDown = false;
                                break;
                            case SDLK_a:
                                moveLeft = false;
                                break;
                            case SDLK_d:
                                moveRight = false;
                                break;
                        }
                    }
                }

                if (gameOver) {
                    if (renderGameOverScreen(renderer, font, score)) {
                        gameOver = false;
                        score = 0;
                        mainX = SCREEN_WIDTH / 2 - CUBE_SIZE / 2;
                        mainY = SCREEN_HEIGHT / 2 - CUBE_SIZE / 2;
                        hoverCubes.clear();
                    } else {
                        quit = true;
                        return 0;
                    }
                    break;
                }

                const float speed = 0.2f; 
                if (moveUp) mainY -= speed * deltaTime;
                if (moveDown) mainY += speed * deltaTime;
                if (moveLeft) mainX -= speed * deltaTime;
                if (moveRight) mainX += speed * deltaTime;
                score++;

                if (mainX < 0) mainX = 0;
                if (mainX + CUBE_SIZE > SCREEN_WIDTH) mainX = SCREEN_WIDTH - CUBE_SIZE;
                if (mainY < 0) mainY = 0;
                if (mainY + CUBE_SIZE > SCREEN_HEIGHT) mainY = SCREEN_HEIGHT - CUBE_SIZE;

                const Uint32 initialSpawnInterval = 300; 
                const float spawnAcceleration = 0.99f; 
                static Uint32 lastSpawnTime = SDL_GetTicks();
                Uint32 spawnInterval = initialSpawnInterval * std::pow(spawnAcceleration, currentTime / 1000.0f);
                if (currentTime - lastSpawnTime > spawnInterval) {
                    Cube newCube;
                    if (std::rand() % 2 == 0) {
                        newCube.x = std::rand() % SCREEN_WIDTH;
                        newCube.y = 0;
                        newCube.fromTop = true;
                    } else {
                        newCube.x = 0;
                        newCube.y = std::rand() % SCREEN_HEIGHT;
                        newCube.fromTop = false;
                    }
                    hoverCubes.push_back(newCube);
                    lastSpawnTime = currentTime;
                }

                for (auto it = hoverCubes.begin(); it != hoverCubes.end();) {
                    if (it->fromTop) {
                        it->y += 0.3f * deltaTime; 
                        if (it->y > SCREEN_HEIGHT) {
                            it = hoverCubes.erase(it);
                            continue;
                        }
                    } else {
                        it->x += 0.3f * deltaTime;
                        if (it->x > SCREEN_WIDTH) {
                            it = hoverCubes.erase(it);
                            continue;
                        }
                    }
                    ++it;
                }

                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);

                for (const auto& cube : hoverCubes) {
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); 
                    SDL_Rect hoverRect = { static_cast<int>(cube.x), static_cast<int>(cube.y), HOVER_CUBE_SIZE, HOVER_CUBE_SIZE };
                    SDL_RenderFillRect(renderer, &hoverRect);
                }

                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); 
                SDL_Rect mainRect = { static_cast<int>(mainX), static_cast<int>(mainY), CUBE_SIZE, CUBE_SIZE };
                SDL_RenderFillRect(renderer, &mainRect);

                SDL_Color textColor = { 0, 0, 0, 255 }; 
                std::string scoreText = "Score: " + std::to_string(score);
                SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
                SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
                SDL_Rect scoreRect = { 10, 10, scoreSurface->w, scoreSurface->h };
                SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);

                SDL_FreeSurface(scoreSurface);
                SDL_DestroyTexture(scoreTexture);

                SDL_RenderPresent(renderer);

                SDL_Rect mainCubeRect = { static_cast<int>(mainX), static_cast<int>(mainY), CUBE_SIZE, CUBE_SIZE };
                for (const auto& cube : hoverCubes) {
                    SDL_Rect hoverCubeRect = { static_cast<int>(cube.x), static_cast<int>(cube.y), HOVER_CUBE_SIZE, HOVER_CUBE_SIZE };
                    if (SDL_HasIntersection(&mainCubeRect, &hoverCubeRect)) {
                        gameOver = true;
                        break;
                    }
                }

                Uint32 frameTime = SDL_GetTicks() - currentTime;
                if (frameTime < targetFrameTime) {
                    SDL_Delay(targetFrameTime - frameTime);
                }
            }

            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_CloseFont(font);
            TTF_Quit();
            SDL_Quit();
        }
    }

    return 0;
}
