#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <stdlib.h>

#define WIDTH 100*16
#define HEIGHT 100*9

typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
    int dx, dy;
    bool isFlipped;
    bool isCorrect;
    int cardIndex;
} Card4Memory;

int main(int argc, char* argv[]) {
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


    // ==================== !코드시작! ====================

    srand(time(NULL));

    // 1. char[] 준비
    char cardImage[15][22] = {
        "imgs/character_0.png",  "imgs/character_1.png",  "imgs/character_2.png",
        "imgs/character_3.png",  "imgs/character_4.png",  "imgs/character_5.png",
        "imgs/character_6.png",  "imgs/character_7.png",  "imgs/character_8.png",
        "imgs/character_9.png",  "imgs/character_10.png", "imgs/character_11.png",
        "imgs/character_12.png", "imgs/character_13.png", "imgs/character_14.png"
    };
    
    // 2. char[] -> SDL_Surface*
    SDL_Surface* imageSurface[15];
    for (int i = 0; i < 15; i++) {
        imageSurface[i] = IMG_Load(cardImage[i]);
    }
    
    // 3. SDL_Surface* -> SDL_Texture*
    SDL_Texture* cardTextures[15];
    // w, h 정보 저장
    cardTextures[0] = SDL_CreateTextureFromSurface(renderer, imageSurface[0]);
    const int w = imageSurface[0]->w;
    const int h = imageSurface[0]->h;
    SDL_FreeSurface(imageSurface[0]);

    for (int i = 1; i < 15; i++) {
        cardTextures[i] = SDL_CreateTextureFromSurface(renderer, imageSurface[i]);
        SDL_FreeSurface(imageSurface[i]);
    }

    // 카드 정보 설정
    Card4Memory card4Game[14];
    for (int i = 0; i < 14; i++) {
        card4Game[i].texture = cardTextures[i + 1];

        bool overlap;
        SDL_Rect rect;
        
        do {
            overlap = false;
            rect = { rand() % (WIDTH - w), rand() % (HEIGHT - h), w, h };
            // SDL_HasIntersection : 다른 객체와 겹침 여부 리턴
            for (int j = 0; j < i; j++) {
                if (SDL_HasIntersection(&rect, &card4Game[j].rect)) {
                    overlap = true;
                    break;
                }
            }
        } while (overlap);

        card4Game[i].rect = rect;
        card4Game[i].isFlipped = false;
        card4Game[i].isCorrect = false;
        card4Game[i].cardIndex = i;
    }

    // 4초 동안 카드 표시
    for (int i = 0; i < 14; i++) {
        SDL_RenderCopy(renderer, cardTextures[i + 1], NULL, &card4Game[i].rect);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);

    // 카드 숨기기
    for (int i = 0; i < 14; i++) {
        card4Game[i].texture = cardTextures[0];
        card4Game[i].isFlipped = false;
    }

    // 메시지 루프
    SDL_Event event;
    int quit = 0;
    int progress = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point point = { event.button.x, event.button.y };
                // 마우스 눌렀을 때
                for (int i = 0; i < 15; i++) {
                    if (SDL_PointInRect(&point, &card4Game[i].rect) && !card4Game[i].isFlipped && !card4Game[i].isCorrect) {
                        // 클릭된 카드에 대해 실행
                        card4Game[i].isFlipped = true;
                        if (card4Game[i].isCorrect) continue;
                        // 보이기
                        card4Game[i].texture = cardTextures[i + 1];

                        if (i = progress) {
                            // 카드가 맞는 경우
                            card4Game[i].isCorrect = true;
                            progress++;
                        }
                        else {
                            // 카드가 틀린 경우
                            SDL_Delay(500);
                            card4Game[i].texture = cardTextures[0];
                        }
                    }
                }
            }
        }

        SDL_RenderClear(renderer);
        for (int i = 0; i < 14; i++) {
            SDL_RenderCopy(renderer, card4Game[i].texture, NULL, &card4Game[i].rect);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    return 0;
}
