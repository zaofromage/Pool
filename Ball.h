#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

#include "Vector.h"

class Ball {
private:
   Vector dir;
   sf::CircleShape circle;
   bool isMoving;
   float friction;
   float speed;
   std::string name;
public:
   static const int RADIUS = 20;

   Ball(int x, int y)
       : dir(Vector(0, 0)),
         circle(RADIUS),
         isMoving(false),
         friction(0.99f),
         speed(0),
         name("default") {
      circle.setPosition(x, y);
      circle.setFillColor(sf::Color::Red);
   }

   Ball(int x, int y, std::string name)
       : dir(Vector(0, 0)),
         circle(RADIUS),
         isMoving(false),
         friction(0.99f),
         speed(0),
         name(name) {
      circle.setPosition(x, y);
      circle.setFillColor(sf::Color::Red);
   }

   void update(sf::RenderWindow& window) {
      if (isMoving) {
         bounce(window);
         //applyFriction();
         Vector displacement = dir * speed;
         circle.move(displacement.toSfVector());
         if (speed <= 0.05f) {
            setDir(Vector(0, 0));
            isMoving = false;
            speed = 0;
         }
      }
      window.draw(circle);
   }

   void addForce(const Vector& force) {
      isMoving = true;
      dir = force;
      dir.normalize();
      speed = force.norm();  // Utiliser la norme du vecteur comme vitesse
   }

   void applyFriction() {
      speed *= friction;         // Réduit la vitesse
      if (speed < 0) speed = 0;  // Empêche la vitesse de devenir négative
   }

   void bounce(sf::RenderWindow& window) {
      if (circle.getPosition().x <= 0 ||
          circle.getPosition().x >=
              window.getSize().x - circle.getRadius() * 2) {
         dir.x *= -1;
      }
      if (circle.getPosition().y <= 0 ||
          circle.getPosition().y >=
              window.getSize().y - circle.getRadius() * 2) {
         dir.y *= -1;
      }
   }

   Vector getCenter() const {
      return Vector(circle.getPosition().x + circle.getRadius(),
                    circle.getPosition().y + circle.getRadius());
   }

   const sf::CircleShape& getCircle() const { return circle; }

   Vector getDir() const { return dir; }

   void setDir(Vector v) {
      isMoving = true;
      dir = v;
      dir.normalize();
   }

   float getSpeed() const { return speed; }

   const std::string& getName() const {
      return name;
   }

   void setSpeed(float s) {
      speed = s;
      isMoving = true;
   }

   Vector getVectorSpeed() {
      return dir * speed;
   }

   static void exchangeForce(Ball& a, Ball& b) {
      Vector adir = a.getDir();
      float aspeed = a.getSpeed();
      a.setDir(b.getDir());
      a.setSpeed(b.getSpeed());
      b.setDir(adir);
      b.setSpeed(aspeed);
   }

   static bool checkCollision(Ball& ball1, Ball& ball2) {
      Vector sep = ball1.getCenter() - ball2.getCenter();
      return sep.norm() <=
             ball1.getCircle().getRadius() + ball2.getCircle().getRadius();
   }

   static void resolveCollisions(Ball& a, Ball& b) {
      if (Vector::isColinear(a.getDir(), b.getDir())) {
         exchangeForce(a, b);
      } else if (a.getDir().isNull()) {
         Vector normal = a.getCenter() - b.getCenter();
         a.setSpeed(b.getSpeed());
         a.setDir(normal);
         b.setSpeed(0);
      } else if (b.getDir().isNull()) {
         Vector normal = b.getCenter() - a.getCenter();
         b.setSpeed(a.getSpeed());
         b.setDir(normal);
         a.setSpeed(0);
      } 
      else {
         //le fait en question
         Vector rab = b.getCenter() - a.getCenter();
         Vector rba = a.getCenter() - b.getCenter();
         Vector va = a.getVectorSpeed();
         Vector vb = b.getVectorSpeed();
         float cosTetaA = std::abs(Vector::scalar(va, rab) / va.norm() * rab.norm());
         float cosTetaB = std::abs(Vector::scalar(vb, rba) / vb.norm() * rba.norm());
         float var = va.norm() * cosTetaA;
         float vbr = vb.norm() * cosTetaB;
         // ce systeme ne suffie pas, il faut ajouter le fait que le vecteur directeur de la boule pointe
         // le plus vers l'autre boule (ex: si une boule A va a 12km/h vers la gauche et que une boule B va a 10k
         // m/h vers le bas mais percute A en pleine course, c'est B qui initie le choc)
         Ball& init = (var > vbr) ? a : b;
         Ball& hit  = (var > vbr) ? b : a;
         Vector normal = hit.getCenter() - init.getCenter();
         normal.normalize();
         float initSpeed = init.getSpeed();
         Vector* tan = nullptr;
         if (normal.x * normal.y > 0) {
            tan = new Vector(normal.y * -1, normal.x);
         } else {
            tan = new Vector(normal.y, normal.x * -1);
         }
         tan->normalize();   
         init.setSpeed(hit.getSpeed());
         init.setDir(*tan);
         delete tan;
         hit.setSpeed(initSpeed);
         hit.setDir(normal);
      }
   }

   static void handleCollisions(std::vector<Ball>& balls) {
      for (size_t i = 0; i < balls.size(); ++i) {
         for (size_t j = i + 1; j < balls.size(); ++j) {
            if (checkCollision(balls[i], balls[j])) {
               resolveCollisions(balls[i], balls[j]);
            }
         }
      }
   }

   static Ball* containsBall(int x, int y, std::vector<Ball>& balls) {
      for (Ball& b : balls) {
         Vector sep = b.getCenter() - Vector(x, y);
         if (sep.norm() <= b.getCircle().getRadius()) {
            return &b;
         }
      }
      return nullptr;
   }
};
