#include <stdio.h> 
#include <iostream>
#include <SDL.h> 
#include <SDL_image.h> //jpg, png를 읽기 위해 참조


int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;      //이미지의 위치
    SDL_Surface* imageSurface = NULL;   //이미지파일 불러오기
    SDL_Texture* texture = NULL;        //사용 가능하게 변환
    SDL_Rect destRect;                  //위치지정

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

    renderer = SDL_CreateRenderer(window, -1, 0);   //renderer 선언
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);          //이미지 범위 선언

    imageSurface = IMG_Load("imgs/num_0.jpg");      //surface 초기화
    texture = SDL_CreateTextureFromSurface(renderer, imageSurface);//taxture로 변환
    destRect = { 100, 50, imageSurface->w, imageSurface->h };//위치지정

    SDL_Event event;
    int quit = 0;
    while (!quit)//메시지 루프
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN://사용자 입력
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

        //SDL_RenderClear(renderer);                       //윈도우 배경 제거
        //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);//배경 색 지정

        SDL_RenderCopy(renderer, texture, NULL, &destRect);//taxture 적용
        SDL_RenderPresent(renderer);
    }

    // 종료
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(imageSurface);

    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}