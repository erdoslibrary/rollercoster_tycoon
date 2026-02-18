#include "Train.hpp"
#include "World.hpp"
#include "render/Renderer.hpp"
#include <algorithm>

namespace rct {

Train::Train(const std::vector<TrackSegment>& trackRef) 
    : track(trackRef), currentSegmentIndex(0), segmentProgress(0.0f), 
      velocity(0.02f), gravity(0.005f), friction(0.0005f) 
{
}

float Train::getSegmentLength(const TrackSegment& s) const {
    // Curves are usually longer than straight pieces (Simplified: 1.0 or 1.4)
    if (s.type == TrackType::CURVE_LEFT || s.type == TrackType::CURVE_RIGHT) return 1.414f;
    return 1.0f;
}

void Train::update() {
    if (track.empty()) return;

    const TrackSegment& current = track[currentSegmentIndex];
    
    // 1. Gravity & Physics Calculation
    // Slope affects acceleration: Downwards (+) Upwards (-)
    float slopeEffect = 0.0f;
    if (current.type == TrackType::SLOPE_DOWN) slopeEffect = gravity;
    else if (current.type == TrackType::SLOPE_UP) slopeEffect = -gravity;
    
    velocity += (slopeEffect - friction);
    
    // Simple speed limiters
    if (velocity < 0.005f) velocity = 0.005f; // Min speed to keep it moving
    if (velocity > 0.2f) velocity = 0.2f;     // Max speed for safety

    // 2. Movement along segments
    float len = getSegmentLength(current);
    segmentProgress += (velocity / len);
    
    // 3. G-Force Calculation (Every tick)
    calculateGForces(current);

    if (segmentProgress >= 1.0f) {
        segmentProgress = 0.0f;
        currentSegmentIndex = (currentSegmentIndex + 1) % track.size();
    }
}

void Train::calculateGForces(const TrackSegment& current) {
    // Basic Vertical G: 1.0 (Static) + Acceleration from curves
    float vertG = 1.0f;
    if (current.type == TrackType::SLOPE_UP) vertG -= (velocity * 2.0f); // Going up
    if (current.type == TrackType::SLOPE_DOWN) vertG += (velocity * 3.0f); // Going down fast

    // Basic Lateral G: From curves
    float latG = 0.0f;
    if (current.type == TrackType::CURVE_LEFT || current.type == TrackType::CURVE_RIGHT) {
        latG = (velocity * velocity) * 100.0f; // Simplified v^2/r
    }

    // Keep track of peaks
    if (vertG > currentGData.verticalMax) currentGData.verticalMax = vertG;
    if (vertG < currentGData.verticalMin) currentGData.verticalMin = vertG;
    if (latG > currentGData.lateralMax) currentGData.lateralMax = latG;
}

void Train::render(Renderer& renderer, const World& world, const Camera& cam) const {
    if (track.empty()) return;

    const TrackSegment& seg = track[currentSegmentIndex];
    
    // Interpolate world position for smooth train movement
    // (Simplified: using the segment's tile position + progress)
    // In real RCT, we'd need more complex interpolation for curves
    
    float fx = seg.tx;
    float fy = seg.ty;
    int h = seg.baseHeight;

    // Very simple linear progress for this prototype
    // For now, let's just use the current segment's start position for clarity
    int sx, sy;
    world.gridToScreen(static_cast<int>(fx), static_cast<int>(fy), h, sx, sy, cam);
    
    // Draw a "Train Car" (Yellow rectangle)
    int carW = 10 * cam.zoom;
    int carH = 6 * cam.zoom;
    for (int y = -carH; y < 0; ++y) {
        for (int x = -carW/2; x < carW/2; ++x) {
            renderer.putPixel(sx + x, sy + y, 40); // Yellow Index
        }
    }
}

} // namespace rct
