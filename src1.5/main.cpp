#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include "font.h"
extern unsigned char Font_ttf[];
extern unsigned int Font_ttf_len;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int CUBE_SIZE = 30;
const int HOVER_CUBE_SIZE = 25;

struct Cube {
    float x, y; 
    bool fromTop;
};

void renderGameOverScreen(SDL_Renderer* renderer, TTF_Font* font, int score) {
 
    SDL_Color textColor = { 255, 0, 0, 255 }; 
    std::string gameOverText = "Game Over - Score: " + std::to_string(score);
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, gameOverText.c_str(), textColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);

    SDL_Rect gameOverRect;
    gameOverRect.x = (SCREEN_WIDTH - gameOverSurface->w) / 2;
    gameOverRect.y = (SCREEN_HEIGHT - gameOverSurface->h) / 2;
    gameOverRect.w = gameOverSurface->w;
    gameOverRect.h = gameOverSurface->h;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
    SDL_RenderPresent(renderer);


    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }
    }

  
    SDL_FreeSurface(gameOverSurface);
    SDL_DestroyTexture(gameOverTexture);
}

int main(int argc, char* args[]) {
  
   

 
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

   SDL_RWops *rw = SDL_RWFromConstMem(Font_ttf, Font_ttf_len);
    TTF_Font* font = TTF_OpenFontRW(rw, 1, 30);
   

    SDL_Window* window = SDL_CreateWindow("Cub",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
  
  
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    

    float mainX = 400.0f;
    float mainY = 500.0f;

    std::vector<Cube> hoverCubes;

    std::srand(static_cast<unsigned>(std::time(0)));

    bool quit = false;

    SDL_Event e;

    bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;

    int score = 0;

    bool gameOver = false;

    Uint32 lastFrameTime = SDL_GetTicks();
    const Uint32 targetFrameTime = 1000 / 60; 

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        moveUp = true;
                        break;
                    case SDLK_DOWN:
                        moveDown = true;
                        break;
                    case SDLK_LEFT:
                        moveLeft = true;
                        break;
                    case SDLK_RIGHT:
                        moveRight = true;
                        break;
                }
            }
            else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        moveUp = false;
                        break;
                    case SDLK_DOWN:
                        moveDown = false;
                        break;
                    case SDLK_LEFT:
                        moveLeft = false;
                        break;
                    case SDLK_RIGHT:
                        moveRight = false;
                        break;
                }
            }
        }

        if (gameOver) {
            renderGameOverScreen(renderer, font, score);
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

    return 0;
}
