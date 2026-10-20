#include <SDL2/SDL.h>
#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#define JSON_NOEXCEPT 1
#include <mgba/core/core.h>
#include <mgba/gba/core.h>

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

bool IsValidGBA(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if(!f) return false;
    f.seekg(0xB2);
    char logo = 0;
    f.read(&logo, 1);
    // Checagem simples de header Nintendo
    return f.gcount() == 1 && logo == 0x96;
}

void RunGame(const std::string& path) {
    // Inicializa o core do mGBA EMBUTIDO
    struct mCore* core = GBACoreCreate();
    if (!core->init(core)) return;
    
    struct mCoreLoadState state = mCoreLoadState_NonPersistent;
    if (!core->loadROM(core, path.c_str())) {
        MessageBoxA(NULL, "Falha ao carregar ROM! ROM corrompida?", "Erro", MB_ICONERROR);
        return;
    }
    core->loadSave(core, (path + ".sav").c_str());
    
    SDL_Window* win = SDL_CreateWindow("Pokemon Unbound", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 240, 160);
    
    core->reset(core);
    
    SDL_Event e;
    bool running = true;
    while(running) {
        while(SDL_PollEvent(&e)) if(e.type == SDL_QUIT) running = false;
        
        core->runFrame(core);
        
        // Pega o framebuffer do GBA e joga na tela SDL
        const uint16_t* fb = (uint16_t*)core->getPixels(core);
        SDL_UpdateTexture(tex, NULL, fb, 240 * 2);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
    
    core->saveSave(core, (path + ".sav").c_str());
    core->deinit(core);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int show) {
    romPath = LoadConfig();
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);

    // --- TELA DO LAUNCHER ---
    SDL_Window* launcherWin = SDL_CreateWindow("Pokemon Unbound Launcher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 300, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(launcherWin, -1, 0);

    bool launcherRunning = true;
    SDL_Rect btnSelect = {100, 100, 300, 45};
    SDL_Rect btnPlay = {100, 160, 300, 45};
    
    while(launcherRunning) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) return 0;
            if(e.type == SDL_DROPFILE) {
                std::string dropped = e.drop.file;
                if(IsValidGBA(dropped)) {
                    SaveConfig(dropped);
                } else {
                    MessageBoxA(NULL, "Arquivo não é uma ROM GBA válida!", "Erro", MB_ICONERROR);
                }
                SDL_free(e.drop.file);
            }
            if(e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x, y = e.button.y;
                if(x > btnSelect.x && x < btnSelect.x+btnSelect.w && y > btnSelect.y && y < btnSelect.y+btnSelect.h) {
                    std::string p = OpenFileDialog();
                    if(!p.empty()) {
                        if(IsValidGBA(p)) SaveConfig(p);
                        else MessageBoxA(NULL, "ROM inválida!", "Erro", MB_ICONERROR);
                    }
                }
                if(x > btnPlay.x && x < btnPlay.x+btnPlay.w && y > btnPlay.y && y < btnPlay.y+btnPlay.h) {
                    if(romPath.empty() || !IsValidGBA(romPath)) {
                        MessageBoxA(NULL, "Selecione uma ROM válida primeiro!", "Aviso", MB_ICONWARNING);
                    } else {
                        SDL_DestroyWindow(launcherWin);
                        RunGame(romPath);
                        return 0;
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
        SDL_RenderClear(ren);
        // Desenha botões (simplificado)
        SDL_SetRenderDrawColor(ren, 60, 60, 60, 255);
        SDL_RenderFillRect(ren, &btnSelect);
        SDL_RenderFillRect(ren, &btnPlay);
        SDL_RenderPresent(ren);
    }
    return 0;
}
