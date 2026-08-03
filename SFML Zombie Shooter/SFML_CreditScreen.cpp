#include "SFML_CreditScreen.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

SFML_CreditScreen::SFML_CreditScreen(string creditfilename, int windowWidth, int windowHeight)
{
	ifstream inputFile(creditfilename);

	if (inputFile.is_open())
	{
		string stringLine;

		// Read 1st line - company logo texture
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			m_logoTexture.loadFromFile(stringLine);
			m_logoSprite.setTexture(m_logoTexture);

			sf::Vector2u textureSize = m_logoTexture.getSize();
			m_logoSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
			m_logoSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
			m_logoSprite.setPosition(0, 0);
			m_logoSprite.setScale(0.3f, 0.3f);
		}
		// Read 2nd line - font
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

		// Read the 3rd line - Normal Text Fill Colour (R, G, B)
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

		m_menuAlignment = CENTRE_JUSTIFIED;

		// Read the 4th line - Menu Text Position
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			if (!strcmp(stringLine.c_str(), "CENTRE_JUSTIFIED"))
				m_menuAlignment = CENTRE_JUSTIFIED;
		}

		sf::Text credits;
		credits.setFont(m_gameFont);
		credits.setCharacterSize(50);
		credits.setStyle(sf::Text::Bold);
		credits.setFillColor(m_normalTextColour);

		sf::Vector2f textpos;

		if (m_menuAlignment == CENTRE_JUSTIFIED)
		{
			textpos.x = 0;
			textpos.y = windowHeight / 2;
		}

		do
		{
			// Read every line for each text element
			getline(inputFile, stringLine);
			if (inputFile.good())
			{
				// Set the texts position to textpos
				credits.setPosition(textpos.x, textpos.y);

				// Set the texts string to stringLine
				credits.setString(stringLine);

				// Set the texts origin to origin to center
				sf::FloatRect textbox = credits.getLocalBounds();
				credits.setOrigin(textbox.width / 2, textbox.height / 2);

				// Store the text to m_creditsText array
				m_creditsText.push_back(credits);

				// Calculate the next text position by incrementing its y-position
				textpos.y += credits.getLocalBounds().height * 2.f;
			}
			else
				break;
		} while (true);
		inputFile.close();
	}
}

SFML_CreditScreen::~SFML_CreditScreen()
{
	// Clear the m_creditsText array
	m_creditsText.clear();
}

void SFML_CreditScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_logoSprite, states);

	for (size_t count = 0; count < m_creditsText.size(); count++)
	{
		target.draw(m_creditsText[count], states);
	}
}
