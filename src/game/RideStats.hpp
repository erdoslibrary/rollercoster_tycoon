#ifndef RCT_RIDE_STATS_HPP
#define RCT_RIDE_STATS_HPP

namespace rct {

struct RideRatings {
    float excitement; // 흥분도
    float intensity;  // 강도
    float nausea;     // 멀미도

    RideRatings() : excitement(0), intensity(0), nausea(0) {}
};

struct GForceData {
    float verticalMax;
    float verticalMin;
    float lateralMax;

    GForceData() : verticalMax(1.0f), verticalMin(1.0f), lateralMax(0.0f) {}
};

} // namespace rct

#endif // RCT_RIDE_STATS_HPP
