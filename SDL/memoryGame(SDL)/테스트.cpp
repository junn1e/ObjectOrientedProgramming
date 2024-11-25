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
    // SDL �غ�
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


    // ==================== !�ڵ����! ====================

    srand(time(NULL));

    // 1. char[] �غ�
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
    // w, h ���� ����
    cardTextures[0] = SDL_CreateTextureFromSurface(renderer, imageSurface[0]);
    const int w = imageSurface[0]->w;
    const int h = imageSurface[0]->h;
    SDL_FreeSurface(imageSurface[0]);

    for (int i = 1; i < 15; i++) {
        cardTextures[i] = SDL_CreateTextureFromSurface(renderer, imageSurface[i]);
        SDL_FreeSurface(imageSurface[i]);
    }

    // ī�� ���� ����
    Card4Memory card4Game[14];
    for (int i = 0; i < 14; i++) {
        card4Game[i].texture = cardTextures[i + 1];

        bool overlap;
        SDL_Rect rect;
        
        do {
            overlap = false;
            rect = { rand() % (WIDTH - w), rand() % (HEIGHT - h), w, h };
            // SDL_HasIntersection : �ٸ� ��ü�� ��ħ ���� ����
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

    // 4�� ���� ī�� ǥ��
    for (int i = 0; i < 14; i++) {
        SDL_RenderCopy(renderer, cardTextures[i + 1], NULL, &card4Game[i].rect);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);

    // ī�� �����
    for (int i = 0; i < 14; i++) {
        card4Game[i].texture = cardTextures[0];
        card4Game[i].isFlipped = false;
    }

    // �޽��� ����
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
                // ���콺 ������ ��
                for (int i = 0; i < 15; i++) {
                    if (SDL_PointInRect(&point, &card4Game[i].rect) && !card4Game[i].isFlipped && !card4Game[i].isCorrect) {
                        // Ŭ���� ī�忡 ���� ����
                        card4Game[i].isFlipped = true;
                        if (card4Game[i].isCorrect) continue;
                        // ���̱�
                        card4Game[i].texture = cardTextures[i + 1];

                        if (i = progress) {
                            // ī�尡 �´� ���
                            card4Game[i].isCorrect = true;
                            progress++;
                        }
                        else {
                            // ī�尡 Ʋ�� ���
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
