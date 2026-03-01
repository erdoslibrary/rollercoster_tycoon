#ifndef RCT_FIXED_HPP
#define RCT_FIXED_HPP

#include <cstdint>

namespace rct {

/**
 * @brief 16.16 Fixed-point arithmetic class for deterministic simulation.
 */
class Fixed {
private:
    int32_t raw;
    static const int32_t FRACTIONAL_BITS = 16;
    static const int32_t ONE = 1 << FRACTIONAL_BITS;

public:
    Fixed() : raw(0) {}
    explicit Fixed(int32_t i) : raw(i << FRACTIONAL_BITS) {}
    explicit Fixed(float f) : raw(static_cast<int32_t>(f * ONE)) {}

    // Raw value access
    static Fixed fromRaw(int32_t r) {
        Fixed f;
        f.raw = r;
        return f;
    }
    int32_t getRaw() const { return raw; }

    // Conversion
    float toFloat() const { return static_cast<float>(raw) / ONE; }
    int32_t toInt() const { return raw >> FRACTIONAL_BITS; }

    // Comparison
    bool operator==(const Fixed& other) const { return raw == other.raw; }
    bool operator!=(const Fixed& other) const { return raw != other.raw; }
    bool operator<(const Fixed& other) const { return raw < other.raw; }
    bool operator>(const Fixed& other) const { return raw > other.raw; }
    bool operator<=(const Fixed& other) const { return raw <= other.raw; }
    bool operator>=(const Fixed& other) const { return raw >= other.raw; }

    // Arithmetic
    Fixed operator+(const Fixed& other) const { return fromRaw(raw + other.raw); }
    Fixed operator-(const Fixed& other) const { return fromRaw(raw - other.raw); }
    
    Fixed operator*(const Fixed& other) const {
        return fromRaw(static_cast<int32_t>((static_cast<int64_t>(raw) * other.raw) >> FRACTIONAL_BITS));
    }

    Fixed operator/(const Fixed& other) const {
        if (other.raw == 0) return fromRaw(0); // Basic safety
        return fromRaw(static_cast<int32_t>((static_cast<int64_t>(raw) << FRACTIONAL_BITS) / other.raw));
    }

    Fixed& operator+=(const Fixed& other) { raw += other.raw; return *this; }
    Fixed& operator-=(const Fixed& other) { raw -= other.raw; return *this; }
};

} // namespace rct

#endif // RCT_FIXED_HPP
