#include "Pendulum.h"
#include <random>

//ctors

Pendulum::Pendulum(float m, float a0, float r, float x, float y)
{
	this->x = x;
	this->y = y;
	this->dampingFactor = 1.f;
	init(m, a0, r);
}

Pendulum::Pendulum(float m, float a0, float r, Pendulum* upperPendulum)
{
	this->upperPendulum = upperPendulum;
	upperPendulum->lowerPendulum = this;
	this->dampingFactor = 1.1f;
	this->x = upperPendulum->getXPoint();
	this->y = upperPendulum->getXPoint();
	init(m, a0, r);
}

// dtor
Pendulum::~Pendulum()
{
	delete this->body;
	delete this->arm;
}

// getters
float Pendulum::getKineticEnergy()
{
	float velocitySquared = v * v + va * va * r * r;
	return 0.5f * mass * velocitySquared;
}


sf::Vector2f Pendulum::getBodyPosition() const
{
	return body->getPosition();
}

sf::Vector2f Pendulum::getAttachmentPoint() const
{
	return sf::Vector2f(x, y);
}


float Pendulum::getMaxDistance() const
{
	return r;
}

// setters
void Pendulum::setBodyPosition(const sf::Vector2f& newPosition)
{
	body->setPosition(newPosition);
}


void Pendulum::increaseEnergy()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(100.0f, 300.0f);
	float randomAmount = dis(gen);
	this->a += randomAmount;
}

// other functions

void Pendulum::update()
{
	float m1 = upperPendulum->mass;
	float m2 = this->mass;
	float a1 = upperPendulum->a;
	float a2 = this->a;
	float r1 = upperPendulum->r;
	float r2 = this->r;
	float v1 = upperPendulum->v;
	float v2 = this->v;

	float num1 = -GRAVITY * (2 * m1 + m2) * sin(a1);
	float num2 = -m2 * GRAVITY * sin(a1 - 2 * a2);
	float num3 = -2 * sin(a1 - a2) * m2 * (v2 * v2 * r2 + v1 * v1 * r1 * cos(a1 - a2));
	float den = r1 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));

	upperPendulum->va = (num1 + num2 + num3) / den;

	num1 = 2 * sin(a1 - a2);
	num2 = v1 * v1 * r1 * (m1 + m2);
	num3 = GRAVITY * (m1 + m2) * cos(a1) + v2 * v2 * r2 * m2 * cos(a1 - a2);
	den = r2 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
	this->va = (num1 * (num2 + num3)) / den;
}

void Pendulum::draw(sf::RenderWindow& window)
{
	window.draw(sf::RectangleShape(*this->arm));
	window.draw(sf::CircleShape(*this->body));
}

sf::Vector2f Pendulum::updatePos(float dt)
{
	this->v += this->va * dampingFactor * dt;
	this->a += this->v * dampingFactor * dt;

	this->x = upperPendulum != nullptr ? upperPendulum->getXPoint() : x;
	this->y = upperPendulum != nullptr ? upperPendulum->getYPoint() : y;
	this->a += this->va * dt;

	this->arm->setPosition(x, y);
	this->arm->setRotation(this->a * 180.f / PI);
	this->body->setPosition(this->getXPoint(), this->getYPoint());
	return sf::Vector2f(this->getXPoint(), this->getYPoint());
}

void Pendulum::printKineticEnergy()
{
	std::cout << "Kinetic Energy: " << getKineticEnergy() << " Joules" << std::endl;
}

//private functions

void Pendulum::init(float m, float a0, float radius)
{
	this->va = {};
	this->mass = m;
	this->r = radius;
	this->a = a0 * PI / 180;
	this->va = {};
	this->v = {};

	this->arm = new sf::RectangleShape(sf::Vector2f{ 5, radius });
	this->arm->setOrigin(this->arm->getSize().x / 2, 0);
	this->arm->setPosition(sf::Vector2f{ this->x, this->y });
	this->arm->setFillColor(sf::Color::Black);
	this->arm->setRotation(a * 180 / PI);

	this->body = new sf::CircleShape(25);
	this->body->setOrigin(sf::Vector2f{ this->body->getRadius(), this->body->getRadius() });
	this->body->setPosition(sf::Vector2f{ getXPoint(), getYPoint() });
	this->body->setFillColor(sf::Color(255, 0, 0, 255));
}

float Pendulum::getXPoint()
{
	return arm->getPosition().x - r * sin(a);
}

float Pendulum::getYPoint()
{
	return arm->getPosition().y + r * cos(a);
}
