#pragma once

#include "SFML/Graphics.hpp"

class Application{
    // window for rendering
    sf::RenderWindow window;
    // keep track of which scene is active, also used for switching scenes
    int currentScene;

    // font used throught the game
    sf::Font upheavtt;

    // keep track of current score
    size_t score;
public:
    Application(int resX, int resY);
    void Run();

    int StartMenuScene();
    int GameplayScene();
    int GameOverScene();
};