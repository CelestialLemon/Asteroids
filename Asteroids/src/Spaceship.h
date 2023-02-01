#pragma once

#include "Sprite.h"
#include "Rigidbody.h"
#include "Asteroid.h"
#include "Bullet.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_set>
#include <iostream>

// The player class
// Inherits from Sprite because it has a sprite
class Spaceship : public Sprite {
    
    // use to performs physics on the object
    Rigidbody m_rigidbody;
    
    // amount of force applied to the rigidbody per second
    float m_thrust;

    // holds the texture of bullets, for creating new one
    sf::Texture m_bulletTexture;
    //std::vector<Bullet*> m_bullets;
    std::unordered_set<Bullet*> m_bullets;

    // polygon collider for spaceship
    sf::ConvexShape collider;
    
    // gun has charge level, increases every second by given speed, fires when reaching maxcharge
    int gunMaxCharge, gunRechargeSpeed;
    float gunCurrentCharge;

    // fire bullets if reached max charge
    void Fire();

    public:
    Spaceship();

    void AsteroidBulletCollision(const std::unordered_set<Asteroid*>& asteroids);

    bool AsteroidSpaceshipCollision(const std::unordered_set<Asteroid*>& asteroids, sf::RenderWindow& window);

    // update the position of the ship by calculating next position given currentPos and velocity and dt
    void updatePosition(float dt);

    // update the rotation depending on mouse position
    void updateRotation(float dt, sf::Vector2f mousePosition);
    
    // take input from user to move the spaceship
    void update(float dt, sf::RenderWindow& window) override;

};