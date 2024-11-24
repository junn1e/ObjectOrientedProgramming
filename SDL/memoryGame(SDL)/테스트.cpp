#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <random>  // <random> �߰�

#define WIDTH 130*4
#define HEIGHT 130*4
#define MAX_STACK 20
#define MEMORY 4

// MemorySlot Ŭ���� ����
class MemorySlot {
public:
    SDL_Rect rect;
    int value;  // �ش� ������ �� (1~14, ����)
    bool isRevealed; // ������ ������ �������� ����
    SDL_Texture* texture;

    MemorySlot(SDL_Rect r, int val) : rect(r), value(val), isRevealed(false), texture(nullptr) {}

    // �ؽ�ó �ε� �Լ�
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

    // ������ ���� �׸���
    void render(SDL_Renderer* renderer);
};

void MemorySlot::render(SDL_Renderer* renderer) {
    if (isRevealed) {
        // isRevealed�� true�� �� ���� �̹��� ǥ��
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
    else {
        // isRevealed�� false�� �� character_0.png �̹����� ����
        if (texture == nullptr) {
            loadTexture(renderer, "imgs/character_0.png");
        }
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

// MemoryGame Ŭ���� ����
class MemoryGame {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<MemorySlot> slots;
    int memory;  // ���� memory �� (������ ���̵�)
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

        // ���� �ߺ� ���� �迭 �����
        std::vector<int> numbers;
        for (int i = 1; i <= 14; ++i) {
            numbers.push_back(i);
            numbers.push_back(i); // ���ڴ� �� ���� �ʿ�
        }

        // ���� ������ ���� std::shuffle ���
        // random_device�� mt19937 ���� ���
        std::random_device rd;
        std::mt19937 g(rd()); // Mersenne Twister ����

        std::shuffle(numbers.begin(), numbers.end(), g);

        // ������ ��ġ�� memory ������ŭ ���� ����
        for (int i = 0; i < memory; i++) {
            int x = rand() % (WIDTH - 130);
            int y = rand() % (HEIGHT - 130);
            SDL_Rect rect = { x, y, 130, 130 };
            slots.push_back(MemorySlot(rect, numbers[i]));
        }

        // �̹��� �ε� �� ���� ����
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
                    slot.isRevealed = false; // 3�� �� ��� character_0 �̹����� ����
                    slot.loadTexture(renderer, "imgs/character_0.png"); // character_0 �̹��� �ε�
                }
                gameActive = false;
            }

            // ȭ�� �׸���
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // ��� ������
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
                // Ŭ���� ������ �̹� ������ ���°� �ƴϸ� ����
                if (!slot.isRevealed) {
                    slot.isRevealed = true;  // Ŭ���� ������ ���� ���·� ����

                    // ������ ���Կ� �´� �̹����� �ε� (�̹� �ε�� �ؽ�ó�� ����)
                    std::stringstream ss;
                    ss << slot.value;
                    std::string imagePath = "imgs/character_" + ss.str() + ".png";

                    // ���Կ� �̹� �ε�� �ؽ�ó�� ���ٸ� �ε�
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
    MemoryGame game(MEMORY); // �ʱ� memory�� 4
    game.startGame();
    return 0;
}
