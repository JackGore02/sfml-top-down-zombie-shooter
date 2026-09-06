#include "SFML_Application.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

SFML_Application::SFML_Application(int windowWidth, int windowHeight)
	: m_window(sf::VideoMode(windowWidth, windowHeight), "SFML Application", sf::Style::Default)
	, m_initialised(true)
	, m_screenWidth(windowWidth)
	, m_screenHeight(windowHeight)
	, m_appState(TITLESCREEN)
	, m_titleScreen("Media/Textures/title-screen.png", windowWidth, windowHeight)
	, m_mainMenu("Media/MenuInfo/main-menu.txt", windowWidth, windowHeight)
	, m_desiredUpdateTime(sf::seconds(1.f / 60.f))
	, m_timeCounter(sf::seconds(0))
	, m_FPSCounter(0)
	, m_gameWorld(windowWidth, windowHeight, &m_window)
	, m_creditScreen("Media/CreditsInfo/credits-menu.txt", windowWidth, windowHeight)
	, m_pauseMenu("Media/MenuInfo/pause-menu.txt", windowWidth, windowHeight)
	, m_storyScreen("Media/MenuInfo/background-story.txt", windowWidth, windowHeight, 0.1f)
{
	// Load fonts to be used for on-screen messages -----------------
	m_initialised = m_textFont.loadFromFile("Media/Fonts/SegoeMarker.ttf");

	// Set the message text object to use the font we just loaded
	m_FPSText.setFont(m_textFont);
	m_creditText.setFont(m_textFont);
	m_titleScreenText.setFont(m_textFont); 

	m_mainMenu.setScale(0.85f, 0.85f);
	m_mainMenu.setPosition(m_screenWidth / 2, m_screenHeight / 2);

	m_pauseMenu.setScale(0.85f, 0.85f);
	m_pauseMenu.setPosition(m_screenWidth / 2, m_screenHeight / 2);

	m_creditScreen.setPosition(m_screenWidth / 2, m_screenHeight);

	m_storyScreen.setPosition(m_screenWidth / 2, m_screenHeight / 2);
}

SFML_Application::~SFML_Application()
{
}

void SFML_Application::update(sf::Time elapsedTime)
{
	// Check if the player has tried to close the window, if so close the application
	// This m_event triggers when 1) The x window button is pressed or 2) Alt-F4 keys are pressed
	bool keyPressed = false;
	sf::Keyboard::Key pressedKey = sf::Keyboard::Unknown;
	while (m_window.pollEvent(m_event))
	{
		if (m_event.type == sf::Event::Closed)
			m_window.close();
		else if (m_event.type == sf::Event::KeyPressed)
		{
			keyPressed = true;
			pressedKey = m_event.key.code;
		}
	}

	int mainMenuSelection;
	int pauseMenuSelection;

	// Prepare the message text and its position
	switch (m_appState)
	{
	case TITLESCREEN:
		if (keyPressed)
			if (pressedKey == sf::Keyboard::Return)
			m_appState = MAINMENU;

		m_titleScreen.update(elapsedTime);
		m_titleScreenText.setString("The Apocalypse");

		break;
	case MAINMENU:
		mainMenuSelection = m_mainMenu.update(elapsedTime);
		if (mainMenuSelection >= 0)
		{
			if (mainMenuSelection == 0)
				m_appState = STORYSCREEN;
			else if (mainMenuSelection == 1)
				m_appState = CREDITS;
			else if (mainMenuSelection == 2)
				m_appState = EXITING;
		}

		break;
	case CREDITS:
		if (keyPressed)
			m_appState = MAINMENU;                                     

		break;
	case STORYSCREEN:
		if (keyPressed)
		{
			if (pressedKey == sf::Keyboard::Escape)
			{
				m_appState = GAMEPLAYING;
			}
		}
		else if (m_storyScreen.isFinished())
		{
			m_appState = GAMEPLAYING;
		}

		break;
	case GAMEPLAYING:
		if (keyPressed)
		{
			if (pressedKey == sf::Keyboard::Escape)
				m_appState = PAUSEMENU;
		}

		m_gameWorld.update(elapsedTime.asSeconds());

		break;
	case PAUSEMENU:
		pauseMenuSelection = m_pauseMenu.update(elapsedTime);
		if (pauseMenuSelection >= 0)
		{
			if (pauseMenuSelection == 0)
				m_appState = GAMEPLAYING;
			else if (pauseMenuSelection == 1)
				m_appState = CREDITS;
			else if (pauseMenuSelection == 2)
				m_appState = EXITING;
		}

		break;
	case EXITING:
		m_window.close();

		break;
	}

	m_FPSText.setPosition(m_screenWidth / 5, m_screenHeight / 18);
	m_FPSText.setCharacterSize(40);

	sf::FloatRect FPSTextbox = m_FPSText.getLocalBounds();
	m_FPSText.setOrigin(FPSTextbox.width / 2, FPSTextbox.height / 2);

	m_creditText.setPosition(m_screenWidth / 2, m_screenHeight / 2);
	m_creditText.setCharacterSize(40);

	sf::FloatRect creditTextbox = m_creditText.getLocalBounds();
	m_creditText.setOrigin(creditTextbox.width / 2, creditTextbox.height / 2);

	m_titleScreenText.setPosition(m_screenWidth / 2, m_screenHeight / 2.2);
	m_titleScreenText.setCharacterSize(150);
	m_titleScreenText.setFillColor(sf::Color::Red);
	m_titleScreenText.setStyle(sf::Text::Bold);
	m_titleScreenText.setOutlineColor(sf::Color::Black);
	m_titleScreenText.setOutlineThickness(3.f);

	sf::FloatRect titleTextbox = m_titleScreenText.getLocalBounds();
	m_titleScreenText.setOrigin(titleTextbox.width / 2, titleTextbox.height / 2);
}

void SFML_Application::render()
{
	m_window.clear();

	switch (m_appState)
	{
	case TITLESCREEN:
		m_window.draw(m_titleScreen);
		m_window.draw(m_titleScreenText);
		break;
	case MAINMENU:
		m_window.draw(m_mainMenu);
		break;
	case CREDITS:
		m_window.draw(m_creditScreen);
		m_creditScreen.move(0.f, -0.02f);
		break;
	case STORYSCREEN:
		m_window.draw(m_storyScreen);
		break;
	case GAMEPLAYING:
		m_window.draw(m_gameWorld);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
		{
			m_window.draw(m_FPSText);
		}
		break;
	case PAUSEMENU:
		m_window.draw(m_pauseMenu);
		break;
	case EXITING:
		break;
	}

	m_window.display();
}

void SFML_Application::run()
{
	if (!m_initialised)
		return;

	sf::Clock clock;

	// Variable to track the time since update() was last executed
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	bool displayFPS = false;

	while (m_window.isOpen())
	{
		// Get the elapsed time since the last loop
		sf::Time elapsedTime = clock.restart();

		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > m_desiredUpdateTime)
		{
			// Update application states
			update(m_desiredUpdateTime);

			// Subtract m_desiredUpdateTime from timeSinceLastUpdate
			timeSinceLastUpdate -= m_desiredUpdateTime;
		}

		// Calculate FPS
		updateFPS(elapsedTime);

		// Render result to the screen
		render();
	}
}

void SFML_Application::updateFPS(sf::Time elapsedTime)
{
	// Increment the time counter by the time since the last render
	m_timeCounter += elapsedTime;

	// Increment the FPS counter by one
	m_FPSCounter += 1;

	// If one second has passed
	if (m_timeCounter >= sf::seconds(1.0f))
	{
		// Set the message text
		m_FPSText.setString("Refresh Rate = " + to_string(m_FPSCounter) + " FPS \n" + "Time between screen refreshes = "
			+ to_string(m_timeCounter.asMicroseconds() / m_FPSCounter) + " Microseconds \n"
		    );

		// Take away one second from the time counter
		m_timeCounter -= sf::seconds(1.0f);

		// Reset the FPS counter
		m_FPSCounter = 0;
	}
}
