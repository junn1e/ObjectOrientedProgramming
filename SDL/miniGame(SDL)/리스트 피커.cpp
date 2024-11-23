#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

class listPicker {
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
    listPicker(int moveRate, int base) {
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

    ~listPicker() {
        free(this->arr);
    }

    int pick() {
        if (this->cursor >= base) {
            shuffle();
            this->cursor = 0;
        }
        return this->arr[cursor++];
    }
};

int main(void)
{
    listPicker picker(2, 5);

    for (int i = 0; i < 50; i++) {
        printf("%d ", picker.pick());
    }
    return 0;
}