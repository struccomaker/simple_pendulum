#include <iostream>
#include <string>
#include <vector>

#include "Pendulum.h"

#define PI 3.14159265

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 920), "Pendulum", sf::Style::Close, sf::ContextSettings(0, 0, 8));

    Pendulum p1(100.f, 90.f, 230.f, window.getSize().x / 2.f, 175.f);
    Pendulum p2(100.f, 90.f, 230.f, &p1);

    sf::Clock clock;
    sf::Font font;

    if (!font.loadFromFile("kenvector_future.ttf"))
    {
        std::cout << "Cannot load font" << std::endl;
        return 1;
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Black);
    text.setString("Click To Reset"); 
    text.setPosition(window.getSize().x / 2.f - text.getLocalBounds().width / 2.f, window.getSize().y / 2.f - text.getLocalBounds().height + 300.f);

    std::vector<sf::Vertex> line;
    sf::Vertex* lineArr;
    sf::Vector2i mousePos;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    p1.increaseEnergy();
                }
                break;
            }
        }

        p2.update();
        line.push_back(sf::Vertex(p2.updatePos(clock.getElapsedTime().asSeconds() * 20), sf::Color::Black));
        p1.updatePos(clock.getElapsedTime().asSeconds() * 20);

        clock.restart();
        lineArr = &line[0];
        window.clear(sf::Color::White);

        window.draw(lineArr, line.size(), sf::Lines);
        p1.draw(window);
        p2.draw(window);
        p1.printKineticEnergy();
        window.draw(text);
        window.display();
    }

    return 0;
}