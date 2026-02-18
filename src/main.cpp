#include <SDL.h>
#include <iostream>
#include <vector>
#include <memory>
#include <ctime>
#include "render/Renderer.hpp"
#include "game/World.hpp"
#include "game/Guest.hpp"
#include "render/Camera.hpp"
#include "game/Track.hpp"
#include "game/Train.hpp"

int main(int argc, char* argv[]) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    int sw = 800, sh = 600;
    SDL_Window* window = SDL_CreateWindow("RCT Native Phase 4.3 - Ratings", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sw, sh, SDL_WINDOW_SHOWN);
    
    rct::Renderer renderer(window, sw, sh);
    renderer.setPaletteColor(40, 255, 255, 0); // Train Yellow
    
    rct::World world(32, 32);
    rct::Camera cam;
    cam.x = 400; cam.y = 100;

    rct::Ride rollerCoaster;
    // Circuit with Hill and Curve
    for (int i = 5; i < 10; ++i) rollerCoaster.addSegment(rct::TrackSegment(i, 5, 2, rct::TrackType::STRAIGHT, rct::Direction::EAST));
    rollerCoaster.addSegment(rct::TrackSegment(10, 5, 2, rct::TrackType::SLOPE_UP, rct::Direction::EAST));
    rollerCoaster.addSegment(rct::TrackSegment(11, 5, 3, rct::TrackType::SLOPE_UP, rct::Direction::EAST));
    rollerCoaster.addSegment(rct::TrackSegment(12, 5, 4, rct::TrackType::STRAIGHT, rct::Direction::EAST));
    rollerCoaster.addSegment(rct::TrackSegment(13, 5, 4, rct::TrackType::SLOPE_DOWN, rct::Direction::EAST));
    rollerCoaster.addSegment(rct::TrackSegment(14, 5, 3, rct::TrackType::SLOPE_DOWN, rct::Direction::EAST));
    rollerCoaster.addSegment(rct::TrackSegment(15, 5, 2, rct::TrackType::CURVE_RIGHT, rct::Direction::EAST));
    for (int i = 15; i >= 5; --i) rollerCoaster.addSegment(rct::TrackSegment(i, 6, 2, rct::TrackType::STRAIGHT, rct::Direction::WEST));

    rct::Train train(rollerCoaster.getSegments());

    bool quit = false;
    SDL_Event e;
    int tickCounter = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
        }

        train.update();
        tickCounter++;

        // Every 300 ticks, calculate and show ratings based on G-forces
        if (tickCounter % 300 == 0) {
            rollerCoaster.calculateFinalRatings(train.getGForceData());
            const auto& r = rollerCoaster.getRatings();
            std::cout << "--- Ride Status ---" << std::endl;
            std::cout << "Excitement: " << r.excitement << " (Medium)" << std::endl;
            std::cout << "Intensity:  " << r.intensity  << " (High)" << std::endl;
            std::cout << "Nausea:     " << r.nausea     << " (Low)" << std::endl;
            std::cout << "Max Vert G: " << train.getGForceData().verticalMax << std::endl;
        }

        renderer.clear(0);
        world.render(renderer, cam);
        rollerCoaster.render(renderer, world, cam);
        train.render(renderer, world, cam);
        renderer.present();
        SDL_Delay(16);
    }

    SDL_Quit();
    return 0;
}
