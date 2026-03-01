#include "Renderer.hpp"
#include <cstring>
#include <iostream>

namespace rct {

Renderer::Renderer(SDL_Window* win, int w, int h) 
    : width(w), height(h), window(win), sdlRenderer(nullptr), screenTexture(nullptr) 
{
    // SDL Renderer (accelerated)
    sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdlRenderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    }

    // ARGB8888 texture to display our 8-bit buffer
    screenTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, 
                                      SDL_TEXTUREACCESS_STREAMING, width, height);

    // Initial buffer setup
    pixelBuffer.resize(width * height, 0);

    // Initialize palette with black
    std::memset(palette, 0, sizeof(palette));
    setupDefaultPalette();
}

Renderer::~Renderer() {
    if (screenTexture) SDL_DestroyTexture(screenTexture);
    if (sdlRenderer) SDL_DestroyRenderer(sdlRenderer);
}

void Renderer::setPaletteColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    palette[index].r = r;
    palette[index].g = g;
    palette[index].b = b;
    palette[index].a = 255;
}

void Renderer::setupDefaultPalette() {
    // Basic palette for testing (index 0: black, 1: red, 2: green, 3: blue, etc.)
    setPaletteColor(0, 0, 0, 0);       // Transparent / Black
    setPaletteColor(1, 255, 0, 0);     // Red
    setPaletteColor(2, 0, 255, 0);     // Green
    setPaletteColor(3, 0, 0, 255);     // Blue
    setPaletteColor(4, 255, 255, 255); // White
    
    // Gradient (for fun)
    for(int i = 5; i < 256; ++i) {
        setPaletteColor(i, i, i, i);   // Grayscale
    }
}

void Renderer::clear(uint8_t colorIndex) {
    std::memset(pixelBuffer.data(), colorIndex, pixelBuffer.size());
}

void Renderer::putPixel(int x, int y, uint8_t colorIndex) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixelBuffer[y * width + x] = colorIndex;
    }
}

void Renderer::drawSprite(const Sprite& sprite, int x, int y) {
    int startX = x - sprite.offsetX;
    int startY = y - sprite.offsetY;

    for (int sy = 0; sy < sprite.height; ++sy) {
        for (int sx = 0; sx < sprite.width; ++sx) {
            uint8_t pixel = sprite.data[sy * sprite.width + sx];
            if (pixel != 0) { // Index 0 is transparent
                putPixel(startX + sx, startY + sy, pixel);
            }
        }
    }
}

void Renderer::present() {
    updateTexture();
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, screenTexture, nullptr, nullptr);
    SDL_RenderPresent(sdlRenderer);
}

void Renderer::updateTexture() {
    void* pixels;
    int pitch;
    
    if (SDL_LockTexture(screenTexture, nullptr, &pixels, &pitch) == 0) {
        uint32_t* dest = static_cast<uint32_t*>(pixels);
        
        // Convert 8-bit indices to ARGB using palette
        for (int i = 0; i < width * height; ++i) {
            uint8_t index = pixelBuffer[i];
            SDL_Color c = palette[index];
            dest[i] = (255 << 24) | (c.r << 16) | (c.g << 8) | c.b;
        }
        
        SDL_UnlockTexture(screenTexture);
    }
}

} // namespace rct
