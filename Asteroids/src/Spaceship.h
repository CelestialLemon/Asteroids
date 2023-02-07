#pragma once

#include "Sprite.h"
#include "Rigidbody.h"
#include "Asteroid.h"
#include "Bullet.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <unordered_set>
#include <iostream>

// The player class
// Inherits from Sprite because it has a sprite
class Spaceship : public Sprite {
    
    // use to performs physics on the object
    Rigidbody m_rigidbody;
    
    // amount of force applied to the rigidbody per second
    // how much thrust the spaceship can generate
    float m_thrust;

    // holds the texture of bullets, for creating new one
    sf::Texture m_bulletTexture;

    // holds reference to all bullets in the game
    std::unordered_set<Bullet*> m_bullets;

    // polygon collider for spaceship
    sf::ConvexShape collider;
    
    sf::SoundBuffer soundBuffer_fire;
    sf::Sound sound_fire;
    
    // gun has charge level, increases every second by given speed, fires when reaching maxcharge
    int gunMaxCharge, gunRechargeSpeed;
    float gunCurrentCharge;

    // fire bullets if reached max charge
    void Fire();

    public:
    Spaceship();

    void AsteroidBulletCollision(const std::unordered_set<Asteroid*>& asteroids);

    bool AsteroidSpaceshipCollision(const std::unordered_set<Asteroid*>& asteroids);

    // update the position of the ship by calculating next position given currentPos and velocity and dt
    void updatePosition(float dt);

    // update the rotation depending on mouse position
    void updateRotation(float dt, sf::Vector2f mousePosition);

    // set volume for all the sound effects and music managed by this class
    void SetVolumes(const float MASTER_VOLUME, const float SFX_VOLUME, const float MUSIC_VOLUME);
    
    // take input from user to move the spaceship
    void update(float dt, sf::RenderWindow& window) override;

};