#pragma once
#include <iostream>
struct Vec3{
  float x,y,z;
  Vec3() = default;
  Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
  Vec3& operator=(const Vec3& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }
  void print(){
    std::cout<<"X = "<<x<<" Y = "<<y<<"\n";
  }
};
inline Vec3 operator+(const Vec3& l, const Vec3& r){
    Vec3 result;
    result.x = l.x + r.x;
    result.y = l.y + r.y;
    result.z = l.z + r.z;
    return result;
};
inline Vec3 operator-(const Vec3& l, const Vec3& r){
    Vec3 result;
    result.x = l.x - r.x;
    result.y = l.y - r.y;
    result.z = l.z - r.z;
    return result;
};
