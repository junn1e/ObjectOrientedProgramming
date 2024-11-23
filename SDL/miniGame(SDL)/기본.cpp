class Gameobject {
protected:
    int distance;   //�ѹ� �̵��Ÿ�
    int x, y;   //������ġ 
public:
    Gameobject(int startX, int startY, int distance) {  //������. �ʱ� ��ġ�� �̵� �Ÿ� ����
        this->x = startX;
        this->y = startY;
        this->distance = distance;
    }
    virtual ~Gameobject() {};   //�Ҹ���
    virtual void move() = 0;    //�̵� �� x,y �� �� ��ġ�� ����
    virtual char getShape() = 0;    //Human = H / Monster = M / Food = @

    //�浹 �Ǻ�
    int getX() { return x; }
    int getY() { return y; }
    bool collide(Gameobject* p) {
        if (this->x == p->getX() && this->y == p->getY()) return true;
        else return false;
    }
};