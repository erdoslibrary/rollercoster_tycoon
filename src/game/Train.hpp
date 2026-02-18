#ifndef RCT_TRAIN_HPP
#define RCT_TRAIN_HPP

#include <vector>
#include "Track.hpp"
#include "common/Fixed.hpp"

#include "RideStats.hpp"

namespace rct {

class Train {
private:
    const std::vector<TrackSegment>& track;
    int currentSegmentIndex;
    float segmentProgress; 
    float velocity;        
    
    float gravity;         
    float friction;        

    GForceData currentGData;

public:
    Train(const std::vector<TrackSegment>& trackRef);

    void update();
    void render(class Renderer& renderer, const class World& world, const class Camera& cam) const;

    const GForceData& getGForceData() const { return currentGData; }

private:
    float getSegmentLength(const TrackSegment& s) const;
    void calculateGForces(const TrackSegment& current);
};

} // namespace rct

#endif // RCT_TRAIN_HPP
