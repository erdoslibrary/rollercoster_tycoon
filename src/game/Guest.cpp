#include "Guest.hpp"
#include "World.hpp"
#include "render/Renderer.hpp"
#include "Pathfinder.hpp"
#include <cstdlib>
#include <iostream>

namespace rct {

Guest::Guest(Id id, int tx, int ty, std::shared_ptr<Sprite> s) 
    : Entity(id), state(GuestState::IDLE), stateTimer(0),
      currentTileX(tx), currentTileY(ty), 
      targetTileX(tx), targetTileY(ty), moveProgress(1.0f), 
      pathIndex(0), sprite(s) 
{
}

void Guest::setDestination(int tx, int ty, const World& world) {
    path = Pathfinder::findPath(currentTileX, currentTileY, tx, ty, world.getWidth(), world.getHeight());
    if (!path.empty()) {
        pathIndex = 1;
        state = GuestState::WALKING_TO_TARGET;
    }
}

void Guest::updateStats() {
    // Gradually increase hunger and decrease energy
    stats.hunger += 0.0001f;
    if (state != GuestState::RESTING) {
        stats.energy -= 0.0002f;
    } else {
        stats.energy += 0.001f; // Recover while resting
        if (stats.energy >= 1.0f) {
            stats.energy = 1.0f;
            state = GuestState::IDLE;
        }
    }

    // Caps
    if (stats.hunger > 1.0f) stats.hunger = 1.0f;
    if (stats.energy < 0.0f) stats.energy = 0.0f;
}

void Guest::rethinkAction(const World& world) {
    if (stats.energy < 0.2f) {
        state = GuestState::RESTING;
        stateTimer = 120; // Rest for 120 ticks
        return;
    }

    if (std::rand() % 2 == 0) {
        // Decide to wander to a random location
        setDestination(std::rand() % world.getWidth(), std::rand() % world.getHeight(), world);
    } else {
        state = GuestState::IDLE;
        stateTimer = 60 + (std::rand() % 60);
    }
}

void Guest::update() {
    updateStats();

    if (state == GuestState::RESTING || state == GuestState::IDLE) {
        if (stateTimer > 0) {
            stateTimer--;
        } else {
            // Time to do something else
            // Note: In real app, we need a world reference here. 
            // For now, we'll wait for the next rethink trigger in main loop or logic.
        }
    }

    if (state == GuestState::WALKING_TO_TARGET) {
        if (moveProgress < 1.0f) {
            moveProgress += 0.05f;
            if (moveProgress >= 1.0f) {
                moveProgress = 1.0f;
                currentTileX = targetTileX;
                currentTileY = targetTileY;
            }
        } else {
            if (pathIndex < (int)path.size()) {
                targetTileX = path[pathIndex].first;
                targetTileY = path[pathIndex].second;
                moveProgress = 0.0f;
                pathIndex++;
            } else {
                state = GuestState::IDLE;
                stateTimer = 60;
            }
        }
    }
}

void Guest::renderAt(Renderer& renderer, const World& world, const Camera& cam) {
    float fx = currentTileX + (targetTileX - currentTileX) * moveProgress;
    float fy = currentTileY + (targetTileY - currentTileY) * moveProgress;
    
    int sx, sy;
    world.gridToScreen(static_cast<int>(fx), static_cast<int>(fy), 0, sx, sy, cam);
    
    // Simple visual feedback for states
    if (sprite) {
        renderer.drawSprite(*sprite, sx, sy);
        
        // If resting, draw a small indicator (just a pixel for now)
        if (state == GuestState::RESTING) {
            renderer.putPixel(sx, sy - 18, 3); // Blue dot above head
        }
        if (stats.hunger > 0.8f) {
            renderer.putPixel(sx + 2, sy - 18, 1); // Red dot if starving
        }
    }
}

void Guest::render() {}

} // namespace rct
