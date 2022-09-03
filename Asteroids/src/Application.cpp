#include "Application.h"
#include "Spaceship.h"
#include "Bullet.h"

Application::Application(int resX, int resY) 
    : window(sf::VideoMode(resX, resY), "Asteroids", sf::Style::Close)
{}

void Application::Run() {
    
    Sprite sprite_staryBackground;
    sprite_staryBackground.loadTextureFromFile("./res/images/background_01.png");
    sprite_staryBackground.setPosition(sf::Vector2f(360, 360));
    sprite_staryBackground.setColor(sf::Color(255, 255, 255, 150));

    Spaceship player;
    player.loadTextureFromFile("./res/images/spaceship.png");
    player.setPosition(sf::Vector2f(200, 400));
    
    sf::Clock clock;

    int a = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        window.clear();
        
        player.update(dt, window);
        
        // draw here
        // draw in proper order, farthest objects first
        sprite_staryBackground.draw(window);
        player.draw(window);

        if(a == 1000)
        { 
            a= 0;
            printf("FPS : %.2f\n", 1.0f / dt);
        }
        a++;
        window.display();
    }
}