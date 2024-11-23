#include <stdio.h> 
#include <iostream>
#include <SDL.h> 
#include <SDL_image.h> //jpg, png�� �б� ���� ����


int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;      //�̹����� ��ġ
    SDL_Surface* imageSurface = NULL;   //�̹������� �ҷ�����
    SDL_Texture* texture = NULL;        //��� �����ϰ� ��ȯ
    SDL_Rect destRect;                  //��ġ����

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow("SDL2 Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL Initialization Fail: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);   //renderer ����
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);          //�̹��� ���� ����

    imageSurface = IMG_Load("imgs/num_0.jpg");      //surface �ʱ�ȭ
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);//taxture�� ��ȯ
    destRect = { 100, 50, imageSurface->w, imageSurface->h };//��ġ����

    SDL_Event event;
    int quit = 0;
    while (!quit)//�޽��� ����
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN://����� �Է�
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

        //SDL_RenderClear(renderer);                       //������ ��� ����
        //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);//��� �� ����

        SDL_RenderCopy(renderer, texture, NULL, &destRect);//taxture ����
        SDL_RenderPresent(renderer);
    }

    // ����
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(imageSurface);

    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}