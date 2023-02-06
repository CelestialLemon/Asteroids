#include "Text.h"

Text::Text(const std::string& text, const sf::Font& font, size_t fontSize)
: m_text(text, font, fontSize)
{
    // set origin to center of sprite
    auto textRect = m_text.getGlobalBounds();
    m_text.setOrigin(textRect.width / 2, textRect.height / 2);
}

void Text::SetLetterSpacing(float letterSpacing) {
    m_text.setLetterSpacing(letterSpacing);
}

void Text::SetPosition(sf::Vector2f pos) {
    m_text.setPosition(pos);
}

sf::FloatRect Text::GetGlobalBounds() const {
    return m_text.getGlobalBounds();
}

void Text::draw(sf::RenderWindow& window) {
    window.draw(m_text);
}