#pragma once

#include "SFML/Graphics.hpp"
#include "json.hpp"

// switch to undefined scene to close game
enum Scene {
    UNDEFINED_SCENE = -1,
    START_MENU_SCENE = 0,
    GAMEPLAY_SCENE = 1,
    GAME_OVER_SCENE = 2,
    SETTINGS_SCENE = 3
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

    // controls overall volume of the application, has value between 0 and 1
    float MASTER_VOLUME;
    // controls volume of sound effect like explosins, deaths, guns etc
    float SFX_VOLUME;
    // controls volume of bgm
    float MUSIC_VOLUME;

    // reads game data from file
    void loadData();
    // saves current values of attributes in json file
    void saveData();
    // resets attribute values to default
    void resetData();

public:
    Application(int resX, int resY);
    void Run();

    Scene StartMenuScene();
    Scene GameplayScene();
    Scene GameOverScene();
    Scene SettingsScene();
};