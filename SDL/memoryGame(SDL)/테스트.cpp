#include "SDLMemoryGame.h"

#define WIDTH 100*16
#define HEIGHT 100*9

int main(int argc, char* argv[]) {
    srand(time(NULL));
    // SDL 준비
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    SDL_Rect destRect;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow("SDL Memory Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    TTF_Init();

    // 이미지 준비
    char cardImage[15][22] = {
        "imgs/character_0.png",  "imgs/character_1.png",  "imgs/character_2.png",
        "imgs/character_3.png",  "imgs/character_4.png",  "imgs/character_5.png",
        "imgs/character_6.png",  "imgs/character_7.png",  "imgs/character_8.png",
        "imgs/character_9.png",  "imgs/character_10.png", "imgs/character_11.png",
        "imgs/character_12.png", "imgs/character_13.png", "imgs/character_14.png"
    };
    SDL_Surface* imageSurface[15];
    for (int i = 0; i < 15; i++) { imageSurface[i] = IMG_Load(cardImage[i]); }
    SDL_Texture* taxtureBack = SDL_CreateTextureFromSurface(renderer, imageSurface[0]);
    const int w = imageSurface[0]->w;
    const int h = imageSurface[0]->h;
    SDL_Texture* taxtureFronts[14];
    SDL_FreeSurface(imageSurface[0]);
    for (int i = 0; i < 14; i++) {
        taxtureFronts[i] = SDL_CreateTextureFromSurface(renderer, imageSurface[i + 1]);
        SDL_FreeSurface(imageSurface[i + 1]);
    }

    // 폰트 준비
    TTF_Font* font = TTF_OpenFont("fonts/NGULIM.ttf", 24);


    GameManager manager(renderer, taxtureBack, taxtureFronts, font);
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point point = { event.button.x, event.button.y };
                int result = manager.eventClick(point);
                if (result == 1) {
                }
                else if (result == -1) {
                }
            }
        }
        manager.renderAll();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_CloseFont(font);
    SDL_Quit();
    return 0;
}