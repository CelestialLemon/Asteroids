#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// a basic GameObject
// set the params like color position scale etc. All this is stored in the SFML properties for sf::Sprite class

class Sprite {
	// set the m_sprite field with loadTextureFromFile
	sf::Sprite m_sprite;

	// m_texture loads and holds the texture till the object has to be destroyed
	sf::Texture m_texture;

public:
	void loadTextureFromFile(const std::string& filepath);
	
	void setColor(sf::Color color);
	void setPosition(sf::Vector2f pos);
	void setScale(sf::Vector2f scale);

	// Move changes the position of the given GameObject with the given Vector2
	void Move(sf::Vector2f dir);
	
	// calls the SFML draw function on the given window and draws m_sprite
	void draw(sf::RenderWindow& window);
};