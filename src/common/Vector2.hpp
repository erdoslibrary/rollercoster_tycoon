#ifndef RCT_VECTOR2_HPP
#define RCT_VECTOR2_HPP

#include "Fixed.hpp"

namespace rct {

/**
 * @brief 2D Vector using Fixed-point arithmetic.
 */
struct Vector2 {
    Fixed x;
    Fixed y;

    Vector2() : x(), y() {}
    Vector2(Fixed x_val, Fixed y_val) : x(x_val), y(y_val) {}
    Vector2(int32_t x_int, int32_t y_int) : x(Fixed(x_int)), y(Fixed(y_int)) {}

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(const Fixed& scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Isometric Projection Helpers
    // (x, y) -> (screenX, screenY)
    // RCT Isometric: screenX = (x - y), screenY = (x + y) / 2
    Vector2 toIsometric() const {
        Fixed screenX = x - y;
        Fixed screenY = (x + y) / Fixed(2);
        return Vector2(screenX, screenY);
    }
};

} // namespace rct

#endif // RCT_VECTOR2_HPP
