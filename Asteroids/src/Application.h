#pragma once

#include "SFML/Graphics.hpp"

// switch to undefined scene to close game
enum Scene {
    UNDEFINED_SCENE = -1,
    START_MENU_SCENE = 0,
    GAMEPLAY_SCENE = 1,
    GAME_OVER_SCENE = 2
};

class Application{
    // window for rendering
    sf::RenderWindow window;
    // keep track of which scene is active, also used for switching scenes
    Scene currentScene;

    // font used throught the game
    sf::Font upheavtt;

    // keep track of current score
    size_t score;
public:
    Application(int resX, int resY);
    void Run();

    Scene StartMenuScene();
    Scene GameplayScene();
    Scene GameOverScene();
};