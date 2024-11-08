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