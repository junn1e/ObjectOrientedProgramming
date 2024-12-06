#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

#define WIDTH 100*16
#define HEIGHT 100*9

using namespace std;

// ī�� Ŭ����
class Card {
protected:
    SDL_Renderer* renderer;
    SDL_Texture* taxtureFront;
    SDL_Texture* taxtureBack;
    int cardNumber;
    bool isFlipped;
    bool isCorrect;

public:
    SDL_Rect rect; // �ڵ� ������ ����� ���� public���� ����

    Card(int num, SDL_Renderer* render, SDL_Rect rect, SDL_Texture* front, SDL_Texture* back) {
        renderer = render;
        cardNumber = num;
        this->rect = rect;
        taxtureFront = front;
        taxtureBack = back;
        isFlipped = false;
        isCorrect = false;
    }
    void render() {
        SDL_RenderCopy(renderer, this->isFlipped ? taxtureFront : taxtureBack, nullptr, &rect);
    }
    // ī�� ������
    int flip(int progress) {
        if (isFlipped) return 0;
        isFlipped = true;
        render();
        if (cardNumber == progress) {
            isCorrect = true;
            return 1;
        }
        SDL_Delay(500);
        isFlipped = false;
        render();
        return -1;
    }
    void reset() {
        isFlipped = false;
        isCorrect = false;
    }
};

class CardManager {
private:
    vector<Card*> cardStack1;
    vector<Card*> cardStack2;
    int gameLevel;

public:
    CardManager(SDL_Renderer* renderer, SDL_Texture* taxtureBack, SDL_Texture* taxtureFronts[], int i = 3) {
        gameLevel = i;
        for (int i = 0; i < 14; i++) {
            SDL_Rect rect = { 0, -300 };
            cardStack1.push_back(new Card(i, renderer, rect, taxtureFronts[i], taxtureBack));
        }
    }

    // ī�� ��ġ ����
    void shuffle(int from, int to) {
        int w = 100, h = 100;
        to = (to < 14) ? to : 14;

        for (int i = from; i < to; ++i) {
            SDL_Rect rect;
            bool overlap;
            do {
                overlap = false;
                rect = { rand() % (WIDTH - w), rand() % (HEIGHT - h), w, h };

                for (Card* card : cardStack1) {
                    if (SDL_HasIntersection(&rect, &card->rect)) {
                        overlap = true;
                        break;
                    }
                }
            } while (overlap);
        }
    }

    // ī�� ��ü ������
    void renderAll() {
        for (Card* card : cardStack1) {
            card->render();
        }
        for (Card* card : cardStack2) {
            card->render();
        }
    }

    // Ŭ�� �̺�Ʈ������
    int eventClick(SDL_Point point, int progress) {
        for (int i = 0; i < gameLevel; i++) {
            if (SDL_PointInRect(&point, &cardStack1[i]->rect)) {
                return cardStack1[i]->flip(progress);
            }
        }
        return 0;
    }
};

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

    // 1. char[] �غ�
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
        taxtureFronts[i] = SDL_CreateTextureFromSurface(renderer, imageSurface[i+1]);
        SDL_FreeSurface(imageSurface[i+1]);
    }

    srand(time(NULL));

    CardManager manager(renderer, taxtureBack, taxtureFronts);

    bool quit = false;
    SDL_Event event;
    int progress = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point point = { event.button.x, event.button.y };
                int result = manager.eventClick(point, progress);
                if (result == 1) {
                }
                else if (result == -1) {
                }
            }
        }

        SDL_RenderClear(renderer);
        manager.renderAll();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
