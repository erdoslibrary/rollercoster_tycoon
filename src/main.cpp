#include <SDL.h>
#include <iostream>
#include <vector>
#include <memory>
#include <ctime>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "render/Renderer.hpp"
#include "render/UI.hpp"
#include "render/Sprite.hpp"
#include "render/Camera.hpp"
#include "game/World.hpp"
#include "game/Guest.hpp"
#include "game/Economy.hpp"
#include "game/Scenario.hpp"
#include "game/Track.hpp"
#include "game/Train.hpp"
#include "common/SoundManager.hpp"

// AppContext to hold all game state for the main loop
struct AppContext {
    bool quit;
    SDL_Window* window;
    rct::Renderer* renderer;
    rct::World* world;
    rct::Camera* cam;
    rct::Economy* economy;
    rct::Scenario* scenario;
    rct::UIManager* uiManager;
    rct::ScenarioWindow* scenarioWin;
    rct::Train* train;
    rct::Ride* rollerCoaster;
    std::shared_ptr<rct::Sprite> guestSprite;
    std::vector<std::unique_ptr<rct::Guest>> guests;
    bool rightMouseDown;
    int prevMx, prevMy;
};

// Helper to create a guest sprite
std::shared_ptr<rct::Sprite> createGuestSprite() {
    auto s = std::make_shared<rct::Sprite>(8, 16, 4, 14);
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (y < 4) s->data[y * 8 + x] = 4; // Head
            else s->data[y * 8 + x] = 200;    // Pink Body
        }
    }
    return s;
}

// Main Loop Function (Called every frame)
void main_loop(void* arg) {
    AppContext* ctx = static_cast<AppContext*>(arg);
    SDL_Event e;
    int mx, my;
    uint32_t mouseState = SDL_GetMouseState(&mx, &my);
    bool leftMouseDown = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
    
    // Calculate mouse delta CORRECTLY
    int deltaX = mx - ctx->prevMx; 
    int deltaY = my - ctx->prevMy;
    ctx->prevMx = mx; 
    ctx->prevMy = my;

    // Get Hovered Tile (using separate local variables to avoid overwriting prevMx/prevMy)
    int hX, hY;
    ctx->world->screenToGrid(mx, my, *ctx->cam, hX, hY);

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) ctx->quit = true;
        
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) ctx->rightMouseDown = true;
        if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT) ctx->rightMouseDown = false;
        
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:    ctx->cam->y += 20; break;
                case SDLK_DOWN:  ctx->cam->y -= 20; break;
                case SDLK_LEFT:  ctx->cam->x += 20; break;
                case SDLK_RIGHT: ctx->cam->x -= 20; break;
                case SDLK_r: ctx->cam->rotateClockwise(); break;
                case SDLK_l: ctx->cam->rotateCounterClockwise(); break;
                case SDLK_EQUALS: ctx->cam->adjustZoom(0.1f); break;
                case SDLK_MINUS:  ctx->cam->adjustZoom(-0.1f); break;
                case SDLK_s: ctx->world->save("savegame.dat"); break;
                case SDLK_o: ctx->world->load("savegame.dat"); break;
                case SDLK_ESCAPE: ctx->quit = true; break;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            // Check if UI handled the click first (Simplified: check if mouse is not over a window)
            // (In a real UI system, handleInput would return a bool to block this)
            if (hX != -1 && hY != -1) {
                const uint8_t* keys = SDL_GetKeyboardState(nullptr);
                if (keys[SDL_SCANCODE_LSHIFT]) {
                    ctx->world->getTile(hX, hY).baseHeight++;
                } else {
                    ctx->guests.emplace_back(new rct::Guest(ctx->guests.size(), hX, hY, ctx->guestSprite));
                    ctx->economy->addIncome(1000);
                }
            }
        }
    }

    // Logic Update
    ctx->uiManager->handleInput(mx, my, leftMouseDown, deltaX, deltaY);
    
    // Camera Drag Logic (Only if not clicking on UI - simplified)
    if (ctx->rightMouseDown) { 
        ctx->cam->x += deltaX; 
        ctx->cam->y += deltaY; 
    }
    
    for (auto& g : ctx->guests) g->update();
    ctx->train->update();
    ctx->scenario->checkVictory(ctx->guests.size(), ctx->economy->getBalance());
    ctx->scenarioWin->updateProgress(ctx->guests.size());

    // Render
    ctx->renderer->clear(0);
    ctx->world->render(*ctx->renderer, *ctx->cam, hX, hY);
    ctx->rollerCoaster->render(*ctx->renderer, *ctx->world, *ctx->cam);
    ctx->train->render(*ctx->renderer, *ctx->world, *ctx->cam);
    for (auto& g : ctx->guests) g->renderAt(*ctx->renderer, *ctx->world, *ctx->cam);
    ctx->uiManager->render(*ctx->renderer);
    ctx->renderer->present();
}

int main(int argc, char* argv[]) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    AppContext ctx;
    ctx.quit = false;
    int sw = 800, sh = 600;
    ctx.window = SDL_CreateWindow("RCT Native - Full Roadmap Integration", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sw, sh, SDL_WINDOW_SHOWN);
    
    ctx.renderer = new rct::Renderer(ctx.window, sw, sh);
    ctx.renderer->setPaletteColor(7, 50, 50, 50);   
    ctx.renderer->setPaletteColor(8, 180, 180, 180); 
    ctx.renderer->setPaletteColor(200, 255, 100, 200); 
    ctx.renderer->setPaletteColor(40, 255, 255, 0);   
    ctx.renderer->setPaletteColor(1, 200, 0, 0); 
    ctx.renderer->setPaletteColor(2, 0, 200, 0); 

    ctx.world = new rct::World(32, 32);
    ctx.cam = new rct::Camera();
    ctx.cam->x = 400; ctx.cam->y = 100;
    ctx.economy = new rct::Economy(100000);
    ctx.scenario = new rct::Scenario("Forest Frontiers", rct::ScenarioGoal(30, 200000, 10000));
    
    ctx.uiManager = new rct::UIManager();
    ctx.scenarioWin = new rct::ScenarioWindow(ctx.scenario, 550, 350, 200, 100);
    ctx.uiManager->addWindow(new rct::ParkInfoWindow(ctx.economy, 50, 50, 200, 150));
    ctx.uiManager->addWindow(ctx.scenarioWin);

    ctx.guestSprite = createGuestSprite();
    ctx.rollerCoaster = new rct::Ride();
    // Create track circuit
    for (int i = 5; i < 12; ++i) ctx.rollerCoaster->addSegment(rct::TrackSegment(i, 8, 2, rct::TrackType::STRAIGHT, rct::Direction::EAST));
    ctx.rollerCoaster->addSegment(rct::TrackSegment(12, 8, 2, rct::TrackType::SLOPE_UP, rct::Direction::EAST));
    ctx.rollerCoaster->addSegment(rct::TrackSegment(13, 8, 3, rct::TrackType::SLOPE_DOWN, rct::Direction::EAST));
    for (int i = 14; i >= 5; --i) ctx.rollerCoaster->addSegment(rct::TrackSegment(i, 9, 2, rct::TrackType::STRAIGHT, rct::Direction::WEST));
    
    ctx.train = new rct::Train(ctx.rollerCoaster->getSegments());
    ctx.rightMouseDown = false;
    
    // Initial mouse state
    SDL_GetMouseState(&ctx.prevMx, &ctx.prevMy);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop, &ctx, 0, 1);
#else
    while (!ctx.quit) {
        main_loop(&ctx);
        SDL_Delay(16);
    }
#endif

    delete ctx.train;
    delete ctx.rollerCoaster;
    delete ctx.uiManager;
    delete ctx.economy;
    delete ctx.scenario;
    delete ctx.cam;
    delete ctx.world;
    delete ctx.renderer;
    SDL_DestroyWindow(ctx.window);
    SDL_Quit();
    return 0;
}
