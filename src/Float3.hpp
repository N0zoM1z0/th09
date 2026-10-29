#pragma once

struct Float2
{
    float x;
    float y;
};

typedef char Float2SizeIs08[(sizeof(Float2) == 0x08) ? 1 : -1];

struct Float3
{
    Float3() {}
    Float3(float x, float y, float z);
    Float3 operator+(const Float3 &other) const;
    Float3 operator-(const Float3 &other) const;
    Float3 operator*(float scalar) const;
    Float3 operator/(float scalar) const;
    Float3 *operator/=(float scalar);
    Float3 &operator+=(const Float3 &other);
    Float3 &operator-=(const Float3 &other);
    void FromAngleMagnitude(float angle, float magnitude);
    operator float *();

    float x;
    float y;
    float z;
};

typedef char Float3SizeIs0C[(sizeof(Float3) == 0x0C) ? 1 : -1];
