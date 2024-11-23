#include<iostream>
using namespace std;
#include<Windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 100
#define HEIGHT 20

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ARROW 224

void printObj(int x, int y, char ch);
char getInput();

class Gameobject {
protected:
    int distance;   //한번 이동거리
    int x, y;   //현재위치 
public:
    Gameobject(int startX, int startY, int distance) {  //생성자. 초기 위치와 이동 거리 설정
        this->x = startX;
        this->y = startY;
        this->distance = distance;
    }
    virtual ~Gameobject() {};   //소멸자
    virtual void move() = 0;    //이동 후 x,y 를 새 위치로 변경
    virtual char getShape() = 0;    //Human = H / Monster = M / Food = @

    //충돌 판별
    int getX() { return x; }
    int getY() { return y; }
    bool collide(Gameobject* p) {
        if (this->x == p->getX() && this->y == p->getY()) return true;
        else return false;
    }
};


class Human :public Gameobject {
public:
    Human(int startX, int startY, int distance) :Gameobject(startX, startY, distance) {}
    void move()override {

        switch (getInput()) {
        case RIGHT:
            if (x < WIDTH - 1)  x += distance; break;
        case LEFT:
            if (x > 0)          x -= distance; break;
        case DOWN:
            if (y < HEIGHT - 1) y += distance; break;
        case UP:
            if (y > 0)          y -= distance; break;
        }
    }
    char getShape() override {
        return 'H';
    }
    ~Human() {};
};


class Monster :public Gameobject {
public:
    Monster(int startX, int startY, int distance) :Gameobject(startX, startY, distance) {}
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
    char getShape() override {
        return 'M';
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
    char getShape() override {
        return '%';
    }
    ~Food() {
        free(this->arr);
    };
};

void printObj(Gameobject* p) {
    COORD Cur;
    Cur.X = p->getX();
    Cur.Y = p->getY();
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
    cout << p->getShape();
    Cur.X = 0;
    Cur.Y = HEIGHT + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void printField() {
    system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << "_";
        }
        cout << endl;
    }
}

char getInput() {
    char c;
    if (_kbhit) c = _getch();

    if (c == -32)
    {
        c = _getch();
        return c;
    }
}

int main() {
    Human player(3, 3, 1);
    Monster enemy1(10, 10, 2);
    Food food1(30, 10, 1);
    while (1) { //--------------------------update
        player.move();
        food1.move();
        enemy1.move();
        printField();
        printObj(&player);
        printObj(&food1);
        printObj(&enemy1);
        cout << player.collide(&food1) << "  ";
        cout << player.collide(&enemy1);
    }

    return 0;
}
