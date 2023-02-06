#include <SFML/Graphics.hpp>

class Text {
    sf::Text m_text;

    public:
    Text(const std::string& text, const sf::Font& font ,size_t fontSize);
    void SetString(const std::string& _string);
    void SetLetterSpacing(float letterSpacing);
    void SetPosition(sf::Vector2f pos);

    sf::FloatRect GetGlobalBounds() const;

    void draw(sf::RenderWindow& window);
};