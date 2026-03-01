#include "Track.hpp"
#include "World.hpp"
#include "render/Renderer.hpp"
#include <algorithm>

namespace rct {

void Ride::calculateFinalRatings(const GForceData& gData) {
    // Simplified RCT Rating formula
    
    // 1. Excitement: More segments and moderate G-forces are better
    ratings.excitement = (segments.size() * 0.1f) + (gData.verticalMax * 0.5f);
    
    // 2. Intensity: High G-forces increase intensity rapidly
    ratings.intensity = (gData.verticalMax * 1.2f) + (gData.lateralMax * 2.0f);
    if (gData.verticalMin < -1.0f) ratings.intensity += 2.0f; // Airtime adds intensity

    // 3. Nausea: Primarily from lateral Gs
    ratings.nausea = (gData.lateralMax * 1.5f) + (ratings.intensity * 0.2f);

    // Limit values for sanity
    if (ratings.excitement > 10.0f) ratings.excitement = 10.0f;
    if (ratings.intensity > 15.0f) ratings.intensity = 15.0f;
    if (ratings.nausea > 10.0f) ratings.nausea = 10.0f;
}

void Ride::render(Renderer& renderer, const World& world, const Camera& cam) const {
    for (const auto& segment : segments) {
        int sx, sy;
        world.gridToScreen(segment.tx, segment.ty, segment.baseHeight, sx, sy, cam);

        uint8_t color = 4; 
        switch (segment.type) {
            case TrackType::STRAIGHT:    color = 4; break;
            case TrackType::CURVE_LEFT:  color = 1; break;
            case TrackType::CURVE_RIGHT: color = 2; break;
            case TrackType::SLOPE_UP:    color = 3; break;
            case TrackType::SLOPE_DOWN:  color = 6; break;
        }

        int h = (World::TILE_HALF / 2) * cam.zoom;
        int w = World::TILE_HALF * cam.zoom;

        for (int dy = -h/4; dy < h/4; ++dy) {
            int lineW = w / 2;
            for (int dx = -lineW/2; dx < lineW/2; ++dx) {
                renderer.putPixel(sx + dx, sy + dy, color);
            }
        }
    }
}

} // namespace rct
