#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

class Vector {
public:
   float x, y;

   Vector() : x(0), y(0) {}
   Vector(float x, float y) : x(x), y(y) {}

   float norm() const { return std::sqrt(x * x + y * y); }

   void normalize() {
      float n = norm();
      if (n != 0) {  // Vérification pour éviter la division par zéro
         x /= n;
         y /= n;
      }
   }

   Vector operator+(const Vector& other) const {
      return Vector(x + other.x, y + other.y);
   }

   Vector operator+(const sf::Vector2f& other) const {
      return Vector(x + other.x, y + other.y);
   }

   Vector operator-(const Vector& other) const {
      return Vector(x - other.x, y - other.y);
   }

   Vector operator-(const sf::Vector2f& other) const {
      return Vector(x - other.x, y - other.y);
   }

   Vector operator*(float delta) const { return Vector(x * delta, y * delta); }

   void operator*=(float delta) {
      x *= delta;
      y *= delta;
   }

   float dot(const Vector& other) const {
        return x * other.x + y * other.y;
   }

   void reverse() {
      x *= -1;
      y *= -1;
   }  

   sf::Vector2f toSfVector() const { return sf::Vector2f(x, y); }

   static bool isColinear(Vector v1, Vector v2) {
      if (!v1.isNull() && !v2.isNull()) {
         return std::abs(v1.x * v2.y - v1.y * v2.x) < 1e-10;
      }
      return false;
   }

   static float scalar(const Vector& v1, const Vector& v2) {
      return (v1.x * v2.x) + (v1.y * v2.y);
   }

   bool isNull() { return x == 0 && y == 0; }
};
