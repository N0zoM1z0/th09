#include "AsciiManager.hpp"

Float3 Float3::operator*(float scalar) const
{
    return Float3(this->x * scalar, this->y * scalar, this->z * scalar);
}

Float3 &Float3::operator-=(const Float3 &other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

Float3 *Float3::operator/=(float scalar)
{
    float inverse = 1.0f / scalar;
    this->x *= inverse;
    this->y *= inverse;
    this->z *= inverse;
    return this;
}

Float3 &Float3::operator+=(const Float3 &other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}
