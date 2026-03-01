#ifndef RCT_RENDERER_HPP
#define RCT_RENDERER_HPP

#include <SDL.h>
#include <vector>
#include <cstdint>

#include "Sprite.hpp"

namespace rct {

/**
 * @brief Software renderer for 8-bit indexed pixel manipulation.
 */
class Renderer {
private:
    int width;
    int height;
    
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* screenTexture;
    
    // 8-bit pixel buffer (indices to palette)
    std::vector<uint8_t> pixelBuffer;
    
    // RGB Palette (256 colors)
    SDL_Color palette[256];

public:
    Renderer(SDL_Window* window, int w, int h);
    ~Renderer();

    // Palette management
    void setPaletteColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
    void setupDefaultPalette();

    // Drawing primitives
    void clear(uint8_t colorIndex);
    void putPixel(int x, int y, uint8_t colorIndex);
    void drawSprite(const Sprite& sprite, int x, int y);
    
    // Core pipeline
    void present();

    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    void updateTexture();
};

} // namespace rct

#endif // RCT_RENDERER_HPP
