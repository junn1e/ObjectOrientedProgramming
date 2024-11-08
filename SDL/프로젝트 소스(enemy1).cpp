#include<iostream>  //콘솔입출력

#include <time.h>   //랜덤
#include <stdlib.h>

#include <SDL.h>    //SDL
#include <SDL_image.h>
#include <SDL_messagebox.h>
using namespace std;

#define OBJ_SIZE 30
#define WIDTH  30
#define HEIGHT 20

class Gameobject {
public:
    int x, y, distance;
    SDL_Rect rect;
    SDL_Texture* texture;
    Gameobject(SDL_Renderer* renderer, int startX, int startY, int distance, const char* imgName) {
        x = startX;
        y = startY;
        distance = distance;

        SDL_Surface* imageSurface = IMG_Load(imgName);
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        rect = { startX * OBJ_SIZE, startY * OBJ_SIZE, OBJ_SIZE, OBJ_SIZE };
        SDL_FreeSurface(imageSurface);
    }
    virtual ~Gameobject() {
        SDL_DestroyTexture(texture);
    };
    virtual void move(int dx, int dy) {
        x += dx;
        y += dy;
        if (x > WIDTH - 2)  x = WIDTH - 2;
        if (x < 1)          x = 1;
        if (y > HEIGHT - 2) y = HEIGHT - 2;
        if (y < 1)          y = 1;

        rect = { x * OBJ_SIZE, y * OBJ_SIZE, OBJ_SIZE, OBJ_SIZE };
    }

    bool collide(Gameobject* p) {
        if (x == p->x && y == p->x) return true;
        else return false;
    }
};


class Human :public Gameobject {
public:
    Human(SDL_Renderer* renderer, int startX, int startY, int distance, const char* imgName)
        :Gameobject(renderer, startX, startY, distance, imgName) {}

    void move(int dx, int dy)override {
        Gameobject::move(dx, dy);
    }
    ~Human() {

    };
};

class Monster :public Gameobject {
public:
    Monster(SDL_Renderer* renderer, int startX, int startY, int distance, const char* imgName)
        :Gameobject(renderer, startX, startY, distance, imgName) {}
    void move(int dx, int dy)override {
        switch (rand() % 4) {
        case 0:
            Gameobject::move(1, 0); break;
        case 1:
            Gameobject::move(-1, 0); break;
        case 2:
            Gameobject::move(0, 1); break;
        case 3:
            Gameobject::move(0, -1); break;
        }
    }
    ~Monster() {};
};


int main(int argc, char* argv[]) {                                        //----------메인----------
    srand(time(NULL));
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* imageSurface = NULL;
    SDL_Texture* texture = NULL;
    SDL_Rect destRect;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow("SDL Mini Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        OBJ_SIZE * WIDTH, OBJ_SIZE * HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    //SDL_Renderer * renderer, int startX, int startY, int distance, char* imgName
    Human   player(renderer, 3, 1, 1, "imgs/num_0.jpg");
    Monster enemy1(renderer, 10, 10, 2, "imgs/num_1.jpg");

    SDL_Event event;//메시지 루프 (update)
    int quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_w) {
                    player.move(0, -1);
                }
                if (event.key.keysym.sym == SDLK_a) {
                    player.move(-1, 0);
                }
                if (event.key.keysym.sym == SDLK_s) {
                    player.move(0, 1);
                }
                if (event.key.keysym.sym == SDLK_d) {
                    player.move(1, 0);
                }
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
        enemy1.move(0, 0);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, enemy1.texture, NULL, &enemy1.rect);
        SDL_RenderCopy(renderer, player.texture, NULL, &player.rect);
        SDL_RenderPresent(renderer);
    }

    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
