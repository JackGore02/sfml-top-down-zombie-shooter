#include "SFML_PauseMenu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

SFML_PauseMenu::SFML_PauseMenu(string menufilename, int windowWidth, int windowHeight)
{
	ifstream inputFile(menufilename);

	if (inputFile.is_open())
	{
		string stringLine;

		// Read the 1st line - background texture
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			// Load the texture file and set it to the sprite
			m_backgroundTexture.loadFromFile(stringLine);
			m_backgroundSprite.setTexture(m_backgroundTexture);

			// Set the origin, texture rectangle, and position of the sprite
			sf::Vector2u textureSize = m_backgroundTexture.getSize();
			m_backgroundSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
			m_backgroundSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
			m_backgroundSprite.setPosition(0, 0);

			// Set the rectangle texture which is centred at 0,0
			m_backgroundRect.left = -static_cast<float> (textureSize.x / 2);
			m_backgroundRect.top = -static_cast<float> (textureSize.y / 2);
			m_backgroundRect.width = textureSize.x;
			m_backgroundRect.height = textureSize.y;
		}

		// Read the 2nd line - game logo texture
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			// Load the texture file and set it to the sprite
			m_logoTexture.loadFromFile(stringLine);
			m_logoSprite.setTexture(m_logoTexture);

			// Set the origin, texture rectangle, and position of the sprite
			sf::Vector2u textureSize = m_logoTexture.getSize();
			m_logoSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
			m_logoSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
			m_logoSprite.setPosition(0, m_backgroundTexture.getSize().y / 2 * 0.9f);
			// Sets the position of the logo 90% down the screen height
		}

		// Read the 3rd line - font
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			m_gameFont.loadFromFile(stringLine);
		}

		string redString;
		string greenString;
		string blueString;

		int redInt;
		int greenInt;
		int blueInt;

		// Read the 4th line - Normal Text Fill Colour (R, G, B)
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			// Seperate them into three strings
			istringstream stringStream(stringLine);
			stringStream >> redString;
			stringStream >> greenString;
			stringStream >> blueString;

			// Convert them to integers
			redInt = stoi(redString);
			greenInt = stoi(greenString);
			blueInt = stoi(blueString);

			// Set the values to m_normalTextColour
			m_normalTextColour = sf::Color(redInt, greenInt, blueInt);
		}

		// Read the 5th line - Normal Text Outline Colour (R, G, B)
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			istringstream stringStream(stringLine);
			stringStream >> redString;
			stringStream >> greenString;
			stringStream >> blueString;

			redInt = stoi(redString);
			greenInt = stoi(greenString);
			blueString = stoi(blueString);

			m_normalOutlineColour = sf::Color(redInt, greenInt, blueInt);
		}

		// Read the 6th line - Highlight Text Fill Colour (R, G, B)
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			// Seperate them into three strings 
			istringstream stringStream(stringLine);
			stringStream >> redString;
			stringStream >> greenString;
			stringStream >> blueString;

			// Convert them to integers
			redInt = stoi(redString);
			greenInt = stoi(greenString);
			blueInt = stoi(blueString);

			// Set the values to m_highlightTextColour
			m_highlightTextColour = sf::Color(redInt, greenInt, blueInt);
		}

		// Read the 7th line - Highlight Text Outline Colour (R, G, B)
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			// Seperate them into three strings 
			istringstream stringStream(stringLine);
			stringStream >> redString;
			stringStream >> greenString;
			stringStream >> blueString;

			// Convert them to integers
			redInt = stoi(redString);
			greenInt = stoi(greenString);
			blueInt = stoi(blueString);

			// Set the values to m_highlightOutlineColour
			m_highlightOutlineColour = sf::Color(redInt, greenInt, blueInt);
		}

		string xBorderString;
		string yBorderString;

		int xBorderInt;
		int yBorderInt;

		// Read the 8th line - Border size in pixels
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			// Seperate them into two strings
			istringstream stringStream(stringLine);
			stringStream >> xBorderString;
			stringStream >> yBorderString;

			// Convert them into integers
			xBorderInt = stoi(xBorderString);
			yBorderInt = stoi(yBorderString);

			// Set the values to m_menuItemBorder
			m_menuItemBorder.x = xBorderInt;
			m_menuItemBorder.y = yBorderInt;
		}

		m_menuAlignment = CENTRE_JUSTIFIED;

		// Read the 9th line - Menu Text Position
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			if (!strcmp(stringLine.c_str(), "LEFT_JUSTIFIED"))
				m_menuAlignment = LEFT_JUSTIFIED;
			if (!strcmp(stringLine.c_str(), "CENTRE_JUSTIFIED"))
				m_menuAlignment = CENTRE_JUSTIFIED;
			if (!strcmp(stringLine.c_str(), "RIGHT_JUSTIFIED"))
				m_menuAlignment = RIGHT_JUSTIFIED;
		}

		sf::Text start;
		start.setFont(m_gameFont);
		start.setCharacterSize(50);
		start.setStyle(sf::Text::Bold);
		start.setFillColor(m_normalTextColour);
		start.setOutlineColor(m_normalOutlineColour);
		start.setOutlineThickness(1.0f);

		sf::Vector2f textpos;

		if (m_menuAlignment == CENTRE_JUSTIFIED)
		{
			textpos.x = m_backgroundRect.left + m_backgroundRect.width / 2;
			textpos.y = m_backgroundRect.top + m_menuItemBorder.y;
		}
		else if (m_menuAlignment == LEFT_JUSTIFIED)
		{
			textpos.x = m_backgroundRect.left + m_menuItemBorder.x;
			textpos.y = m_backgroundRect.top + m_menuItemBorder.y;
		}
		else if (m_menuAlignment == RIGHT_JUSTIFIED)
		{
			textpos.x = m_backgroundRect.left + m_backgroundRect.width - m_menuItemBorder.x;
			textpos.y = m_backgroundRect.top + m_menuItemBorder.y;
		}

		do
		{
			// Read every line for each menu item
			getline(inputFile, stringLine);
			if (inputFile.good())
			{
				// Set the text's position to textpos
				start.setPosition(textpos.x, textpos.y);

				// Set the text's string to stringLine
				start.setString(stringLine);

				// Set the text's origin to Mid-Left coordinate
				sf::FloatRect textbox = start.getLocalBounds();
				if (m_menuAlignment == CENTRE_JUSTIFIED)
				{
					start.setOrigin(textbox.width / 2, textbox.height / 2);
				}
				else if (m_menuAlignment == LEFT_JUSTIFIED)
				{
					start.setOrigin(0, textbox.height / 2);
				}
				else if (m_menuAlignment == RIGHT_JUSTIFIED)
				{
					start.setOrigin(textbox.width, textbox.height / 2);
				}

				// Store the text to m_menuText array
				m_menuText.push_back(start);

				// Calculate next text position by incrementing its y-position
				textpos.y += start.getLocalBounds().height * 1.2f;
			}
			else
				break;
		} while (true);

		m_highlightIndex = 0;
		m_menuText[m_highlightIndex].setFillColor(m_highlightTextColour);
		m_menuText[m_highlightIndex].setOutlineColor(m_highlightOutlineColour);

		m_prevDownKeyPressed = true;
		m_prevUpKeyPressed = true;
		m_prevReturnKeyPressed = true;

		inputFile.close();
	}
}

SFML_PauseMenu::~SFML_PauseMenu()
{
	// Clear the menu item list
	m_menuText.clear();
}

void SFML_PauseMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_backgroundSprite, states);
	target.draw(m_logoSprite, states);

	for (size_t count = 0; count < m_menuText.size(); count++)
		target.draw(m_menuText[count], states);
}

int SFML_PauseMenu::update(sf::Time elapsedTime)
{
	bool m_currentDownKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	bool m_currentUpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	bool m_currentReturnKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Return);

	if (m_currentDownKeyPressed && !m_prevDownKeyPressed)
	{
		// Set the colour of the current highlighted menu item to normal colour 
		m_menuText[m_highlightIndex].setFillColor(m_normalTextColour);
		m_menuText[m_highlightIndex].setOutlineColor(m_normalOutlineColour);

		// Increment the highlighted index by one, modulated by the number of menu items
		m_highlightIndex++;
		m_highlightIndex = m_highlightIndex % m_menuText.size();

		// Set the colour of the new highlighted menu item to highlight colour 
		m_menuText[m_highlightIndex].setFillColor(m_highlightTextColour);
		m_menuText[m_highlightIndex].setOutlineColor(m_highlightOutlineColour);
	}
	else if (m_currentUpKeyPressed && !m_prevUpKeyPressed)
	{
		// Set the colour of the current highlighted menu item to normal colour
		m_menuText[m_highlightIndex].setFillColor(m_normalTextColour);
		m_menuText[m_highlightIndex].setOutlineColor(m_normalOutlineColour);

		// Increment the highlighted index by one, if it becomes negative then add
		// the number of menu items to it
		m_highlightIndex--;
		if (m_highlightIndex < 0)
			m_highlightIndex = m_highlightIndex + m_menuText.size();

		// Set the colour of the new highlighted menu item to highlight colour
		m_menuText[m_highlightIndex].setFillColor(m_highlightTextColour);
		m_menuText[m_highlightIndex].setOutlineColor(m_highlightOutlineColour);
	}

	m_prevDownKeyPressed = m_currentDownKeyPressed;
	m_prevUpKeyPressed = m_currentUpKeyPressed;

	bool hasSelected = false;

	if (m_currentReturnKeyPressed && !m_prevReturnKeyPressed)
		hasSelected = true;

	m_prevReturnKeyPressed = m_currentReturnKeyPressed;

	if (hasSelected)
		return m_highlightIndex;
	else
		return -1;
}
