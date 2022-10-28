#include "Application.h"
#include "Spaceship.h"
#include "Bullet.h"
#include "Asteroid.h"

Application::Application(int resX, int resY) 
    : window(sf::VideoMode(resX, resY), "Asteroids", sf::Style::Close)
{ srand(time(0)); }

float operator* (sf::Vector2f left, sf::Vector2f right) {
    return (left.x * right.x) + (left.y * right.y);
}

bool isInTriangle(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c) {
    
    bool b0 = (sf::Vector2f(p.x - a.x, p.y - a.y) * sf::Vector2f(a.y - b.y, b.x - a.x) > 0);
    bool b1 = (sf::Vector2f(p.x - b.x, p.y - b.y) * sf::Vector2f(b.y - c.y, c.x - b.x) > 0);
    bool b2 = (sf::Vector2f(p.x - c.x, p.y - c.y) * sf::Vector2f(c.y - a.y, a.x - c.x) > 0);

    return (b0 == b1 && b1 == b2);
}

void Application::Run() {
    
    Sprite sprite_staryBackground;
    sprite_staryBackground.loadTextureFromFile("./res/images/background_01.png");
    sprite_staryBackground.setPosition(sf::Vector2f(360, 360));
    sprite_staryBackground.setColor(sf::Color(255, 255, 255, 150));

    Spaceship player;
    player.loadTextureFromFile("./res/images/spaceship.png");
    player.setPosition(sf::Vector2f(200, 400));

    Asteroid asteroid1(100, 100, 50);
    asteroid1.SetPosition(sf::Vector2f(400, 200));
    
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

        window.clear();
        
        player.update(dt, window);
        asteroid1.update(dt);

        
        // draw here
        // draw in proper order, farthest objects first
        sprite_staryBackground.draw(window);
        player.draw(window);
        asteroid1.draw(window);

        // auto bounds = player.getGlobalBounds();
        // printf("Bounds, left: %.2f, top: %.2f\n", bounds.left, bounds.top);
        //window.draw(asteroid);

        window.display();
    }
}