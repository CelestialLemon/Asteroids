#pragma once

#include <SFML/Graphics.hpp>

sf::Vector2f Normalize ( sf::Vector2f vec ) {
    float magnitude = sqrtf((vec.x * vec.x) + (vec.y * vec.y));
    
    vec.x /= magnitude;
    vec.y /= magnitude;

    return vec;
}