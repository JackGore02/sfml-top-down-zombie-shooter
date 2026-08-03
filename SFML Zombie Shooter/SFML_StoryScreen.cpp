#include "SFML_StoryScreen.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

SFML_StoryScreen::SFML_StoryScreen(string storyfilename, int windowWidth, int windowHeight, float charDelay)
{
	m_charDelay = charDelay;

	ifstream inputFile(storyfilename);

	if (inputFile.is_open())
	{
		string stringLine;

		// Read 1st line - font
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

		// Read the 2nd line - Normal Text Fill Colour (R, G, B)
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

		// Read the 3rd line - Menu Text Position
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			if (!strcmp(stringLine.c_str(), "CENTRE_JUSTIFIED"))
				m_menuAlignment = CENTRE_JUSTIFIED;
		}

		sf::Text credits;
		credits.setFont(m_gameFont);
		credits.setCharacterSize(33);
		credits.setStyle(sf::Text::Bold);
		credits.setFillColor(m_normalTextColour);

		sf::Vector2f textpos;

		if (m_menuAlignment == CENTRE_JUSTIFIED)
		{
			textpos.x = 0;
			textpos.y = -150;
		}

		// Initialised the clock before the first line is drawn
		m_clock.restart();

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
				m_storyText.push_back(credits);

				// Calculate the next text position by incrementing its y-position
				textpos.y += credits.getLocalBounds().height * 2.f;
			}
			else
				break;
		} while (true);
		inputFile.close();
	}
}

SFML_StoryScreen::~SFML_StoryScreen()
{
	// Clear the m_creditsText array
	m_storyText.clear();
}

void SFML_StoryScreen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	// Keep track of the current line being drawn
	static int currentLine = 0;

	// Check if the current line is fully drawn
	const sf::Text& currentText = m_storyText[currentLine];
	float elapsed = m_clock.getElapsedTime().asSeconds();
	int displayedChars = static_cast<int>(elapsed / m_charDelay);
	bool isLineFullyDrawn = (displayedChars >= static_cast<int>(currentText.getString().getSize()));
	float finishedDelay = m_clock.getElapsedTime().asSeconds();

	m_finishedDelay = 2.0f;

	// Draw all the previously displayed lines of text
	for (int i = 0; i < currentLine; i++)
	{
		target.draw(m_storyText[i], states);
	}

	// Draw the current line if it is fully drawn
	if (isLineFullyDrawn)
	{
		target.draw(currentText, states);

		// Move on to the next line if there is one
		if (currentLine < static_cast<int>(m_storyText.size()) - 1)
		{
			currentLine++;
			m_clock.restart();
		}
		else
		{
			if (finishedDelay >= m_finishedDelay)
			{
				m_isFinished = true;
			}
		}
	}
	// Otherwise, only draw the partially drawn text
	else
	{
		sf::String str = currentText.getString().substring(0, displayedChars);
		sf::Text partialText(currentText);
		partialText.setString(str);
		target.draw(partialText, states);
	}

	// Reset current line if we've reached the end of the text
	if (currentLine >= static_cast<int>(m_storyText.size()))
	{
		currentLine = 0;
	}
}

bool SFML_StoryScreen::isFinished() const
{
	return m_isFinished;
}

