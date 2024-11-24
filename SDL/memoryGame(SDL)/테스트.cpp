#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <random>  // <random> 추가

#define WIDTH 130*4
#define HEIGHT 130*4
#define MAX_STACK 20
#define MEMORY 4

// MemorySlot 클래스 정의
class MemorySlot {
public:
    SDL_Rect rect;
    int value;  // 해당 슬롯의 값 (1~14, 숫자)
    bool isRevealed; // 슬롯이 공개된 상태인지 여부
    SDL_Texture* texture;

    MemorySlot(SDL_Rect r, int val) : rect(r), value(val), isRevealed(false), texture(nullptr) {}

    // 텍스처 로드 함수
    bool loadTexture(SDL_Renderer* renderer, const std::string& imagePath) {
        SDL_Surface* surface = IMG_Load(imagePath.c_str());
        if (!surface) {
            std::cerr << "Unable to load image: " << IMG_GetError() << std::endl;
            return false;
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        return texture != nullptr;
    }

    // 공개된 슬롯 그리기
    void render(SDL_Renderer* renderer);
};

void MemorySlot::render(SDL_Renderer* renderer) {
    if (isRevealed) {
        // isRevealed가 true일 때 원래 이미지 표시
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
    else {
        // isRevealed가 false일 때 character_0.png 이미지로 변경
        if (texture == nullptr) {
            loadTexture(renderer, "imgs/character_0.png");
        }
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

// MemoryGame 클래스 정의
class MemoryGame {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<MemorySlot> slots;
    int memory;  // 현재 memory 수 (게임의 난이도)
    bool gameActive;
    Uint32 startTime;

public:
    MemoryGame(int initialMemory) : memory(initialMemory), gameActive(true), startTime(0) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL Initialization Fail: " << SDL_GetError() << std::endl;
            exit(1);
        }
        window = SDL_CreateWindow("SDL Memory Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "SDL Window Creation Fail: " << SDL_GetError() << std::endl;
            SDL_Quit();
            exit(1);
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer) {
            std::cerr << "SDL Renderer Creation Fail: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(1);
        }

        IMG_Init(IMG_INIT_PNG);
    }

    ~MemoryGame() {
        for (auto& slot : slots) {
            if (slot.texture) {
                SDL_DestroyTexture(slot.texture);
            }
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
    }

    void startGame() {
        createSlots();
        mainLoop();
    }

private:
    void createSlots() {
        srand(time(NULL));
        slots.clear();

        // 숫자 중복 방지 배열 만들기
        std::vector<int> numbers;
        for (int i = 1; i <= 14; ++i) {
            numbers.push_back(i);
            numbers.push_back(i); // 숫자는 두 번씩 필요
        }

        // 랜덤 셔플을 위해 std::shuffle 사용
        // random_device와 mt19937 엔진 사용
        std::random_device rd;
        std::mt19937 g(rd()); // Mersenne Twister 엔진

        std::shuffle(numbers.begin(), numbers.end(), g);

        // 랜덤한 위치에 memory 개수만큼 슬롯 생성
        for (int i = 0; i < memory; i++) {
            int x = rand() % (WIDTH - 130);
            int y = rand() % (HEIGHT - 130);
            SDL_Rect rect = { x, y, 130, 130 };
            slots.push_back(MemorySlot(rect, numbers[i]));
        }

        // 이미지 로드 및 랜덤 배정
        for (auto& slot : slots) {
            std::stringstream ss;
            ss << slot.value;
            std::string imagePath = "imgs/character_" + ss.str() + ".png";

            if (!slot.loadTexture(renderer, imagePath)) {
                std::cerr << "Failed to load texture: " << imagePath << std::endl;
            }
        }

        startTime = SDL_GetTicks();
    }

    void mainLoop() {
        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }

                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    handleClick(e.button.x, e.button.y);
                }
            }

            if (SDL_GetTicks() - startTime > 3000 && gameActive) {
                for (auto& slot : slots) {
                    slot.isRevealed = false; // 3초 후 모두 character_0 이미지로 변경
                    slot.loadTexture(renderer, "imgs/character_0.png"); // character_0 이미지 로드
                }
                gameActive = false;
            }

            // 화면 그리기
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // 배경 검정색
            SDL_RenderClear(renderer);

            for (auto& slot : slots) {
                slot.render(renderer);
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(16);  // 60fps
        }
    }

    void handleClick(int x, int y) {
        for (auto& slot : slots) {
            if (x >= slot.rect.x && x <= slot.rect.x + slot.rect.w &&
                y >= slot.rect.y && y <= slot.rect.y + slot.rect.h) {
                // 클릭한 슬롯이 이미 공개된 상태가 아니면 공개
                if (!slot.isRevealed) {
                    slot.isRevealed = true;  // 클릭된 슬롯을 공개 상태로 설정

                    // 공개된 슬롯에 맞는 이미지를 로드 (이미 로드된 텍스처는 재사용)
                    std::stringstream ss;
                    ss << slot.value;
                    std::string imagePath = "imgs/character_" + ss.str() + ".png";

                    // 슬롯에 이미 로드된 텍스처가 없다면 로드
                    if (!slot.texture) {
                        if (!slot.loadTexture(renderer, imagePath)) {
                            std::cerr << "Failed to load texture: " << imagePath << std::endl;
                        }
                    }
                }
                break;
            }
        }
    }

};

int main(int argc, char* argv[]) {
    MemoryGame game(MEMORY); // 초기 memory는 4
    game.startGame();
    return 0;
}
