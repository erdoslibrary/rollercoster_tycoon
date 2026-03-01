#ifndef RCT_ENTITY_HPP
#define RCT_ENTITY_HPP

#include <cstdint>
#include "common/Vector2.hpp"

namespace rct {

/**
 * @brief Base class for all game entities (Guests, Rides, Props, etc.)
 */
class Entity {
public:
    using Id = uint32_t;

protected:
    Id id;
    Vector2 position;
    bool active;

public:
    Entity(Id id) : id(id), position(), active(true) {}
    virtual ~Entity() = default;

    // Core lifecycle
    virtual void update() = 0;
    virtual void render() = 0;

    // Getters / Setters
    Id getId() const { return id; }
    Vector2 getPosition() const { return position; }
    void setPosition(const Vector2& pos) { position = pos; }
    
    bool isActive() const { return active; }
    void deactivate() { active = false; }

    // Disable copy for safety (entities are managed by pointers)
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
};

} // namespace rct

#endif // RCT_ENTITY_HPP
