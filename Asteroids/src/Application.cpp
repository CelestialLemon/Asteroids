#include "Application.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "Asteroid.h"
#include <iostream>
#include <string>

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

Application::Application(int resX, int resY) 
    : window(sf::VideoMode(resX, resY), "Asteroids", sf::Style::Close)
{ srand(time(0)); }

void Application::Run() {
    
    Sprite sprite_staryBackground;

    // load a random background from 6 backgrounds
    short backgroundNum = rand() % 6 + 1;
    std::string backgroundFileName = "./res/images/background_0" + std::to_string(backgroundNum) + ".png";
    sprite_staryBackground.loadTextureFromFile(backgroundFileName);

    sprite_staryBackground.setPosition(sf::Vector2f(360, 360));
    sprite_staryBackground.setColor(sf::Color(255, 255, 255, 150));
    Spaceship player;
    player.loadTextureFromFile("./res/images/spaceship.png");
    player.setPosition(sf::Vector2f(360, 360));

    // holds all the instantiated asteroids in the scene
    std::unordered_set<Asteroid*> asteroids;
    int nAsteroidsDestroyed = 0;

    // spawn an asteroid every 1.0f seconds
    const float spawnSpeed = 1.0f;
    float timeSinceLastAsteroidSpawn = spawnSpeed;
   
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // close the application if escape is pressed
        // allows me to quickly stop the debugging, must be removed in final version
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        float dt = clock.restart().asSeconds();

        if(timeSinceLastAsteroidSpawn >= spawnSpeed) {
            
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

        window.clear();
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

        window.display();

        std::vector<Asteroid*> toDelete;
        std::vector<Asteroid*> toAdd;

        for(auto asteroid : asteroids) {
            // if asteroid has gone outside the game permitted bounds we need to destroy it
            if(!asteroid->isInBounds()) toDelete.push_back(asteroid);

            // if asteroid has reached 0 hitpoints, we need to destroy it
            if(asteroid->GetHitpoints() <= 0) {
                toDelete.push_back(asteroid);
                nAsteroidsDestroyed++;
                std::cout << nAsteroidsDestroyed << '\n';
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
                    asteroids.insert(b); 

                    // subtract 0.15rad angle to parent direction and hurl child2 in that direction
                    sf::Vector2f directionToHurl2(cos(velocityAngle - 0.15), sin(velocityAngle - 0.15));
                    Asteroid* c = new Asteroid(asteroid->GetPosition(), AsteroidSize::SMALL, asteroid->GetPosition() + directionToHurl2, 10e6);
                    asteroids.insert(c); 
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