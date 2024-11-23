#include <stdio.h> 
#include <iostream>
#include <SDL.h> 

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;  //â�� ���� Ŭ����

    // SDL �ʱ�ȭ
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        return 1;
    }

    // ������ â ����
    window = SDL_CreateWindow("SDL2 Window",//â �̸�
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_SHOWN);

    if (!window) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event event;// �޽��� ���� (����ڷκ��� �Է°��� ����)
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // ����
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}