#ifndef RCT_SPRITE_HPP
#define RCT_SPRITE_HPP

#include <vector>
#include <cstdint>

namespace rct {

/**
 * @brief 8-bit indexed color sprite with transparency (index 0).
 */
struct Sprite {
    int width;
    int height;
    int offsetX; // Anchor X (relative to drawing position)
    int offsetY; // Anchor Y (relative to drawing position)
    
    std::vector<uint8_t> data;

    Sprite(int w, int h, int ox = 0, int oy = 0) 
        : width(w), height(h), offsetX(ox), offsetY(oy) {
        data.resize(w * h, 0);
    }
};

} // namespace rct

#endif // RCT_SPRITE_HPP
