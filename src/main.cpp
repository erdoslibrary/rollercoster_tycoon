#include <SDL.h>
#include <iostream>
#include <vector>
#include <memory>
#include <ctime>
#include "render/Renderer.hpp"
#include "render/UI.hpp"
#include "game/World.hpp"
#include "game/Guest.hpp"
#include "game/Economy.hpp"
#include "game/Scenario.hpp"
#include "render/Camera.hpp"
#include "game/Track.hpp"
#include "game/Train.hpp"
#include "common/SoundManager.hpp"

int main(int argc, char* argv[]) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    int sw = 800, sh = 600;
    SDL_Window* window = SDL_CreateWindow("RCT Native - Scenario Mode", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sw, sh, SDL_WINDOW_SHOWN);
    
    rct::Renderer renderer(window, sw, sh);
    renderer.setPaletteColor(7, 50, 50, 50);   
    renderer.setPaletteColor(8, 180, 180, 180); 
    renderer.setPaletteColor(1, 200, 0, 0); // Red
    renderer.setPaletteColor(2, 0, 200, 0); // Green

    rct::World world(32, 32);
    rct::Camera cam; cam.x = 400; cam.y = 100;
    rct::Economy economy(10000); // Start with $100
    
    // --- Scenario Setup ---
    rct::Scenario scenario("Forest Frontiers", rct::ScenarioGoal(30, 500000, 10000)); // Target: 30 Guests, $5000
    
    rct::UIManager uiManager;
    auto* scenarioWin = new rct::ScenarioWindow(&scenario, 550, 400, 200, 100);
    uiManager.addWindow(new rct::ParkInfoWindow(&economy, 50, 50, 200, 150));
    uiManager.addWindow(scenarioWin);

    std::vector<std::unique_ptr<rct::Guest>> guests;
    bool quit = false;
    SDL_Event e;
    int hX = -1, hY = -1;
    int prevMx = 0, prevMy = 0;

    while (!quit) {
        int mx, my;
        uint32_t mouseState = SDL_GetMouseState(&mx, &my);
        bool leftMouseDown = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
        int deltaX = mx - prevMx; int deltaY = my - prevMy;
        prevMx = mx; prevMy = my;

        world.screenToGrid(mx, my, cam, hX, hY);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                // Spawn a guest on click for testing (costs $50, earns entry fee later)
                if (hX != -1 && hY != -1) {
                    guests.emplace_back(new rct::Guest(guests.size(), hX, hY, nullptr));
                    economy.addIncome(1000); // Entry fee $10
                }
            }
        }

        // --- Victory Check ---
        if (scenario.checkVictory(guests.size(), economy.getBalance())) {
            SDL_SetWindowTitle(window, "CONGRATULATIONS! YOU WON!");
        }
        scenarioWin->updateProgress(guests.size());

        uiManager.handleInput(mx, my, leftMouseDown, deltaX, deltaY);

        renderer.clear(0);
        world.render(renderer, cam, hX, hY);
        uiManager.render(renderer);
        renderer.present();
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
