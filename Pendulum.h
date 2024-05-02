#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

class Pendulum
{
public:
    // ctor
    Pendulum(float m, float a0, float r, float x, float y);
    Pendulum(float m, float a0, float r, Pendulum* upperPendulum);

    // dtor
    ~Pendulum();

    // getters
    float getKineticEnergy();
    sf::Vector2f getBodyPosition() const;
    sf::Vector2f getAttachmentPoint() const;
    float getMaxDistance() const;

    // Setters
    void setBodyPosition(const sf::Vector2f& newPosition);
    void increaseEnergy();

    //  functions
    void update();
    void draw(sf::RenderWindow& window);
    sf::Vector2f updatePos(float dt);
    void printKineticEnergy();

private:
    // cnstants
    const float PI = 3.14159265358979f;
    const float GRAVITY = 9.81f;

    //  functions
    void init(float m, float a0, float r);
    float getXPoint();
    float getYPoint();

    // private members
    float mass, x, y, r; // x y position, mass of ball, radius
    // va is angular acceleration and v is angular velocity, a for angle
    float va, v, a;

    Pendulum* upperPendulum = nullptr;
    Pendulum* lowerPendulum = nullptr;
    sf::CircleShape* body;
    sf::RectangleShape* arm;

    float dampingFactor; // for energy loss
};