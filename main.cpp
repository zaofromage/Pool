#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>

#include "Ball.h"
#include "Timer.h"

int main() {
   sf::RenderWindow window(sf::VideoMode(1080, 720), "Boucle de Jeu SFML");
   window.setFramerateLimit(60);
   srand(time(NULL));
   std::vector<Ball> balls;
   balls.push_back(Ball(320, 360, "timothe"));
   balls.push_back(Ball(540, 1, "corentin"));
   balls[0].addForce(Vector(1, 0));
   balls[1].addForce(Vector(0, 2));
   Ball* pushMode = nullptr;
   int mouseX = 0;
   int mouseY = 0;
   Timer timer;
   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed) {
            timer.stopAll();
            window.close();
         }
         if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
               Ball* b = Ball::containsBall(mouseX, mouseY, balls);
               if (b != nullptr) {
                  pushMode = b;
               } else {
                  std::shared_ptr<Ball> ball = std::shared_ptr<Ball>(new Ball(
                      mouseX - Ball::RADIUS / 2, mouseY - Ball::RADIUS / 2));
                  balls.push_back(*ball);
               }
            }
         }
         if (event.type == sf::Event::MouseButtonReleased) {
            if (pushMode != nullptr) {
               float limiter = 0.05f;
               Vector velo = pushMode->getCenter() - Vector(mouseX, mouseY);
               velo *= limiter;
               pushMode->addForce(velo);
               pushMode = nullptr;
            }
         }
         if (event.type == sf::Event::MouseMoved) {
            mouseX = event.mouseMove.x;
            mouseY = event.mouseMove.y;
         }
      }

      // Logique du jeu ici (mise à jour)

      // Effacer l'écran
      window.clear();
      for (Ball& b : balls) {
         b.update(window);
      }
      if (pushMode != nullptr) {
      }
      Ball::handleCollisions(balls);
      window.display();
   }

   return 0;
}
