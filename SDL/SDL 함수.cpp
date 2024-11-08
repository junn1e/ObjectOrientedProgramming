#include<iostream>  //콘솔입출력

#include <time.h>   //랜덤
#include <stdlib.h>

#include <SDL.h>    //SDL
#include <SDL_image.h>
#include <SDL_messagebox.h>
using namespace std;

#define WIDTH  10*30
#define HEIGHT 10*20

void printObj(int x, int y, char ch);

class Gameobject {
public:
    int x, y, distance;
    SDL_Rect rect;
    SDL_Texture* texture;
    Gameobject(SDL_Renderer* renderer, int startX, int startY, int distance, char* imgName) {
        x = startX;
        y = startY;
        distance = distance;

        SDL_Surface* imageSurface = IMG_Load(imgName);
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
        rect = { startX * 10, startY * 10, imageSurface->w, imageSurface->h };
    }
    virtual ~Gameobject() {
        SDL_DestroyTexture(texture);
    };
    virtual void move() = 0;

    bool collide(Gameobject* p) {
        if (x == p->x && y == p->x) return true;
        else return false;
    }
};


class Human :public Gameobject {
public:
    Human(SDL_Renderer* renderer, int startX, int startY, int distance, char* imgName)
        :Gameobject(renderer, startX, startY, distance, imgName) {}
    void move()override {

    }
    ~Human() {

    };
};


class Monster :public Gameobject {
public:
    Monster(SDL_Renderer* renderer, int startX, int startY, int distance, char* imgName)
        :Gameobject(renderer, startX, startY, distance, imgName) {}
    void move()override {
        srand(time(NULL));
        switch (rand() % 4) {
        case 0:
            if (x < WIDTH - 1)  x += distance; break;
        case 1:
            if (x > 0)          x -= distance; break;
        case 2:
            if (y < HEIGHT - 1) y += distance; break;
        case 3:
            if (y > 0)          y -= distance; break;
        }
    }
    ~Monster() {};
};

class Food :public Gameobject {
    int* arr;
    int cursor;
    int base, moveRate;

    void shuffle() {
        for (int i = 0; i < base - 1; i++) {
            int rn = rand() % (base - i) + i;
            int temp = arr[i];
            arr[i] = arr[rn];
            arr[rn] = temp;
        }
    }

public:
    Food(SDL_Renderer* renderer, int startX, int startY, int distance, char* imgName)
        :Gameobject(renderer, startX, startY, distance, imgName) {}
    void setMoveFreq(int moveRate, int base) {
        this->moveRate = moveRate;
        this->base = base;
        this->cursor = 0;
        this->arr = (int*)malloc(sizeof(int) * base);

        for (int i = 0; i < base; i++) {
            if (i < moveRate)
                this->arr[i] = 1;
            else
                this->arr[i] = 0;
        }
        srand((unsigned int)time(NULL));
        shuffle();
    }

    int pick() {
        if (this->cursor >= base) {
            shuffle();
            this->cursor = 0;
        }
        return this->arr[cursor++];
    }

    Food(int startX, int startY, int distance) :Gameobject(startX, startY, distance) {
        setMoveFreq(3, 5);
    }
    void move()override {
        if (this->pick() == 0) return;
        srand(time(NULL));
        switch (rand() % 4) {
        case 0:
            if (x < WIDTH - 1)  x += distance; break;
        case 1:
            if (x > 0)          x -= distance; break;
        case 2:
            if (y < HEIGHT - 1) y += distance; break;
        case 3:
            if (y > 0)          y -= distance; break;
        }
    }
    ~Food() {
        free(this->arr);
    };
};

int main() {                                        //----------메인----------
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* imageSurface = NULL;
    SDL_Texture* texture = NULL;
    SDL_Rect destRect;
    // SDL 초기화
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        return 1;
    }
    // 윈도우 창 생성
    window = SDL_CreateWindow("SDL Mini Game",
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

    //SDL_Renderer * renderer, int startX, int startY, int distance, char* imgName
    Human player(renderer, 3, 1, 1, "img/free01.png");

    SDL_Event event;//메시지 루프 (update)
    int quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                }



                break;
            case SDL_QUIT:
                quit = 1;
                break;
            default:
                break;
            }
        }

        player.move();

        SDL_RenderCopy(renderer, player.texture, NULL, &player.rect);
        SDL_RenderPresent(renderer);
    }

    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
