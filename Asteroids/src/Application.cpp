#include "Application.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "Asteroid.h"
#include <iostream>
#include <string>

Application::Application(int resX, int resY) 
    : window(sf::VideoMode(resX, resY), "Asteroids", sf::Style::Close),
    // start the game always with scene 0
    currentScene(0)
{ srand(time(0)); }

void Application::Run() {
    

    // when one scene ends, swtich to scene that is currentScene
    // when one scene ends, that scene function should already have modified currentScene value
    // so load into that scene
    // run infinitely until we encounter a scene that does not exist, then close the application
    while(true) {
        switch(currentScene) {
            // start menu
            case 0:
            currentScene = StartMenuScene();
            break;

            // gameplay scene
            case 1:
            currentScene = GameplayScene();
            break;

            // end game scene
            case 2:
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
int Application::StartMenuScene() {

    sf::Font bahnschrift;
    bahnschrift.loadFromFile(FONT_FILEPATH);

    Sprite sprite_startButton;
    sprite_startButton.loadTextureFromFile("./res/images/start_button.png");
    sprite_startButton.setPosition(sf::Vector2f(360, 300));

    Sprite sprite_quitButton;
    sprite_quitButton.loadTextureFromFile("./res/images/quit_button.png");
    sprite_quitButton.setPosition(sf::Vector2f(360, 400));

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
            {
                window.close();
                return -1;
            }
            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                auto mp = sf::Mouse::getPosition(window);
                if(sprite_startButton.getGlobalBounds().contains((sf::Vector2f)mp)) {
                    // change scene to gameplay
                    return 1;
                }
                if(sprite_quitButton.getGlobalBounds().contains((sf::Vector2f)mp)) {
                    // quit application
                    // switch to out of bounds scene
                    return -1;
                }
            }
        }

        window.clear();
        sprite_startButton.draw(window);
        sprite_quitButton.draw(window);
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

int Application::GameplayScene() {

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
                return -1;
            }
        }

        // close the application if escape is pressed
        // allows me to quickly stop the debugging, must be removed in final version
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        float dt = clock.restart().asSeconds();

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
        if(player.AsteroidSpaceshipCollision(asteroids)) return -1;

        window.display();


        std::vector<Asteroid*> toDelete;
        std::vector<Asteroid*> toAdd;

        for(auto asteroid : asteroids) {
            // if asteroid has gone outside the game permitted bounds we need to destroy it
            if(!asteroid->isInBounds()) toDelete.push_back(asteroid);

            // if asteroid has reached 0 hitpoints, we need to destroy it
            if(asteroid->GetHitpoints() <= 0) {
                toDelete.push_back(asteroid);
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
