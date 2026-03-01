#include <SDL.h>
#include <iostream>
#include <vector>
#include <memory>
#include <ctime>

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

int main(int argc, char* argv[]) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    int sw = 800, sh = 600;
    SDL_Window* window = SDL_CreateWindow("RCT Native - Full Integration", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sw, sh, SDL_WINDOW_SHOWN);
    
    rct::Renderer renderer(window, sw, sh);
    renderer.setPaletteColor(7, 50, 50, 50);   // UI Title
    renderer.setPaletteColor(8, 180, 180, 180); // UI Body
    renderer.setPaletteColor(200, 255, 100, 200); // Guest Pink
    renderer.setPaletteColor(40, 255, 255, 0);   // Train Yellow

    rct::World world(32, 32);
    rct::Camera cam;
    cam.x = 400; cam.y = 100;
    
    rct::Economy economy(100000); // $1,000.00
    rct::Scenario scenario("Forest Frontiers", rct::ScenarioGoal(30, 200000, 10000));
    
    rct::UIManager uiManager;
    auto* scenarioWin = new rct::ScenarioWindow(&scenario, 550, 350, 200, 100);
    uiManager.addWindow(new rct::ParkInfoWindow(&economy, 50, 50, 200, 150));
    uiManager.addWindow(scenarioWin);

    auto guestSprite = createGuestSprite();
    std::vector<std::unique_ptr<rct::Guest>> guests;

    // Create a simple track circuit
    rct::Ride rollerCoaster;
    for (int i = 5; i < 12; ++i) rollerCoaster.addSegment(rct::TrackSegment(i, 8, 2, rct::TrackType::STRAIGHT, rct::Direction::EAST));
    rollerCoaster.addSegment(rct::TrackSegment(12, 8, 2, rct::TrackType::SLOPE_UP, rct::Direction::EAST));
    rollerCoaster.addSegment(rct::TrackSegment(13, 8, 3, rct::TrackType::SLOPE_DOWN, rct::Direction::EAST));
    for (int i = 14; i >= 5; --i) rollerCoaster.addSegment(rct::TrackSegment(i, 9, 2, rct::TrackType::STRAIGHT, rct::Direction::WEST));
    
    rct::Train train(rollerCoaster.getSegments());

    bool quit = false;
    SDL_Event e;
    bool rightMouseDown = false;
    int prevMx = 0, prevMy = 0;
    int hX = -1, hY = -1;

    while (!quit) {
        int mx, my;
        uint32_t mouseState = SDL_GetMouseState(&mx, &my);
        bool leftMouseDown = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
        int deltaX = mx - prevMx; int deltaY = my - prevMy;
        prevMx = mx; prevMy = my;

        world.screenToGrid(mx, my, cam, hX, hY);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            
            // Camera Controls
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) rightMouseDown = true;
            if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT) rightMouseDown = false;
            
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:    cam.y += 20; break;
                    case SDLK_DOWN:  cam.y -= 20; break;
                    case SDLK_LEFT:  cam.x += 20; break;
                    case SDLK_RIGHT: cam.x -= 20; break;
                    case SDLK_r: cam.rotateClockwise(); break;
                    case SDLK_l: cam.rotateCounterClockwise(); break;
                    case SDLK_EQUALS: cam.adjustZoom(0.1f); break;
                    case SDLK_MINUS:  cam.adjustZoom(-0.1f); break;
                    case SDLK_s: world.save("savegame.dat"); break;
                    case SDLK_o: world.load("savegame.dat"); break;
                    case SDLK_ESCAPE: quit = true; break;
                }
            }

            // Terrain / Guest Spawning
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (hX != -1 && hY != -1) {
                    // Check if Shift is held for terrain height
                    const uint8_t* keys = SDL_GetKeyboardState(nullptr);
                    if (keys[SDL_SCANCODE_LSHIFT]) {
                        world.getTile(hX, hY).baseHeight++;
                    } else {
                        // Spawn Guest
                        guests.emplace_back(new rct::Guest(guests.size(), hX, hY, guestSprite));
                        economy.addIncome(1000); // $10 Entry fee
                    }
                }
            }
        }

        // --- Logic Update ---
        uiManager.handleInput(mx, my, leftMouseDown, deltaX, deltaY);
        if (rightMouseDown) { cam.x += deltaX; cam.y += deltaY; }

        for (auto& g : guests) g->update();
        train.update();

        scenario.checkVictory(guests.size(), economy.getBalance());
        scenarioWin->updateProgress(guests.size());

        // --- Render ---
        renderer.clear(0);
        world.render(renderer, cam, hX, hY);
        
        rollerCoaster.render(renderer, world, cam);
        train.render(renderer, world, cam);

        for (auto& g : guests) g->renderAt(renderer, world, cam);
        
        uiManager.render(renderer);
        renderer.present();
        
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
