#include "World.hpp"
#include <cmath>
#include <algorithm>
#include <fstream>

namespace rct {

World::World(int w, int h) : width(w), height(h) {
    tiles.resize(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            getTile(x, y).baseHeight = 2; // Flat ground at start
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
    int rx = tx, ry = ty;
    if (cam.rotation == 1) { rx = ty; ry = (width-1-tx); }
    else if (cam.rotation == 2) { rx = (width-1-tx); ry = (height-1-ty); }
    else if (cam.rotation == 3) { rx = (height-1-ty); ry = tx; }

    sx = (rx - ry) * (TILE_HALF * cam.zoom) + cam.x;
    sy = (rx + ry) * (TILE_HALF / 2 * cam.zoom) - (th * HEIGHT_UNIT * cam.zoom) + cam.y;
}

bool World::screenToGrid(int sx, int sy, const Camera& cam, int& tx, int& ty) const {
    float dx = (sx - cam.x) / (TILE_HALF * cam.zoom);
    float dy = (sy - cam.y) / (TILE_HALF / 2 * cam.zoom);

    int rx = static_cast<int>(std::floor((dy + dx) / 2.0f));
    int ry = static_cast<int>(std::floor((dy - dx) / 2.0f));

    if (cam.rotation == 0) { tx = rx; ty = ry; }
    else if (cam.rotation == 1) { tx = (width-1-ry); ty = rx; }
    else if (cam.rotation == 2) { tx = (width-1-rx); ty = (height-1-ry); }
    else if (cam.rotation == 3) { tx = ry; ty = (height-1-rx); }

    return (tx >= 0 && tx < width && ty >= 0 && ty < height);
}

bool World::save(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;

    const char magic[4] = {'R', 'C', 'T', 'N'};
    uint32_t version = 1;
    out.write(magic, 4);
    out.write(reinterpret_cast<const char*>(&version), 4);
    out.write(reinterpret_cast<const char*>(&width), 4);
    out.write(reinterpret_cast<const char*>(&height), 4);
    out.write(reinterpret_cast<const char*>(tiles.data()), tiles.size() * sizeof(Tile));
    return true;
}

bool World::load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;

    char magic[4];
    uint32_t version;
    in.read(magic, 4);
    in.read(reinterpret_cast<char*>(&version), 4);
    if (magic[0] != 'R' || magic[1] != 'C' || magic[2] != 'T' || magic[3] != 'N') return false;

    int newW, newH;
    in.read(reinterpret_cast<char*>(&newW), 4);
    in.read(reinterpret_cast<char*>(&newH), 4);
    width = newW; height = newH;
    tiles.resize(width * height);
    in.read(reinterpret_cast<char*>(tiles.data()), tiles.size() * sizeof(Tile));
    return true;
}

void World::render(Renderer& renderer, const Camera& cam, int hoverX, int hoverY) const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Tile& tile = const_cast<World*>(this)->getTile(x, y);
            int sx, sy;
            gridToScreen(x, y, tile.baseHeight, sx, sy, cam);
            uint8_t color = tile.type;
            if (x == hoverX && y == hoverY) color = 4;
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
