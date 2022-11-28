#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "Rigidbody.h"

class Asteroid {

    // convex shape of the asteroids that is also the sprite for the object
    sf::ConvexShape m_asteroidShape;

    // radius which will be used while randomly generating vertices for the convex shape of asteroid
    float m_radius;

    // current hitpoints of a given asteroid instance
    float m_currentHitpoints;

    // maximum hitpoints of a given asteroid instance (asteroids spawn with these hitpoints)
    float m_maxHitpoints;

    // rigidbody to control physics and motion object
    Rigidbody m_rigidbody;

    // proceduraly generate vertices for the convex shape of the object
    void SetPoints();

    // update position depending on velocity given by rigidbody
    void updatePosition(float dt);

    // throw the asteroid in player's direction
    void Hurl();


    public:
    // default contructor with default values
    Asteroid();

    // parameterized contructors for setting custom values of following properties
    Asteroid(float radius, float maxHitpoints, int nPoints);

    // explicitly set the position of asteroid, should only be set when a new asteroid is instantiated
    void SetPosition(sf::Vector2f position);

    // draw the convex shape of the asteroid to given window
    void draw(sf::RenderWindow& window);

    bool IsPointInside(sf::Vector2f point);

    void Hit(float damage);

    float GetHitpoints();
    // returns if the asteroids is the permitted bounds or not
    bool isInBounds();

    // update function is called in application every frame use it for physics taking input etc.
    void update(float dt);

};