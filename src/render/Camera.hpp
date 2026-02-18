#ifndef RCT_CAMERA_HPP
#define RCT_CAMERA_HPP

namespace rct {

class Camera {
public:
    float x;
    float y;
    float zoom;
    int rotation; // 0: 0°, 1: 90°, 2: 180°, 3: 270°

    Camera() : x(0), y(0), zoom(1.0f), rotation(0) {}

    void rotateClockwise() {
        rotation = (rotation + 1) % 4;
    }

    void rotateCounterClockwise() {
        rotation = (rotation + 3) % 4;
    }

    void adjustZoom(float delta) {
        zoom += delta;
        if (zoom < 0.5f) zoom = 0.5f;
        if (zoom > 4.0f) zoom = 4.0f;
    }
};

} // namespace rct

#endif // RCT_CAMERA_HPP
