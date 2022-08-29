#pragma once

#include "SFML/Graphics.hpp"

class Application{
    sf::RenderWindow window;
public:
    Application(int resX, int resY);
    void Run();
};