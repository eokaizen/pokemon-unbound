#include <SDL2/SDL.h>
#include <windows.h>
#include <string>
#include <fstream>

std::string romPath = "";
std::string configPath = "config.txt";

void SaveConfig(std::string path) {
    std::ofstream f(configPath);
    f << path;
    romPath = path;
}
std::string LoadConfig() {
    std::ifstream f(configPath);
    std::string p;
    if(f) std::getline(f, p);
    return p;
}
std::string OpenFileDialog() {
    char filename[MAX_PATH] = "";
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = "GBA ROM\0*.gba\0Todos\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "Selecione a ROM do Pokemon Unbound";
    if (GetOpenFileNameA(&ofn)) return std::string(filename);
    return "";
}
int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int show) {
    romPath = LoadConfig();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Pokemon Unbound Launcher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 300, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Rect btnSelect = {100, 80, 300, 45};
    SDL_Rect btnPlay = {100, 140, 300, 45};
    bool running = true;
    while(running) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) running = false;
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x, y = e.button.y;
                if(x > btnSelect.x && x < btnSelect.x+btnSelect.w && y > btnSelect.y && y < btnSelect.y+btnSelect.h) {
                    std::string p = OpenFileDialog();
                    if(!p.empty()) SaveConfig(p);
                }
                if(x > btnPlay.x && x < btnPlay.x+btnPlay.w && y > btnPlay.y && y < btnPlay.y+btnPlay.h) {
                    if(romPath.empty()) MessageBoxA(NULL, "Selecione uma ROM primeiro!", "Aviso", MB_ICONWARNING);
                    else MessageBoxA(NULL, romPath.c_str(), "ROM Carregada", MB_OK);
                }
            }
        }
        SDL_SetRenderDrawColor(ren, 18, 18, 18, 255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 60, 60, 60, 255);
        SDL_RenderFillRect(ren, &btnSelect);
        SDL_SetRenderDrawColor(ren, 0, 150, 70, 255);
        SDL_RenderFillRect(ren, &btnPlay);
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
    SDL_Quit();
    return 0;
}
