#pragma once

#include "SFML/Graphics.hpp"

class Application{
    sf::RenderWindow window;
    int currentScene;
public:
    Application(int resX, int resY);
    void Run();

    int StartMenuScene();
    int GameplayScene();
};