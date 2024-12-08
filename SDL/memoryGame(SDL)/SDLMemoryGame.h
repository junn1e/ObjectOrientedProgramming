#ifndef SDL_MEMORY_GAME_H
#define SDL_MEMORY_GAME_H

#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <vector>
#include <ctime>

#define WIDTH 100*16
#define HEIGHT 100*9

#include <iostream>
#include <sstream>
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

    Card(int num, SDL_Renderer* render, SDL_Texture* front, SDL_Texture* back);
    void render() const;
    virtual int flip(int progress) = 0;
    void reset(bool isFlipped);
};

class NormalCard : public Card {
public:
    NormalCard(int num, SDL_Renderer* render, SDL_Texture* front, SDL_Texture* back);
    int flip(int progress) override;
};

class SpecialCard : public Card {
public:
    SpecialCard(int num, SDL_Renderer* render, SDL_Texture* front, SDL_Texture* back);
    int flip(int progress) override;
};

class GameManager {
private:
    SDL_Renderer* renderer;
    std::vector<Card*> cardStack1;
    std::vector<Card*> cardStack2;
    TTF_Font* font;
    SDL_Texture* taxtureBomb;
    SDL_Texture* taxtureEyes;
    SDL_Texture* taxtureBack;
    int gameLevel;
    int progress;

    void shuffle(int from, int to);

public:
    GameManager(SDL_Renderer* renderer, SDL_Texture* taxtureBack, SDL_Texture* taxtureFronts[], TTF_Font* font, int startLv = 3);
    int score = 0;
    int scoreBonus = 0;

    void renderAll();
    int eventClick(SDL_Point point);
    void setLevel(int targetLevel);
};

#endif // SDL_MEMORY_GAME_H
