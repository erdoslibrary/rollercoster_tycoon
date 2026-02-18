#include "World.hpp"
#include <cmath>
#include <algorithm>

namespace rct {

World::World(int w, int h) : width(w), height(h) {
    tiles.resize(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            getTile(x, y).baseHeight = (x + y) / 4;
            getTile(x, y).type = (x % 2 == y % 2) ? 2 : 5;
        }
    }
}

Tile& World::getTile(int x, int y) {
    x = std::max(0, std::min(x, width - 1));
    y = std::max(0, std::min(y, height - 1));
    return tiles[y * width + x];
}

void World::gridToScreen(int tx, int ty, int th, int& sx, int& sy, const Camera& cam) const {
    int rx, ry; // Rotated coordinates
    switch (cam.rotation) {
        case 1: rx = ty; ry = (width - 1 - tx); break; // 90 CW
        case 2: rx = (width - 1 - tx); ry = (height - 1 - ty); break; // 180
        case 3: rx = (height - 1 - ty); ry = tx; break; // 270
        default: rx = tx; ry = ty; break; // 0
    }

    sx = (rx - ry) * (TILE_HALF * cam.zoom) + cam.x;
    sy = (rx + ry) * (TILE_HALF / 2 * cam.zoom) - (th * HEIGHT_UNIT * cam.zoom) + cam.y;
}

void World::render(Renderer& renderer, const Camera& cam) const {
    // Determine loop order based on rotation for correct depth sorting
    int xStart, xEnd, xStep;
    int yStart, yEnd, yStep;

    // We always render from "back to front" relative to the current view
    // In our isometric view, "Back" is top-center, "Front" is bottom-center.
    // This simple loop works for 0 rotation. For others, we adjust the indices.
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Map (x, y) to actual world grid coordinates (tx, ty) based on rotation
            int tx = x, ty = y;
            // (Sorting logic for complex rotation is simplified here for now)
            
            Tile& tile = const_cast<World*>(this)->getTile(tx, ty);
            int sx, sy;
            gridToScreen(tx, ty, tile.baseHeight, sx, sy, cam);
            
            // Render basic diamond (scaled by zoom)
            uint8_t color = tile.type;
            int h = (TILE_HALF / 2) * cam.zoom;
            int w = TILE_HALF * cam.zoom;

            for (int dy = -h/2; dy < h/2; ++dy) {
                int lineW = w * (1.0f - std::abs(dy) / (float)h * 2.0f);
                for (int dx = -lineW/2; dx < lineW/2; ++dx) {
                    renderer.putPixel(sx + dx, sy + dy, color);
                }
            }
        }
    }
}

} // namespace rct
