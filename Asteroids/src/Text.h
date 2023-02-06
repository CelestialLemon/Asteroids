#include <SFML/Graphics.hpp>

class Text {
    sf::Text m_text;

    public:
    Text(const std::string& text, const sf::Font& font ,size_t fontSize);
    void SetLetterSpacing(float letterSpacing);
    void SetPosition(sf::Vector2f pos);

    void draw(sf::RenderWindow& window);
};