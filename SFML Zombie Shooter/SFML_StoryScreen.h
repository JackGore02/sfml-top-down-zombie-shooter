#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

class SFML_StoryScreen :
    public sf::Drawable, public sf::Transformable
{
public: 
                        SFML_StoryScreen(string storyfilename, int windowWidth, int windowHeight, float charDelay);
                        ~SFML_StoryScreen();

    bool                isFinished() const;

private:
    virtual void        draw(sf::RenderTarget& target, sf::RenderStates states) const;
    

    sf::Font            m_gameFont;
    sf::Color           m_normalTextColour;
    vector<sf::Text>    m_storyText;
    sf::Clock mutable   m_clock;
    float               m_charDelay;
    mutable bool        m_isFinished;
    mutable float       m_finishedDelay;

    enum MENU_ALIGNMENT
    {
        CENTRE_JUSTIFIED,
    };

    MENU_ALIGNMENT     m_menuAlignment;
};

