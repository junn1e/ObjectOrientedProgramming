#include<iostream>  //콘솔입출력
#include<string.h>
#include <sstream>

#include <time.h>   //랜덤
#include <stdlib.h>

#include <SDL.h>    //SDL
#include <SDL_image.h>
#include <SDL_messagebox.h>
#include <SDL_ttf.h>
using namespace std;

#define OBJ_SIZE 30
#define WIDTH  30
#define HEIGHT 20

class ListPicker {
public:
    ListPicker(int moveRate, int base) {
        setFreq(moveRate, base);
    }
    bool * arr;
    int cursor, base, moveRate;

    void shuffle() {
        for (int i = 0; i < base - 1; i++) {
            int rn = rand() % (base - i) + i;
            int temp = arr[i];
            arr[i] = arr[rn];
            arr[rn] = temp;
        }
    }

    int pick() {
        if (this->cursor >= base) {
            shuffle();
            this->cursor = 0;
        }
        return this->arr[cursor++];
    }
    
    void setFreq(int moveRate, int base) {
        this->moveRate = moveRate;
        this->base = base;
        this->cursor = 0;
        free(this->arr);
        this->arr = (bool*)malloc(sizeof(bool) * base);

        for (int i = 0; i < base; i++) {
            if (i < moveRate)
                this->arr[i] = 1;
            else
                this->arr[i] = 0;
        }
        shuffle();
    }
    ~ListPicker() {
        free(this->arr);
    }
};

class GameObject {
public:
    int x, y, distance;
    SDL_Rect rect;
    SDL_Texture* texture;
    GameObject(SDL_Renderer* renderer, int startX, int startY, int distance, const char* imgName) {
        x = startX;
        y = startY;
        distance = distance;

        SDL_Surface* imageSurface = IMG_Load(imgName);
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        rect = { startX * OBJ_SIZE, startY * OBJ_SIZE, OBJ_SIZE, OBJ_SIZE };
        SDL_FreeSurface(imageSurface);
    }
    virtual ~GameObject() {
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

    bool collide(GameObject* p) {
        if (x == p->x && y == p->y) return true;
        else return false;
    }
};

class Human :public GameObject {
public:
    Human(SDL_Renderer* renderer, int startX, int startY, int distance, const char* imgName)
        :GameObject(    renderer,     startX,     startY,     distance,             imgName) {}

    void move(int dx, int dy)override {
        GameObject::move(dx, dy);
    }
    ~Human() {

    };
};

class Monster :public GameObject {
public:
    Monster(SDL_Renderer* renderer, int startX, int startY, int distance, const char* imgName)
        :GameObject(      renderer, startX, startY, distance, imgName) {
    }
    bool birth = true;
    void move(int dx, int dy)override {
        birth = false;
        switch (rand() % 4) {
        case 0:
            GameObject::move(1 , 0); break;
        case 1:
            GameObject::move(-1, 0); break;
        case 2:
            GameObject::move(0 , 1); break;
        case 3:
            GameObject::move(0 ,-1); break;
        }
    }
    ~Monster() {};
};

class Goal :public GameObject {
    ListPicker picker;
public:
    Goal(SDL_Renderer* renderer, int startX, int startY, int distance, const char* imgName)
        :GameObject(   renderer,     startX,     startY,     distance,             imgName) , picker(3,5) {
        GameObject::move(0, 0);//시작했을 때 출력 안되는 문제 해결
    }
    
    void setMoveFreq(int moveRate, int base) {
        picker.setFreq(moveRate, base);
    }

    void move(int dx, int dy)override {
        if (picker.pick()) return;
        switch (rand() % 4) {
        case 0:
            GameObject::move(1, 0); break;
        case 1:
            GameObject::move(-1, 0); break;
        case 2:
            GameObject::move(0, 1); break;
        case 3:
            GameObject::move(0, -1); break;
        }
    }
    ~Goal() {
    };
};


struct StackNode {
    GameObject* data;
    struct StackNode* link;
};
class EnemyType {
    StackNode* top = NULL;
    SDL_Renderer* renderer;
    int distance;
    const char* imgName;
public:
    EnemyType(SDL_Renderer* renderer, int distance, const char* imgName) {
        this->renderer = renderer;
        this->distance = distance;
        this->imgName  = imgName;
    }
    void add() {
        if (distance == 0) return;
        StackNode* temp = (StackNode*)malloc(sizeof(StackNode));
        int startX = rand() % (WIDTH - 3)  + 3;
        int startY = rand() % (HEIGHT - 3) + 3;
        temp->data = new Monster(renderer, startX, startY, distance, imgName);
        temp->link = top;
        top = temp;
    }
    void render() {
        StackNode* temp = top;
        while (temp != NULL) {
            SDL_RenderCopy(renderer, temp->data->texture, NULL, &temp->data->rect);
            temp = temp->link;
        }
    }
    void move() {
        StackNode* temp = top;
        while (temp != NULL) {
            temp->data->move(0, 0);
            SDL_RenderCopy(renderer, temp->data->texture, NULL, &temp->data->rect);
            temp = temp->link;
        }
    }
    bool check(GameObject* target) {
        StackNode* temp = top;
        while (temp != NULL) {
            if(temp->data->collide(target)) return true;// && temp->data->birth == false
            temp = temp->link;
        }
        return false;
    }
    void collapse() {
        while (top != NULL) {
            StackNode* temp = top;
            GameObject* data = temp->data;
            top = top->link;
            free(temp->data);
            free(temp);
        }
    }
};

void renderScore(SDL_Renderer* renderer, TTF_Font* font, int score) {
    SDL_Color color = { 255, 255, 255 }; // 흰색 텍스트

    // stringstream으로 숫자를 문자열로 변환
    std::stringstream ss;
    ss << "Score: " << score;
    std::string scoreText = ss.str();

    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = { 10, 10, surface->w, surface->h }; // 왼쪽 위에 텍스트 배치
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int main(int argc, char* argv[]) {                 //----------메인----------
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
        OBJ_SIZE * WIDTH , OBJ_SIZE * HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("fonts/NGULIM.ttf", 24);
    if (!font) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
        return -1;
    }
    int score = 0;

    int lastAddTime = SDL_GetTicks(); //시간확인
    int lastAddTime1 = SDL_GetTicks();
    int addInterval = 3000;

    //SDL_Renderer * renderer, int startX, int startY, int distance, char* imgName
    Human   player(renderer, 3, 1, 1, "imgs/num_0.jpg");
    Goal    food1 (renderer, 20, 20, 2, "imgs/num_3.jpg");
    EnemyType enemyPile1(renderer, 2, "imgs/num_1.jpg");

    SDL_Event event;//메시지 루프 (update)
    int quit = 0;
    while (!quit)
    {
        int currentTime = SDL_GetTicks();
        int currentTime1 = SDL_GetTicks();
        if (currentTime - lastAddTime >= addInterval) {
            enemyPile1.add();
            lastAddTime = currentTime;
        }
        if (currentTime1 - lastAddTime1 >= 500) {
            enemyPile1.move();
            lastAddTime1 = currentTime1;
        }
        
        SDL_RenderClear(renderer);
        enemyPile1.render();
        SDL_RenderCopy(renderer, food1.texture, NULL, &food1.rect);
        SDL_RenderCopy(renderer, player.texture, NULL, &player.rect);
        renderScore(renderer, font, score);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                food1.move(0, 0);
                
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

        if (enemyPile1.check(&player)) {
            quit = 1;
        }
        if (player.collide(&food1)) {
            score++;
            enemyPile1.collapse();
            food1.x = rand() % (WIDTH - 3) + 3;
            food1.y = rand() % (HEIGHT - 3) + 3;
            food1.move(0, 0);
            addInterval -= 1000;
        }
    }
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
