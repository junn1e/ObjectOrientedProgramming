#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

#define WIDTH 100*16
#define HEIGHT 100*9

using namespace std;

void renderText(SDL_Renderer* renderer, TTF_Font* font, int progress);

// 카드 클래스
class Card {
protected:
    SDL_Renderer* renderer;
    SDL_Texture* taxtureFront;
    SDL_Texture* taxtureBack;
    int cardNumber;
    bool isFlipped;
    bool isCorrect;

public:
    SDL_Rect rect; // 코드 가독성 향상을 위해 public으로 설정

    Card(int num, SDL_Renderer* render, SDL_Texture* front, SDL_Texture* back) {
        SDL_Rect rect = { WIDTH / 2, HEIGHT / 2, 100, 100 };
        renderer = render;
        cardNumber = num;
        this->rect = rect;
        taxtureFront = front;
        taxtureBack = back;
        isFlipped = false;
        isCorrect = false;
    }
    void render() const {
        SDL_RenderCopy(renderer, this->isFlipped ? taxtureFront : taxtureBack, nullptr, &rect);
    }
    virtual int flip(int progress) = 0;
    void reset(bool isFlipped) {
        this->isFlipped = isFlipped;
        isCorrect = false;
    }
};

class NormalCard : public Card {
public:
    NormalCard(int num, SDL_Renderer* render, SDL_Texture* front, SDL_Texture* back)
        : Card(num, render, front, back) {}

	int flip(int progress) override {
		cout << "progress : " << progress;
		if (isFlipped) return 0;

		isFlipped = true;
		render();
		SDL_RenderPresent(renderer);

		if (cardNumber == progress) {
			isCorrect = true;
			cout << ", correct. " << endl;
			return 1;
		}
		SDL_Delay(500);

		isFlipped = false;
		render();
		SDL_RenderPresent(renderer);

		cout << ", wrong. " << endl;
		return -1;
	}
};

class SpecialCard : public Card {
public:
    SpecialCard(int num, SDL_Renderer* render, SDL_Texture* front, SDL_Texture* back)
        : Card(num, render, front, back) {}

    int flip(int progress) override {
        cout << "Special Card chosen. number : " << cardNumber << endl;

        isFlipped = true;
        if (cardNumber == 0) {
            SDL_RenderClear(renderer);
            render();
            SDL_RenderPresent(renderer);
            SDL_Delay(500);
            return cardNumber;
        }
        render();
        SDL_RenderPresent(renderer);

        return cardNumber;
    }
};


class GameManager {
private:
    SDL_Renderer* renderer;
    vector<Card*> cardStack1;
    vector<Card*> cardStack2;
    TTF_Font* font;
    SDL_Texture* taxtureBomb;
    SDL_Texture* taxtureEyes;
    SDL_Texture* taxtureBack;
    int gameLevel;
    int progress;

    // 카드 위치 섞기
    void shuffle(int from, int to) {
        int w = 100, h = 100;
        to = (to < 14) ? to : 14;

        std::vector<Card*> allCards;
        allCards.insert(allCards.end(), cardStack1.begin(), cardStack1.end());
        allCards.insert(allCards.end(), cardStack2.begin(), cardStack2.end());

        for (int i = from; i < to; ++i) {
            SDL_Rect rect;
            bool overlap;

            do {
                overlap = false;
                rect = { rand() % (WIDTH - w), rand() % (HEIGHT - h - 100) + 100, w, h };

                // 충돌 검사 (모든 카드에 대해 한 번만 검사)
                for (Card* card : allCards) {
                    if (SDL_HasIntersection(&rect, &card->rect)) {
                        overlap = true;
                        break;
                    }
                }
            } while (overlap);

            cardStack1[i]->rect = rect;
        }

        for (Card* card : cardStack2) {
            SDL_Rect rect;
            bool overlap;

            do {
                overlap = false;
                rect = { rand() % (WIDTH - w), rand() % (HEIGHT - h - 100) + 100, w, h };

                for (Card* card : allCards) {
                    if (SDL_HasIntersection(&rect, &card->rect)) {
                        overlap = true;
                        break;
                    }
                }
            } while (overlap);

            card->rect = rect;
        }
    }


public:
    GameManager(SDL_Renderer* renderer, SDL_Texture* taxtureBack, SDL_Texture* taxtureFronts[], TTF_Font* font, int startLv = 3) {
        gameLevel = startLv;
        progress = 0;
        this->renderer = renderer;
        this->font = font;
        this->taxtureBack = taxtureBack;
        for (int i = 0; i < 14; i++) { cardStack1.push_back(new NormalCard(i, renderer, taxtureFronts[i], taxtureBack)); }

        // ------ 스페셜 카드 부분 ------
        SDL_Surface* imageSurface1 = IMG_Load("imgs/card_bomb.png");
        SDL_Surface* imageSurface2 = IMG_Load("imgs/card_eye.png");
        taxtureBomb = SDL_CreateTextureFromSurface(renderer, imageSurface1);
        taxtureEyes = SDL_CreateTextureFromSurface(renderer, imageSurface2);
        SDL_FreeSurface(imageSurface1);
        SDL_FreeSurface(imageSurface2);
        // ------ 스페셜 카드 부분 ------

        setLevel(gameLevel);
    }

    int score = 0;
    int scoreBonus = 0;

    // 카드 전체 렌더링
    void renderAll() {
        SDL_RenderClear(renderer);
        renderText(renderer, font, score);
        for (int i = 0; i < gameLevel; i++) {
            cardStack1[i]->render();
        }
        for (Card* card : cardStack2) {
            card->render();
        }
        SDL_RenderPresent(renderer); 
    }

    // 클릭 이벤트리스너
    int eventClick(SDL_Point point) {
        for (int i = 0; i < gameLevel; i++) {
            if (SDL_PointInRect(&point, &cardStack1[i]->rect)) {
                switch (cardStack1[i]->flip(progress))
                {
                case 1:  // 정답을 맞췄을 때
                    progress++;
                    score += 1 + scoreBonus;
                    scoreBonus++;
                    if (progress == gameLevel) setLevel(gameLevel + 1);
                    break;
                case -1: // 정답을 틀렸을 때
                    scoreBonus = 0;
                    break;
                default:
                    break;
                }
            }
        }
        for (auto it = cardStack2.begin(); it != cardStack2.end(); ++it) {
            Card* card = *it;
            if (SDL_PointInRect(&point, &card->rect)) {
                switch (card->flip(progress)) {
                case 1:  // 눈 카드
                    for (int i = progress; i < gameLevel; i++) { cardStack1[i]->reset(true); }
                    for (Card* card1 : cardStack2) { card1->reset(true); }
                    renderAll();
                    SDL_Delay(1000);
                    delete card;
                    cardStack2.erase(it);
                    break;
                case 0: // 폭탄 카드
                    score -= 10;
                    delete card;
                    cardStack2.erase(it);
                    shuffle(progress, gameLevel);
                    renderAll();
                    SDL_Delay(500);
                    for (int i = progress; i < gameLevel; i++) { cardStack1[i]->reset(true); }
                    for (Card* card1 : cardStack2) { card1->reset(true); }
                    renderAll();
                    SDL_Delay(2000);
                    break;
                default:
                    break;
                }
                for (int i = progress; i < gameLevel; i++) { cardStack1[i]->reset(false); }
                for (Card* card1 : cardStack2) { card1->reset(false); }
                renderAll();

                break;
            }
        }
        return 0;
    }

    // 선택한 레벨로 초기화
    void setLevel(int targetLevel) {
        gameLevel = (targetLevel < 15)? targetLevel : 15;
        progress = 0;
        cardStack2.clear();
        cardStack2.push_back(new SpecialCard(0, renderer, taxtureBomb, taxtureBack));
        cardStack2.push_back(new SpecialCard(1, renderer, taxtureEyes, taxtureBack));

        shuffle(0, gameLevel);
        for (Card* card : cardStack1) { card->reset(true); }
        for (Card* card : cardStack2) { card->reset(true); }
        renderAll();
        SDL_Delay(1000);
        for (Card* card : cardStack1) { card->reset(false); }
        for (Card* card : cardStack2) { card->reset(false); }
        renderAll();
    }
};

// 텍스트 출력 함수
void renderText(SDL_Renderer* renderer, TTF_Font* font, int progress) {
    char scoreText[32];
    sprintf(scoreText, "score: %d", progress);

    SDL_Color textColor = { 255, 255, 255, 255 }; // 흰색
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, scoreText, textColor);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect;
    textRect.x = 10;  // 텍스트 출력 위치 (x 좌표)
    textRect.y = 10;  // 텍스트 출력 위치 (y 좌표)
    textRect.w = 200; // 텍스트 너비
    textRect.h = 50;  // 텍스트 높이

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}


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
