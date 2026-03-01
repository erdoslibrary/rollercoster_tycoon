#ifndef RCT_ENTITY_MANAGER_HPP
#define RCT_ENTITY_MANAGER_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include "Entity.hpp"

namespace rct {

/**
 * @brief Manages lifecycle of all game entities.
 */
class EntityManager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    Entity::Id nextId;

public:
    EntityManager() : nextId(1) {}

    /**
     * @brief Adds a new entity to the manager.
     * Usage: manager.add<Guest>(...)
     */
    template <typename T, typename... Args>
    T* add(Args&&... args) {
        T* entity = new T(nextId++, std::forward<Args>(args)...);
        entities.emplace_back(std::unique_ptr<Entity>(entity));
        return entity;
    }

    /**
     * @brief Updates all active entities and cleans up inactive ones.
     */
    void update() {
        // Update all entities
        for (auto& entity : entities) {
            if (entity->isActive()) {
                entity->update();
            }
        }

        // Cleanup inactive entities (dead/removed)
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                [](const std::unique_ptr<Entity>& e) { return !e->isActive(); }),
            entities.end()
        );
    }

    /**
     * @brief Renders all active entities.
     */
    void render() {
        for (auto& entity : entities) {
            if (entity->isActive()) {
                entity->render();
            }
        }
    }

    size_t getCount() const { return entities.size(); }
    
    void clear() {
        entities.clear();
        nextId = 1;
    }
};

} // namespace rct

#endif // RCT_ENTITY_MANAGER_HPP
