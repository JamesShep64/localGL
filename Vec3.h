#pragma once
#include <cmath>
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
    std::cout<<"X = "<<x<<" Y = "<<y<<" Z = "<<z<<"\n";
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
inline Vec3 operator*(const Vec3& l, const float& r){
    Vec3 result;
    result.x = l.x*r;
    result.y = l.y*r;
    result.z = l.z*r;
    return result;
};
static Vec3 normalize(const Vec3& v){
    auto mag = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return Vec3(v.x/mag,v.y/mag,v.z/mag);
};
static Vec3 perpindicular(const Vec3 v){
    return Vec3(-v.z,v.y,v.x);
}
static float magnitude(const Vec3 v){
    auto mag = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return mag;
}
