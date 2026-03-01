#ifndef RCT_TILE_HPP
#define RCT_TILE_HPP

#include <cstdint>

namespace rct {

/**
 * @brief Represents a single square tile in the world grid.
 */
struct Tile {
    uint8_t baseHeight; // Base elevation of the tile (0-255)
    uint8_t type;       // Surface type (0: Grass, 1: Dirt, 2: Water, etc.)
    
    // Potential for slope data:
    // uint8_t cornerHeights[4]; // NW, NE, SE, SW corner offsets

    Tile() : baseHeight(0), type(0) {}
};

} // namespace rct

#endif // RCT_TILE_HPP
