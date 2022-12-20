#include "Application.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "Asteroid.h"

Application::Application(int resX, int resY) 
    : window(sf::VideoMode(resX, resY), "Asteroids", sf::Style::Close)
{ srand(time(0)); }

void Application::Run() {
    
    Sprite sprite_staryBackground;
    sprite_staryBackground.loadTextureFromFile("./res/images/background_01.png");
    sprite_staryBackground.setPosition(sf::Vector2f(360, 360));
    sprite_staryBackground.setColor(sf::Color(255, 255, 255, 150));

    Spaceship player;
    player.loadTextureFromFile("./res/images/spaceship.png");
    player.setPosition(sf::Vector2f(200, 400));

    // holds all the instantiated asteroids in the scene
    std::unordered_set<Asteroid*> asteroids;

    // spawn an asteroid every 0.5 seconds
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

        for(auto asteroid : asteroids) {
            if(!asteroid->isInBounds()) toDelete.push_back(asteroid);
            if(asteroid->GetHitpoints() <= 0) toDelete.push_back(asteroid);
        }

        for(auto toDeleteAsteroid: toDelete) {
            asteroids.erase(toDeleteAsteroid);
            delete(toDeleteAsteroid);
        }
    }
}