#pragma once

#include <SFML/Graphics.hpp>
#include "SFML_TitleScreen.h"
#include "SFML_GameMenu.h"
#include "SFML_GameWorld.h"
#include "SFML_CreditScreen.h"
#include "SFML_PauseMenu.h"
#include "SFML_StoryScreen.h"

using namespace std;

class SFML_Application :
    public sf::NonCopyable
{
public :
                     SFML_Application(int windowWidth, int windowHeight);
                     ~SFML_Application();

    void             run();

private:
    void             update(sf::Time elapsedTime);
    void             render();

    // Function to update FPS data
    void             updateFPS(sf::Time elapsedTime);

    // True if the application has been successfully initialised
    bool             m_initialised;

    // Member attributes for graphic window
    sf::RenderWindow m_window;
    std::size_t      m_screenWidth;
    std::size_t      m_screenHeight;

    // Member attribute for capturing events
    sf::Event        m_event;

    sf::Font         m_textFont;

    sf::Text         m_FPSText;
    sf::Text         m_creditText;
    sf::Text         m_titleScreenText;

    enum AppState {
        TITLESCREEN,
        MAINMENU,
        STORYSCREEN,
        GAMEPLAYING,
        PAUSEMENU,
        EXITING,
        CREDITS
    };

    // Member attribute for AppState
    AppState         m_appState;

    // Member attribute for SFML_TitleScreen object
    SFML_TitleScreen m_titleScreen;

    SFML_GameMenu    m_mainMenu;

    SFML_CreditScreen m_creditScreen;

    SFML_PauseMenu   m_pauseMenu;

    SFML_StoryScreen m_storyScreen;

    // Member attributes for FPS data
    sf::Time         m_timeCounter;
    sf::Time         m_desiredUpdateTime;
    size_t           m_FPSCounter;

    // Game world object that contains m_gameWorldLayerList and m_camera
    SFML_GameWorld   m_gameWorld;
};

