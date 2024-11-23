#include <stdio.h> 
#include <iostream>
#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_messagebox.h>
#include <time.h>
#include <stdlib.h>

#define WIDTH 130*4
#define HEIGHT 130*4

typedef struct {
    SDL_Rect rect;
    int dx, dy;
    SDL_Texture* texture;
} Card4Memory;

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* imageSurface[16];
    SDL_Texture* texture = NULL;
    SDL_Rect destRect;

    Card4Memory card4Game[16];

    // SDL 초기화
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        return 1;
    }

    // 윈도우 창 생성
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

    char cardImage[8][16] = { "imgs/free01.png", "imgs/free02.png", "imgs/free03.png", "imgs/free04.png",
                              "imgs/free05.png", "imgs/free06.png", "imgs/free07.png", "imgs/free08.png" };
    int card[16] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7 };
    int selectedCard[16] = { 0 };   // 추가
    int cardtemp[16];
    int i, j, temp1, temp2;
    int clickedCell[2] = { -1, -1 };   // 추가

    srand(time(NULL));

    for (i = 0; i < 16; i++) cardtemp[i] = rand();

    for (i = 15; i > 0; i--) {
        for (j = 0; j < i; j++) {
            if (cardtemp[j] > cardtemp[j + 1]) {
                temp1 = cardtemp[j];
                cardtemp[j] = cardtemp[j + 1];
                cardtemp[j + 1] = temp1;

                temp2 = card[j];
                card[j] = card[j + 1];
                card[j + 1] = temp2;
            }
        }
    }

    for (i = 0; i < 16; i++) imageSurface[i] = IMG_Load(cardImage[card[i]]);
    for (i = 0; i < 16; i++) card4Game[i].texture = SDL_CreateTextureFromSurface(renderer, imageSurface[i]);
    for (i = 0; i < 4; i++) {
        card4Game[4 * i + 0].rect = { 0 * imageSurface[0]->w + 1, i * imageSurface[0]->h + 1, imageSurface[0]->w, imageSurface[0]->h };
        card4Game[4 * i + 1].rect = { 1 * imageSurface[0]->w + 2, i * imageSurface[0]->h + 1, imageSurface[0]->w, imageSurface[0]->h };
        card4Game[4 * i + 2].rect = { 2 * imageSurface[0]->w + 3, i * imageSurface[0]->h + 1, imageSurface[0]->w, imageSurface[0]->h };
        card4Game[4 * i + 3].rect = { 3 * imageSurface[0]->w + 4, i * imageSurface[0]->h + 1, imageSurface[0]->w, imageSurface[0]->h };
    }

    for (i = 0; i < 16; i++) SDL_RenderCopy(renderer, card4Game[i].texture, NULL, &card4Game[i].rect);
    SDL_RenderPresent(renderer);

    SDL_Delay(4000);

    for (i = 0; i < 16; i++) imageSurface[i] = IMG_Load("imgs/free09.png");
    for (i = 0; i < 16; i++) card4Game[i].texture = SDL_CreateTextureFromSurface(renderer, imageSurface[i]);
    for (i = 0; i < 4; i++) {
        card4Game[4 * i + 0].rect = { 0 * imageSurface[0]->w + 1, i * imageSurface[0]->h + 1, imageSurface[0]->w, imageSurface[0]->h };
        card4Game[4 * i + 1].rect = { 1 * imageSurface[0]->w + 2, i * imageSurface[0]->h + 1, imageSurface[0]->w, imageSurface[0]->h };
        card4Game[4 * i + 2].rect = { 2 * imageSurface[0]->w + 3, i * imageSurface[0]->h + 1, imageSurface[0]->w, imageSurface[0]->h };
        card4Game[4 * i + 3].rect = { 3 * imageSurface[0]->w + 4, i * imageSurface[0]->h + 1, imageSurface[0]->w, imageSurface[0]->h };
    }

    // 메시지 루프
    SDL_Event event;
    int quit = 0;
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
                //printf("(%d,%d)\n", event.button.x, event.button.y);
                if ((event.button.x >= (0 * imageSurface[0]->w + 1) && event.button.x <= (1 * imageSurface[0]->w + 1)) &&
                    (event.button.y >= (0 * imageSurface[0]->h + 1) && event.button.y <= (1 * imageSurface[0]->w + 1))) {
                    //printf("%d\n", card[0] + 1);
                    if (!selectedCard[0]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 0;
                        else {
                            if (clickedCell[0] != 0) clickedCell[1] = 0;
                        }
                    }
                }
                else if ((event.button.x >= (1 * imageSurface[0]->w + 2) && event.button.x <= (2 * imageSurface[0]->w + 2)) &&
                    (event.button.y >= (0 * imageSurface[0]->h + 1) && event.button.y <= (1 * imageSurface[0]->w + 1))) {
                    //printf("%d\n", card[1] + 1);
                    if (!selectedCard[1]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 1;
                        else {
                            if (clickedCell[0] != 1) clickedCell[1] = 1;
                        }
                    }
                }
                else if ((event.button.x >= (2 * imageSurface[0]->w + 3) && event.button.x <= (3 * imageSurface[0]->w + 3)) &&
                    (event.button.y >= (0 * imageSurface[0]->h + 1) && event.button.y <= (1 * imageSurface[0]->w + 1))) {
                    //printf("%d\n", card[2] + 1);
                    if (!selectedCard[2]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 2;
                        else {
                            if (clickedCell[0] != 2) clickedCell[1] = 2;
                        }
                    }
                }
                else if ((event.button.x >= (3 * imageSurface[0]->w + 4) && event.button.x <= (4 * imageSurface[0]->w + 4)) &&
                    (event.button.y >= (0 * imageSurface[0]->h + 1) && event.button.y <= (1 * imageSurface[0]->w + 1))) {
                    //printf("%d\n", card[3] + 1);
                    if (!selectedCard[3]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 3;
                        else {
                            if (clickedCell[0] != 3) clickedCell[1] = 3;
                        }
                    }
                }

                else if ((event.button.x >= (0 * imageSurface[0]->w + 1) && event.button.x <= (1 * imageSurface[0]->w + 1)) &&
                    (event.button.y >= (1 * imageSurface[0]->h + 2) && event.button.y <= (2 * imageSurface[0]->w + 2))) {
                    //printf("%d\n", card[4] + 1);
                    if (!selectedCard[4]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 4;
                        else {
                            if (clickedCell[0] != 4) clickedCell[1] = 4;
                        }
                    }
                }
                else if ((event.button.x >= (1 * imageSurface[0]->w + 2) && event.button.x <= (2 * imageSurface[0]->w + 2)) &&
                    (event.button.y >= (1 * imageSurface[0]->h + 2) && event.button.y <= (2 * imageSurface[0]->w + 2))) {
                    //printf("%d\n", card[5] + 1);
                    if (!selectedCard[5]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 5;
                        else {
                            if (clickedCell[0] != 5) clickedCell[1] = 5;
                        }
                    }
                }
                else if ((event.button.x >= (2 * imageSurface[0]->w + 3) && event.button.x <= (3 * imageSurface[0]->w + 3)) &&
                    (event.button.y >= (1 * imageSurface[0]->h + 2) && event.button.y <= (2 * imageSurface[0]->w + 2))) {
                    //printf("%d\n", card[6] + 1);
                    if (!selectedCard[6]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 6;
                        else {
                            if (clickedCell[0] != 6) clickedCell[1] = 6;
                        }
                    }
                }
                else if ((event.button.x >= (3 * imageSurface[0]->w + 4) && event.button.x <= (4 * imageSurface[0]->w + 4)) &&
                    (event.button.y >= (1 * imageSurface[0]->h + 2) && event.button.y <= (2 * imageSurface[0]->w + 2))) {
                    //printf("%d\n", card[7] + 1);
                    if (!selectedCard[7]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 7;
                        else {
                            if (clickedCell[0] != 7) clickedCell[1] = 7;
                        }
                    }
                }
                else if ((event.button.x >= (0 * imageSurface[0]->w + 1) && event.button.x <= (1 * imageSurface[0]->w + 1)) &&
                    (event.button.y >= (2 * imageSurface[0]->h + 3) && event.button.y <= (3 * imageSurface[0]->w + 3))) {
                    //printf("%d\n", card[8] + 1);
                    if (!selectedCard[8]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 8;
                        else {
                            if (clickedCell[0] != 8) clickedCell[1] = 8;
                        }
                    }
                }
                else if ((event.button.x >= (1 * imageSurface[0]->w + 2) && event.button.x <= (2 * imageSurface[0]->w + 2)) &&
                    (event.button.y >= (2 * imageSurface[0]->h + 3) && event.button.y <= (3 * imageSurface[0]->w + 3))) {
                    //printf("%d\n", card[9] + 1);
                    if (!selectedCard[9]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 9;
                        else {
                            if (clickedCell[0] != 9) clickedCell[1] = 9;
                        }
                    }
                }
                else if ((event.button.x >= (2 * imageSurface[0]->w + 3) && event.button.x <= (3 * imageSurface[0]->w + 3)) &&
                    (event.button.y >= (2 * imageSurface[0]->h + 3) && event.button.y <= (3 * imageSurface[0]->w + 3))) {
                    //printf("%d\n", card[10] + 1);
                    if (!selectedCard[10]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 10;
                        else {
                            if (clickedCell[0] != 10) clickedCell[1] = 10;
                        }
                    }
                }
                else if ((event.button.x >= (3 * imageSurface[0]->w + 4) && event.button.x <= (4 * imageSurface[0]->w + 4)) &&
                    (event.button.y >= (2 * imageSurface[0]->h + 3) && event.button.y <= (3 * imageSurface[0]->w + 3))) {
                    //printf("%d\n", card[11] + 1);
                    if (!selectedCard[11]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 11;
                        else {
                            if (clickedCell[0] != 11) clickedCell[1] = 11;
                        }
                    }
                }
                else if ((event.button.x >= (0 * imageSurface[0]->w + 1) && event.button.x <= (1 * imageSurface[0]->w + 1)) &&
                    (event.button.y >= (3 * imageSurface[0]->h + 4) && event.button.y <= (4 * imageSurface[0]->w + 4))) {
                    //printf("%d\n", card[12] + 1);
                    if (!selectedCard[12]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 12;
                        else {
                            if (clickedCell[0] != 12) clickedCell[1] = 12;
                        }
                    }
                }
                else if ((event.button.x >= (1 * imageSurface[0]->w + 2) && event.button.x <= (2 * imageSurface[0]->w + 2)) &&
                    (event.button.y >= (3 * imageSurface[0]->h + 4) && event.button.y <= (4 * imageSurface[0]->w + 4))) {
                    //printf("%d\n", card[13] + 1);
                    if (!selectedCard[13]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 13;
                        else {
                            if (clickedCell[0] != 13) clickedCell[1] = 13;
                        }
                    }
                }
                else if ((event.button.x >= (2 * imageSurface[0]->w + 3) && event.button.x <= (3 * imageSurface[0]->w + 3)) &&
                    (event.button.y >= (3 * imageSurface[0]->h + 4) && event.button.y <= (4 * imageSurface[0]->w + 4))) {
                    //printf("%d\n", card[14] + 1);
                    if (!selectedCard[14]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 14;
                        else {
                            if (clickedCell[0] != 14) clickedCell[1] = 14;
                        }
                    }
                }
                else if ((event.button.x >= (3 * imageSurface[0]->w + 4) && event.button.x <= (4 * imageSurface[0]->w + 4)) &&
                    (event.button.y >= (3 * imageSurface[0]->h + 4) && event.button.y <= (4 * imageSurface[0]->w + 4))) {
                    //printf("%d\n", card[15] + 1);
                    if (!selectedCard[15]) {
                        if (clickedCell[0] == -1) clickedCell[0] = 15;
                        else {
                            if (clickedCell[0] != 15) clickedCell[1] = 15;
                        }
                    }
                }

                if (clickedCell[0] != -1 && clickedCell[1] == -1) {
                    // 첫번째 카드를 선택했을때
                    //printf("First %s\n", cardImage[card[clickedCell[0]]]);
                    imageSurface[clickedCell[0]] = IMG_Load(cardImage[card[clickedCell[0]]]);
                    card4Game[clickedCell[0]].texture = SDL_CreateTextureFromSurface(renderer, imageSurface[clickedCell[0]]);
                    SDL_RenderCopy(renderer, card4Game[clickedCell[0]].texture, NULL, &card4Game[clickedCell[0]].rect);
                    SDL_RenderPresent(renderer);
                }
                else if (clickedCell[0] != -1 && clickedCell[1] != -1) {
                    // 두번째 카드를 선택했을때
                    //printf("Second %s\n", cardImage[card[clickedCell[1]]]);
                    imageSurface[clickedCell[1]] = IMG_Load(cardImage[card[clickedCell[1]]]);
                    card4Game[clickedCell[1]].texture = SDL_CreateTextureFromSurface(renderer, imageSurface[clickedCell[1]]);
                    SDL_RenderCopy(renderer, card4Game[clickedCell[1]].texture, NULL, &card4Game[clickedCell[1]].rect);
                    SDL_RenderPresent(renderer);

                    if (card[clickedCell[0]] == card[clickedCell[1]]) {
                        //printf("Same\n");
                        //printf("%d, %d\n", clickedCell[0], clickedCell[1]);
                        selectedCard[clickedCell[0]] = 1;
                        selectedCard[clickedCell[1]] = 1;

                        clickedCell[0] = clickedCell[1] = -1;

                        int sum = 0;
                        for (i = 0; i < 16; i++) sum += selectedCard[i];
                        if (sum == 16) printf("Success = %d\n", sum);
                    }
                    else {
                        //printf("Difference\n");
                        SDL_Delay(1000);

                        imageSurface[clickedCell[0]] = IMG_Load("imgs/free09.png");
                        card4Game[clickedCell[0]].texture = SDL_CreateTextureFromSurface(renderer, imageSurface[clickedCell[0]]);
                        SDL_RenderCopy(renderer, card4Game[clickedCell[0]].texture, NULL, &card4Game[clickedCell[0]].rect);
                        SDL_RenderPresent(renderer);

                        imageSurface[clickedCell[1]] = IMG_Load("imgs/free09.png");
                        card4Game[clickedCell[1]].texture = SDL_CreateTextureFromSurface(renderer, imageSurface[clickedCell[1]]);
                        SDL_RenderCopy(renderer, card4Game[clickedCell[1]].texture, NULL, &card4Game[clickedCell[1]].rect);
                        SDL_RenderPresent(renderer);

                        clickedCell[0] = clickedCell[1] = -1;
                    }
                }
            }
        }

        for (i = 0; i < 16; i++) SDL_RenderCopy(renderer, card4Game[i].texture, NULL, &card4Game[i].rect);
        SDL_RenderPresent(renderer);
    }

    // 종료
    for (i = 0; i < 16; i++) {
        SDL_DestroyTexture(card4Game[i].texture);
        SDL_FreeSurface(imageSurface[i]);
    }

    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}