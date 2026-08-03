#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class SFML_CreditScreen :
    public sf::Drawable, public sf::Transformable
{
public:
                       SFML_CreditScreen(string creditfilename, int windowWidth, int windowHeight);
                       ~SFML_CreditScreen();

private:
    virtual void       draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Texture        m_logoTexture;
    sf::Sprite         m_logoSprite;

    sf::Font           m_gameFont;
    sf::Color          m_normalTextColour;
    vector<sf::Text>   m_creditsText;

    enum MENU_ALIGNMENT
    {
        CENTRE_JUSTIFIED,
    };

    MENU_ALIGNMENT     m_menuAlignment;
};

