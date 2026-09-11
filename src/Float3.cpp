#include "AsciiManager.hpp"

Float3::Float3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Float3 Float3::operator+(const Float3 &other) const
{
    return Float3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Float3 Float3::operator-(const Float3 &other) const
{
    return Float3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Float3 Float3::operator*(float scalar) const
{
    return Float3(this->x * scalar, this->y * scalar, this->z * scalar);
}
