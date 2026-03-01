#ifndef RCT_TRACK_HPP
#define RCT_TRACK_HPP

#include <vector>
#include <cstdint>
#include "common/Vector2.hpp"

#include "RideStats.hpp"

namespace rct {

enum class TrackType {
    STRAIGHT,
    CURVE_LEFT,
    CURVE_RIGHT,
    SLOPE_UP,
    SLOPE_DOWN
};

enum class Direction {
    NORTH, EAST, SOUTH, WEST
};

/**
 * @brief Represents a single piece of track.
 */
struct TrackSegment {
    int tx, ty;       // Tile position
    int baseHeight;   // Starting height
    TrackType type;
    Direction direction;

    TrackSegment(int x, int y, int h, TrackType t, Direction d)
        : tx(x), ty(y), baseHeight(h), type(t), direction(d) {}
};

/**
 * @brief Manages a collection of track segments forming a ride.
 */
class Ride {
private:
    std::vector<TrackSegment> segments;
    RideRatings ratings;

public:
    void addSegment(const TrackSegment& segment) {
        segments.push_back(segment);
    }

    const std::vector<TrackSegment>& getSegments() const {
        return segments;
    }

    void calculateFinalRatings(const GForceData& gData);
    const RideRatings& getRatings() const { return ratings; }

    void render(class Renderer& renderer, const class World& world, const class Camera& cam) const;
};

} // namespace rct

#endif // RCT_TRACK_HPP
