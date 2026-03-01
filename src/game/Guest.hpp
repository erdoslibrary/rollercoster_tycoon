#ifndef RCT_GUEST_HPP
#define RCT_GUEST_HPP

#include "Entity.hpp"
#include "render/Sprite.hpp"
#include <memory>
#include <vector>

namespace rct {

enum class GuestState {
    IDLE,
    WANDERING,
    WALKING_TO_TARGET,
    RESTING
};

struct GuestStats {
    float hunger;   // 0.0 (full) to 1.0 (starving)
    float energy;   // 1.0 (full) to 0.0 (exhausted)
    float happiness; // 1.0 (happy) to 0.0 (angry)

    GuestStats() : hunger(0.0f), energy(1.0f), happiness(1.0f) {}
};

class Guest : public Entity {
private:
    GuestState state;
    GuestStats stats;
    int stateTimer; // Frames until next state rethink

    int targetTileX;
    int targetTileY;
    float moveProgress; 
    
    int currentTileX;
    int currentTileY;

    // Pathfinding data
    std::vector<std::pair<int, int>> path;
    int pathIndex;

    std::shared_ptr<Sprite> sprite;

public:
    Guest(Id id, int tx, int ty, std::shared_ptr<Sprite> s);

    void update() override;
    void render() override; 
    void renderAt(class Renderer& renderer, const class World& world, const class Camera& cam);
    
    void setDestination(int tx, int ty, const class World& world);
    
private:
    void rethinkAction(const class World& world);
    void updateStats();
};

} // namespace rct

#endif // RCT_GUEST_HPP
