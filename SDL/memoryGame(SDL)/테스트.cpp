#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

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

void updateScreen(SDL_Renderer* renderer, Card4Memory* card4Game) {
    SDL_RenderClear(renderer);
    for (int i = 0; i < 14; i++) {
        SDL_RenderCopy(renderer, card4Game[i].texture, NULL, &card4Game[i].rect);
    }
    SDL_RenderPresent(renderer);
}

class Card {
protected:
    SDL_Rect rect;
    SDL_Texture* texture;
    bool isFlipped;
    bool isCorrect;

public:
    Card(SDL_Rect rect, SDL_Texture* texture) : 
        rect(rect), texture(texture), isFlipped(false), isCorrect(false) {}

    virtual void render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    virtual void flip(SDL_Texture* frontTexture, SDL_Texture* backTexture) {
        isFlipped = !isFlipped;
        texture = isFlipped ? frontTexture : backTexture;
    }

    virtual bool isMatch(int index) {
        return false;
    }

    SDL_Rect getRect() const { return rect; }
    bool isFlippedState() const { return isFlipped; }
};

class SpecialCard : public Card {
private:
    int effectType; // 0: 점수 배가, 1: 시간 연장 등

public:
    SpecialCard(SDL_Rect rect, SDL_Texture* texture, int effectType)
        : Card(rect, texture), effectType(effectType) {}

    void applyEffect(int& level, int& timeLeft) {
        if (effectType == 0) {
            level += 1; // 레벨 증가
        }
        else if (effectType == 1) {
            timeLeft += 5; // 시간 추가
        }
    }

    bool isMatch(int index) override {
        return effectType == index; // 특별한 매칭 로직
    }
};

class CardManager {
private:
    std::vector<Card*> cardPile;

public:
    void addCard(Card* card) {
        cardPile.push_back(card);
    }

    void renderAll(SDL_Renderer* renderer) {
        for (Card* card : cardPile) {
            card->render(renderer);
        }
    }

    void checkClick(int x, int y, SDL_Texture* backTexture, SDL_Texture* frontTextures[]) {
        for (Card* card : cardPile) {
            SDL_Rect rect = card->getRect();
            if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h && !card->isFlippedState()) {
                card->flip(frontTextures[0], backTexture);
            }
        }
    }
};



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
    updateScreen(renderer, card4Game);
    SDL_Delay(2000);

    // 카드 숨기기
    for (int i = 0; i < 14; i++) {
        card4Game[i].texture = cardTextures[0];
        card4Game[i].isFlipped = false;
    }
    updateScreen(renderer, card4Game);

    // 메시지 루프
    SDL_Event event;
    int quit = 0;
    int level = 10;
    int progress = 0;
    while (level < 16 && !quit)
    {
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
                for (int i = 0; i < 15; i++) {
                    if (SDL_PointInRect(&point, &card4Game[i].rect) && !card4Game[i].isFlipped && !card4Game[i].isCorrect) {
                        card4Game[i].isFlipped = true;

                        // 카드 열기
                        card4Game[i].texture = cardTextures[i + 1];
                        updateScreen(renderer, card4Game);
                        SDL_Delay(2000);  // 카드가 열린 상태를 유지

                        // 정답 확인
                        if (i == progress) {
                            card4Game[i].isCorrect = true; // 맞는 카드는 유지
                            progress++; // 정답일 때만 증가
                            printf("correct");
                        }
                        else {
                            // 틀린 경우 카드 닫기
                            card4Game[i].isFlipped = false;
                            card4Game[i].texture = cardTextures[0];
                            printf("wrong");
                        }

                        // 상태 렌더링
                        updateScreen(renderer, card4Game);
                    }
                }
            }
        }
        updateScreen(renderer, card4Game);
        if (level == progress) {
            SDL_Quit();
            return 0;
        }
    }
}
