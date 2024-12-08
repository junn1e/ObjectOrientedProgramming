#include "SDLMemoryGame.h"
#include <iostream>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <SDL.h>

using namespace std;

void renderText(SDL_Renderer* renderer, TTF_Font* font, int progress) {
    char scoreText[32];
    sprintf(scoreText, "score: %d", progress);

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, scoreText, textColor);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect;
    textRect.x = 10;
    textRect.y = 10;
    textRect.w = 200;
    textRect.h = 50;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

// 카드 클래스 정의
Card::Card(int num, SDL_Renderer* render, SDL_Texture* front, SDL_Texture* back) {
    SDL_Rect rect = { WIDTH / 2, HEIGHT / 2, 100, 100 };
    this->renderer = render;
    cardNumber = num;
    this->rect = rect;
    taxtureFront = front;
    taxtureBack = back;
    isFlipped = false;
    isCorrect = false;
}

void Card::render() const {
    SDL_RenderCopy(renderer, this->isFlipped ? taxtureFront : taxtureBack, nullptr, &rect);
}

void Card::reset(bool isFlipped) {
    this->isFlipped = isFlipped;
    isCorrect = false;
}

NormalCard::NormalCard(int num, SDL_Renderer* render, SDL_Texture* front, SDL_Texture* back)
    : Card(num, render, front, back) {}

int NormalCard::flip(int progress) {
    if (isFlipped) return 0;

    isFlipped = true;
    render();
    SDL_RenderPresent(renderer);

    if (cardNumber == progress) {
        isCorrect = true;
        return 1;
    }
    SDL_Delay(500);

    isFlipped = false;
    render();
    SDL_RenderPresent(renderer);

    return -1;
}

SpecialCard::SpecialCard(int num, SDL_Renderer* render, SDL_Texture* front, SDL_Texture* back)
    : Card(num, render, front, back) {}

int SpecialCard::flip(int progress) {
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

// GameManager 클래스 정의
GameManager::GameManager(SDL_Renderer* renderer, SDL_Texture* taxtureBack, SDL_Texture* taxtureFronts[], TTF_Font* font, int startLv)
    : renderer(renderer), taxtureBack(taxtureBack), font(font), gameLevel(startLv), progress(0) {

    for (int i = 0; i < 14; i++) {
        cardStack1.push_back(new NormalCard(i, renderer, taxtureFronts[i], taxtureBack));
    }

    SDL_Surface* imageSurface1 = IMG_Load("imgs/card_bomb.png");
    SDL_Surface* imageSurface2 = IMG_Load("imgs/card_eye.png");
    taxtureBomb = SDL_CreateTextureFromSurface(renderer, imageSurface1);
    taxtureEyes = SDL_CreateTextureFromSurface(renderer, imageSurface2);
    SDL_FreeSurface(imageSurface1);
    SDL_FreeSurface(imageSurface2);

    setLevel(gameLevel);
}

void GameManager::shuffle(int from, int to) {
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

void GameManager::renderAll() {
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

int GameManager::eventClick(SDL_Point point) {
    for (int i = 0; i < gameLevel; i++) {
        if (SDL_PointInRect(&point, &cardStack1[i]->rect)) {
            switch (cardStack1[i]->flip(progress)) {
            case 1:
                progress++;
                score += 1 + scoreBonus;
                scoreBonus++;
                if (progress == gameLevel) setLevel(gameLevel + 1);
                break;
            case -1:
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
            case 1:
                for (int i = progress; i < gameLevel; i++) {
                    cardStack1[i]->reset(true);
                }
                for (Card* card1 : cardStack2) {
                    card1->reset(true);
                }
                renderAll();
                SDL_Delay(1000);
                delete card;
                cardStack2.erase(it);
                break;
            case 0:
                score -= 10;
                delete card;
                cardStack2.erase(it);
                shuffle(progress, gameLevel);
                renderAll();
                SDL_Delay(500);
                for (int i = progress; i < gameLevel; i++) {
                    cardStack1[i]->reset(true);
                }
                for (Card* card1 : cardStack2) {
                    card1->reset(true);
                }
                renderAll();
                SDL_Delay(2000);
                break;
            default:
                break;
            }
            for (int i = progress; i < gameLevel; i++) {
                cardStack1[i]->reset(false);
            }
            for (Card* card1 : cardStack2) {
                card1->reset(false);
            }
            renderAll();
            break;
        }
    }
    return 0;
}

void GameManager::setLevel(int targetLevel) {
    gameLevel = (targetLevel < 15) ? targetLevel : 15;
    progress = 0;
    cardStack2.clear();
    cardStack2.push_back(new SpecialCard(0, renderer, taxtureBomb, taxtureBack));
    cardStack2.push_back(new SpecialCard(1, renderer, taxtureEyes, taxtureBack));

    shuffle(0, gameLevel);
    for (Card* card : cardStack1) {
        card->reset(true);
    }
    for (Card* card : cardStack2) {
        card->reset(true);
    }
    renderAll();
    SDL_Delay(1000);
    for (Card* card : cardStack1) {
        card->reset(false);
    }
    for (Card* card : cardStack2) {
        card->reset(false);
    }
    renderAll();
}
