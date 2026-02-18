#ifndef RCT_WORLD_HPP
#define RCT_WORLD_HPP

#include <vector>
#include "Tile.hpp"
#include "render/Renderer.hpp"

#include "render/Camera.hpp"

namespace rct {

class World {
public:
    static const int TILE_SIZE = 32; 
    static const int TILE_HALF = TILE_SIZE / 2;
    static const int HEIGHT_UNIT = 8; 

private:
    int width;
    int height;
    std::vector<Tile> tiles;

public:
    World(int w, int h);

    Tile& getTile(int x, int y);
    
    // Grid to Screen conversion with rotation
    void gridToScreen(int tx, int ty, int th, int& sx, int& sy, const Camera& cam) const;

    void render(Renderer& renderer, const Camera& cam) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

} // namespace rct

#endif // RCT_WORLD_HPP
