#include "Application.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "Text.h"

#include "utility.h"
#include "json.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

const std::string DATA_FILE_PATH = "./res/data/data.json";

Application::Application(int resX, int resY) 
    : window(sf::VideoMode(resX, resY), "Asteroids", sf::Style::Close),
    // start the game always with scene 0
    currentScene(Scene::START_MENU_SCENE),
    // start the game always with score 0
    score(0)
    // volumes will be initialized after reading data from file
    // if file doesn't exist they will revert to default values
{ 
    srand(time(0));
    // load the font from file
    upheavtt.loadFromFile("./res/fonts/upheavtt.ttf"); 
    
    // if data file exists, load the data
    if(std::filesystem::exists(DATA_FILE_PATH)) {
        loadData();
    }
    // if data file does not exist, reset attributes to default values
    // then create data file and save these values
    else {
        resetData();
        saveData();
    }
}

void Application::loadData() {
    // open the data file
    std::ifstream dataInputStream(DATA_FILE_PATH);

    // read data and parse as json
    nlohmann::json data;
    dataInputStream >> data;

    // read attributes from json
    MASTER_VOLUME = data["MASTER_VOLUME"];
    SFX_VOLUME = data["SFX_VOLUME"];
    MUSIC_VOLUME = data["MUSIC_VOLUME"];
}

void Application::resetData() {
    // set default values for attributes
    MASTER_VOLUME = 1.0f;
    SFX_VOLUME = 1.0f;
    MUSIC_VOLUME = 1.0f;
}

void Application::saveData() {
    // create json
    nlohmann::json data;

    // write data to json object
    data["MASTER_VOLUME"] = MASTER_VOLUME;
    data["SFX_VOLUME"] = SFX_VOLUME;
    data["MUSIC_VOLUME"] = MUSIC_VOLUME;

    // open file to write
    std::ofstream o(DATA_FILE_PATH);
    // save data in file
    // setw(4) is to prettify it
    o << std::setw(4) << data << std::endl;
}

void Application::Run() {
    

    // when one scene ends, swtich to scene that is currentScene
    // when one scene ends, that scene function should already have modified currentScene value
    // so load into that scene
    // run infinitely until we encounter a scene that does not exist, then close the application
    while(true) {
        switch(currentScene) {
            // start menu
            case Scene::START_MENU_SCENE:
            currentScene = StartMenuScene();
            break;

            // gameplay scene
            case Scene::GAMEPLAY_SCENE:
            currentScene = GameplayScene();
            break;

            // end game scene
            case Scene::GAME_OVER_SCENE:
            currentScene = GameOverScene();
            break;

            // settings menu scene
            case Scene::SETTINGS_SCENE:
            currentScene = SettingsScene();
            break;

            // if switching to scene that does not exist, close application
            default:
                window.close();
                return;
        }
    }
}

// multi scene constants
const std::string FONT_FILEPATH = "./res/fonts/BAHNSCHRIFT 1.TTF";

// -------------------------------------------------------------------------------------------------------
// Start Menu Scene
Scene Application::StartMenuScene() {

    // load music
    sf::Music music_background;
    music_background.openFromFile("./res/audio/music/DeepSpaceA.wav");
    // 100 is the default volume
    music_background.setVolume(100.0f * MASTER_VOLUME * MUSIC_VOLUME);
    music_background.setLoop(true);
    music_background.play();

    Text text_start("Start", upheavtt, 100);
    text_start.SetLetterSpacing(2.0f);
    text_start.SetPosition(sf::Vector2f(360, 250) - sf::Vector2f(20, 0));

    Text text_settings("Settings", upheavtt, 48);
    text_settings.SetLetterSpacing(1.5f);
    text_settings.SetPosition(sf::Vector2f(360, 400) - sf::Vector2f(10, 0));

    Text text_quit("Quit", upheavtt, 48);
    text_quit.SetLetterSpacing(1.5f);
    text_quit.SetPosition(sf::Vector2f(360, 460) - sf::Vector2f(10, 0));

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
            {
                window.close();
                return Scene::UNDEFINED_SCENE;
            }
            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                auto mp = sf::Mouse::getPosition(window);
                if(text_start.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                    // change scene to gameplay
                    return Scene::GAMEPLAY_SCENE;
                }

                if(text_settings.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                    // change to settings scene
                    return Scene::SETTINGS_SCENE;
                }

                if(text_quit.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                    // quit application
                    // switch to out of bounds scene
                    return Scene::UNDEFINED_SCENE;
                }
            }
        }

        window.clear();
        text_start.draw(window);
        text_settings.draw(window);
        text_quit.draw(window);
        window.display();
    }
}

// -------------------------------------------------------------------------------------------------------
// Settings Scene
// when increaser or reducer is held down the value of master volume changes by this value per second
const float VOLUME_CHANGE_SPEED = 0.1f;
// if button is held down, tick sound will play after this interval in seconds
const float SETTINGS_TICK_SOUND_INTERVAL = 0.1f;

Scene Application::SettingsScene() {

    Text text_settingsTitle("Settings", upheavtt, 80);
    text_settingsTitle.SetLetterSpacing(2.0f);
    text_settingsTitle.SetPosition(sf::Vector2f(360, 100) - sf::Vector2f(20, 0));

    // ---------------------------------------------
    // master volume
    Text text_masterVolumeLabel("Master Volume", upheavtt, 36);
    text_masterVolumeLabel.SetLetterSpacing(1.0f);
    text_masterVolumeLabel.SetPosition(sf::Vector2f(180, 250));

    Text text_masterVolumeValue(to_string_with_precision(MASTER_VOLUME), upheavtt, 36);
    text_masterVolumeValue.SetLetterSpacing(1.0f);
    text_masterVolumeValue.SetPosition(sf::Vector2f(600, 250));

    Text text_masterVolumeReducer("<", upheavtt, 36);
    text_masterVolumeReducer.SetPosition(sf::Vector2f(540, 250));

    Text text_masterVolumeIncreaser(">", upheavtt, 36);
    text_masterVolumeIncreaser.SetPosition(sf::Vector2f(660, 250));

    // ---------------------------------------------
    // sfx volume

    Text text_sfxVolumeLabel("Sfx Volume", upheavtt, 36);
    text_sfxVolumeLabel.SetLetterSpacing(1.0f);
    text_sfxVolumeLabel.SetPosition(sf::Vector2f(180, 300));

    Text text_sfxVolumeValue(to_string_with_precision(SFX_VOLUME), upheavtt, 36);
    text_sfxVolumeValue.SetLetterSpacing(1.0f);
    text_sfxVolumeValue.SetPosition(sf::Vector2f(600, 300));

    Text text_sfxVolumeReducer("<", upheavtt, 36);
    text_sfxVolumeReducer.SetPosition(sf::Vector2f(540, 300));

    Text text_sfxVolumeIncreaser(">", upheavtt, 36);
    text_sfxVolumeIncreaser.SetPosition(sf::Vector2f(660, 300));

    // ---------------------------------------------
    // music volume

    Text text_musicVolumeLabel("Music Volume", upheavtt, 36);
    text_musicVolumeLabel.SetLetterSpacing(1.0f);
    text_musicVolumeLabel.SetPosition(sf::Vector2f(180, 350));

    Text text_musicVolumeValue(to_string_with_precision(MUSIC_VOLUME), upheavtt, 36);
    text_musicVolumeValue.SetLetterSpacing(1.0f);
    text_musicVolumeValue.SetPosition(sf::Vector2f(600, 350));

    Text text_musicVolumeReducer("<", upheavtt, 36);
    text_musicVolumeReducer.SetPosition(sf::Vector2f(540, 350));

    Text text_musicVolumeIncreaser(">", upheavtt, 36);
    text_musicVolumeIncreaser.SetPosition(sf::Vector2f(660, 350));

    Text text_goBackButton("< Go Back", upheavtt, 48);
    text_goBackButton.SetLetterSpacing(1.0f);
    text_goBackButton.SetPosition(sf::Vector2f(360.0f, 600));

    // audio
    sf::SoundBuffer soundBuffer_tick;
    soundBuffer_tick.loadFromFile("./res/audio/sfx/WoodBlock1.wav");
    sf::Sound sound_tick(soundBuffer_tick);
    // lambda that plays the tick sound for ui
    // if the sound is not playing play it
    // if it's playing dont play it until the given interval passes
    // playing the sound on every frame messes things up so we play it after interval
    
    // capture the sound for tick by reference
    auto play_tick = [&sound_tick]() {

        switch(sound_tick.getStatus()) {
            // sound is stopped
            case 0:
                sound_tick.play();
                break;
            // sound is already playing
            case 2:
                if(sound_tick.getPlayingOffset() >= sf::seconds(SETTINGS_TICK_SOUND_INTERVAL)) {
                    sound_tick.stop();
                    sound_tick.play();
                }
            break; 
        }
    };

    // render loop

    sf::Clock clock;
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
            {
                window.close();
                return Scene::UNDEFINED_SCENE;
            }
        }

        const float dt = clock.restart().asSeconds();

        // detect clicks and performs actions for the events
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto mp = sf::Mouse::getPosition(window);

            // reduce master volume button
            if(text_masterVolumeReducer.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                MASTER_VOLUME -= VOLUME_CHANGE_SPEED * dt;
                play_tick();
            }

            // increase master volume button
            else if(text_masterVolumeIncreaser.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                MASTER_VOLUME += VOLUME_CHANGE_SPEED * dt;
                play_tick();
            }

            // reduce sfx volume button
            else if(text_sfxVolumeReducer.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                SFX_VOLUME -= VOLUME_CHANGE_SPEED * dt;
                play_tick();
            }

            // increase sfx volume button
            else if(text_sfxVolumeIncreaser.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                SFX_VOLUME += VOLUME_CHANGE_SPEED * dt;
                play_tick();
            }

            // reduce music volume button
            else if(text_musicVolumeReducer.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                MUSIC_VOLUME -= VOLUME_CHANGE_SPEED * dt;
                play_tick();
            }

            // increase music volume button
            else if(text_musicVolumeIncreaser.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                MUSIC_VOLUME += VOLUME_CHANGE_SPEED * dt;
                play_tick();
            }

            // go back to start menu button
            else if(text_goBackButton.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                // save the changes made
                saveData();
                return Scene::START_MENU_SCENE;
            }

            // clamp volumes between 0 and 1
            MASTER_VOLUME = std::clamp(MASTER_VOLUME, 0.0f, 1.0f);
            SFX_VOLUME = std::clamp(SFX_VOLUME, 0.0f, 1.0f);
            MUSIC_VOLUME = std::clamp(MUSIC_VOLUME, 0.0f, 1.0f);
        }

        // updates

        // update tick volume to we can hear the volume changes in real time
        sound_tick.setVolume(100.0f * MASTER_VOLUME * SFX_VOLUME);
        
        // update strings for value texts
        text_masterVolumeValue.SetString(to_string_with_precision(MASTER_VOLUME));
        text_sfxVolumeValue.SetString(to_string_with_precision(SFX_VOLUME));
        text_musicVolumeValue.SetString(to_string_with_precision(MUSIC_VOLUME));


        window.clear();
        
        // draw here

        text_settingsTitle.draw(window);

        // master volume
        text_masterVolumeLabel.draw(window);
        text_masterVolumeValue.draw(window);
        text_masterVolumeReducer.draw(window);
        text_masterVolumeIncreaser.draw(window);

        // sfx volume
        text_sfxVolumeLabel.draw(window);
        text_sfxVolumeValue.draw(window);
        text_sfxVolumeReducer.draw(window);
        text_sfxVolumeIncreaser.draw(window);

        // music volume
        text_musicVolumeLabel.draw(window);
        text_musicVolumeValue.draw(window);
        text_musicVolumeReducer.draw(window);
        text_musicVolumeIncreaser.draw(window);

        text_goBackButton.draw(window);

        window.display();
    }
}

// -------------------------------------------------------------------------------------------------------
// Gameplay Scene
// one asteroid will spawn after these many seconds
const float ASTEROID_SPAWN_SPEED = 1.0f;

static void normalize(sf::Vector2f& vec) {
    float magnitude = sqrtf((vec.x * vec.x) + (vec.y * vec.y));

    vec.x /= magnitude;
    vec.y /= magnitude;
}

static float cartesianAngle(const sf::Vector2f& vec) {
    float angle = 0.0f;

    if(vec.x > 0 && vec.y > 0) {
        // first quadrant
        angle = asin(vec.y);
    }
    else if(vec.x < 0 && vec.y > 0) {
        // second quadrant
        angle = 3.14159f - asin(vec.y);
    }
    else if(vec.x < 0 && vec.y < 0) {
        // thrid quadrant
        angle = 3.14159 - asin(vec.y);
    }
    else {
        // fourth quadrant
        angle = 2 * 3.14159 + asin(vec.y);
    }

    return angle;
}

Scene Application::GameplayScene() {

    // reset score
    score = 0;

    // load sprites

    Sprite sprite_staryBackground;
    // load a random background from 6 backgrounds
    short backgroundNum = rand() % 6 + 1;
    std::string backgroundFileName = "./res/images/background_0" + std::to_string(backgroundNum) + ".png";
    sprite_staryBackground.loadTextureFromFile(backgroundFileName);

    sprite_staryBackground.setPosition(sf::Vector2f(360, 360));
    sprite_staryBackground.setColor(sf::Color(255, 255, 255, 150));

    Spaceship player;
    player.loadTextureFromFile("./res/images/spaceship_02.png");
    player.setPosition(sf::Vector2f(360, 360));
    player.SetVolumes(MASTER_VOLUME, SFX_VOLUME, MUSIC_VOLUME);

    // load audio
    sf::SoundBuffer soundBuffer_asteroidExplosionSmall;
    soundBuffer_asteroidExplosionSmall.loadFromFile("./res/audio/sfx/Explosion9.wav");
    sf::Sound sound_asteroidExplosionSmall(soundBuffer_asteroidExplosionSmall);
    sound_asteroidExplosionSmall.setVolume(100.0f * MASTER_VOLUME * SFX_VOLUME);

    sf::SoundBuffer soundBuffer_asteroidExplosionLarge;
    soundBuffer_asteroidExplosionLarge.loadFromFile("./res/audio/sfx/MiniExplosionChainReaction.wav");
    sf::Sound sound_asteroidExplosionLarge(soundBuffer_asteroidExplosionLarge);
    sound_asteroidExplosionLarge.setVolume(100.0f * MASTER_VOLUME * SFX_VOLUME);


    sf::SoundBuffer soundBuffer_death;
    soundBuffer_death.loadFromFile("./res/audio/sfx/TotalBurnOut.wav");
    sf::Sound sound_death(soundBuffer_death);
    sound_death.setVolume(sound_death.getVolume() * MASTER_VOLUME * SFX_VOLUME);

    // initialize container for background music
    // we'll set a random track to play during run time
    sf::Music music_background;
    music_background.setVolume(100.0f * MASTER_VOLUME * MUSIC_VOLUME);

    Text text_score(std::to_string(score), upheavtt, 48);
    text_score.SetLetterSpacing(1.5f);
    text_score.SetPosition(sf::Vector2f(80, 10));


    // holds all the instantiated asteroids in the scene
    std::unordered_set<Asteroid*> asteroids;

    float timeSinceLastAsteroidSpawn = ASTEROID_SPAWN_SPEED;
   
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return Scene::UNDEFINED_SCENE;
            }
        }

        // close the application if escape is pressed
        // allows me to quickly stop the debugging, must be removed in final version
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        float dt = clock.restart().asSeconds();
        // ----------------------------------------------------------------------------------------------------
        // background music logic
        // select one of random 4 tracks
        // status, 0 = stopped, 1 = paused, 2 = playing
        // if previous song has finished playing start playing random
        // if no song was playing start playing random
        if(music_background.getStatus() == 0) {
            // get random number from 0-3
            size_t randomTrackNumber = rand() % 4;

            // calculate filepath
            const std::string filepath = "./res/audio/music/DynamicFight" + std::to_string(randomTrackNumber) + ".wav";
            // set file as source for music
            music_background.openFromFile(filepath);
            // play song
            music_background.play();
        }
        

        // ----------------------------------------------------------------------------------------------------
        // Asteroid spawn logic
        if(timeSinceLastAsteroidSpawn >= ASTEROID_SPAWN_SPEED) {
            
            // get a random number from 0-2 for spawning asteroid
            int randAsteroidSize = rand() % 3;

            Asteroid* a = nullptr;

            // all 3 sizes have same probabilty of spawning
            if(randAsteroidSize == 0) a = new Asteroid(AsteroidSize::SMALL);
            else if(randAsteroidSize == 1) a = new Asteroid(AsteroidSize::MEDIUM);
            else if(randAsteroidSize == 2) a = new Asteroid(AsteroidSize::LARGE);

            asteroids.insert(a);

            timeSinceLastAsteroidSpawn = 0;
        }
        else timeSinceLastAsteroidSpawn += dt;

        // ----------------------------------------------------------------------------------------------------

        window.clear();
        // Game logic


        player.update(dt, window);

        // loops through asteroids and bullets to check if any have collided
        // if they have collided reduces the hitpoints of the asteroid hit, and destroyes the bullet that hit
        player.AsteroidBulletCollision(asteroids);
        
        for(auto asteroid : asteroids) asteroid->update(dt);
        
        // draw here
        // draw in proper order, farthest objects first
        sprite_staryBackground.draw(window);
        for(auto asteroid : asteroids) asteroid->draw(window);
        player.draw(window);
        // auto bounds = player.getGlobalBounds();
        // printf("Bounds, left: %.2f, top: %.2f\n", bounds.left, bounds.top);
        //window.draw(asteroid);
        // change scene to -1 to close the game
        // change scene to 2 to change to game over scene
        if(player.AsteroidSpaceshipCollision(asteroids)) {
            // TODO: add a small wait so this sound effect can play
            sound_death.play();

            // release memory allocated for asteroids before leaving
            for(Asteroid* ast : asteroids) {
                delete ast;
            }
            asteroids.clear();
            
            // change scene to game over
            return Scene::GAME_OVER_SCENE;
        }
        // score should show on top of everything so render last
        text_score.SetString(std::to_string(score));
        text_score.draw(window);

        window.display();


        std::vector<Asteroid*> toDelete;
        std::vector<Asteroid*> toAdd;

        for(auto asteroid : asteroids) {
            // if asteroid has gone outside the game permitted bounds we need to destroy it
            if(!asteroid->isInBounds()) toDelete.push_back(asteroid);

            // if asteroid has reached 0 hitpoints, we need to destroy it
            if(asteroid->GetHitpoints() <= 0) {
                toDelete.push_back(asteroid);

                // since the asteroid is destroyed add its max hitpoints to score
                score += asteroid->GetMaxHitpoints();

                // if a large asteroid was destroyed by reaching 0 hitpoints, spawn 2 small ones
                if(asteroid->GetAsteroidSize() == AsteroidSize::LARGE) {

                    // get velocity of parent asteroid
                    // used to hurl child asteroids in proper direction
                    sf::Vector2f asteroidVelocity = asteroid->GetVelocity();
                    normalize(asteroidVelocity);

                    // find the cartesian angle of given velocity in radians
                    float velocityAngle = cartesianAngle(asteroidVelocity);

                    // add 0.15 rad angle to parent direction and hurl child1 in that direction
                    sf::Vector2f directionToHurl1(cos(velocityAngle + 0.15), sin(velocityAngle + 0.15));
                    Asteroid* b = new Asteroid(asteroid->GetPosition(), AsteroidSize::SMALL, asteroid->GetPosition() + directionToHurl1, 10e6);
                    toAdd.push_back(b);

                    // subtract 0.15rad angle to parent direction and hurl child2 in that direction
                    sf::Vector2f directionToHurl2(cos(velocityAngle - 0.15), sin(velocityAngle - 0.15));
                    Asteroid* c = new Asteroid(asteroid->GetPosition(), AsteroidSize::SMALL, asteroid->GetPosition() + directionToHurl2, 10e6);
                    toAdd.push_back(c); 

                    sound_asteroidExplosionLarge.play();
                }
                else {
                    // just play explosion sound
                    sound_asteroidExplosionSmall.play();
                }
            }
        }

        for(auto asteroid : toAdd) {
            asteroids.insert(asteroid);
        }

        for(auto toDeleteAsteroid: toDelete) {
            asteroids.erase(toDeleteAsteroid);
            delete(toDeleteAsteroid);
        }
    }
}

// -------------------------------------------------------------------------------------------------------
// End game scene

Scene Application::GameOverScene() {

    Text text_gameOver("Gameover", upheavtt, 64);
    text_gameOver.SetLetterSpacing(2.0f);
    // add some offset to properly align the text
    text_gameOver.SetPosition(sf::Vector2f(360, 180) - sf::Vector2f(20, 0));

    // final score text
    Text text_finalScore("Final Score", upheavtt, 40);
    text_finalScore.SetLetterSpacing(1.0f);
    text_finalScore.SetPosition(sf::Vector2f(360, 320));

    // the text displaying the actual number of the score
    Text text_finalScoreValue(std::to_string(score), upheavtt, 64);
    text_finalScoreValue.SetLetterSpacing(2.0f);
    text_finalScoreValue.SetPosition(sf::Vector2f(360, 380));

    // play again button
    Text text_playAgain("Play Again", upheavtt, 48);
    text_playAgain.SetLetterSpacing(1.5f);
    text_playAgain.SetPosition(sf::Vector2f(360, 480));

    // go to start menu button
    Text text_startMenu("Start Menu", upheavtt, 48);
    text_startMenu.SetLetterSpacing(1.5f);
    text_startMenu.SetPosition(sf::Vector2f(360, 540));

    // load audio
    sf::Music music_background;
    music_background.openFromFile("./res/audio/music/bgm_26.wav");
    music_background.setVolume(100.0f * MASTER_VOLUME * MUSIC_VOLUME);
    music_background.setLoop(true);
    music_background.play();

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
            {
                window.close();
                return Scene::UNDEFINED_SCENE;
            }
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto mp = sf::Mouse::getPosition(window);
            if(text_playAgain.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                // change to gameplay scene
                return Scene::GAMEPLAY_SCENE;
            }

            if(text_startMenu.GetGlobalBounds().contains((sf::Vector2f)mp)) {
                // change to start menu scene
                return Scene::START_MENU_SCENE;
            }            
        }

        window.clear();
        text_gameOver.draw(window);
        text_finalScore.draw(window);
        text_finalScoreValue.draw(window);
        text_playAgain.draw(window);
        text_startMenu.draw(window);
        window.display();
    }
}