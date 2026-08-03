#include "SFML_SpriteAnimation.h"

SFML_SpriteAnimation::SFML_SpriteAnimation()
	: m_currentFrameNumber(0)
	, m_canbeInterrupted(true)
	, m_isLoop(true)
{

}

SFML_SpriteAnimation::~SFML_SpriteAnimation()
{
	// Clearing the lists of Rectangles and Centres 
	m_frameCentreList.clear();
	m_frameRectangleList.clear();
}

void SFML_SpriteAnimation::setSpriteSheet(sf::Texture* spriteTexture)
{
	m_spritesheet = spriteTexture;
}

sf::Texture* SFML_SpriteAnimation::getSpriteSheet()
{
	return m_spritesheet;
}

void SFML_SpriteAnimation::addFrame(sf::IntRect textureRect, sf::Vector2f centre)
{
	m_frameRectangleList.push_back(textureRect);
	m_frameCentreList.push_back(centre);
}

int SFML_SpriteAnimation::getNumberofFrames()
{
	return m_frameRectangleList.size();
}

bool SFML_SpriteAnimation::getCurrentFrame(sf::IntRect* rect, sf::Vector2f* centre)
{
	if ((m_currentFrameNumber < 0) || (m_currentFrameNumber >= m_frameRectangleList.size()))
	{
		// Out of bounds position is supplied
		return false;
	}

	// Retrieve the Rectangle and Centre data
	sf::IntRect irect = m_frameRectangleList[m_currentFrameNumber];
	sf::Vector2f icenter = m_frameCentreList[m_currentFrameNumber];

	// Copy the information over
	rect->left = irect.left;
	rect->top = irect.top;
	rect->width = irect.width;
	rect->height = irect.height;

	centre->x = icenter.x;
	centre->y = icenter.y;

	return true;
}

int SFML_SpriteAnimation::getCurrentFrameNumber()
{
	return m_currentFrameNumber;
}

void SFML_SpriteAnimation::resetCurrentAnimation()
{
	m_currentFrameNumber = 0;
}

void SFML_SpriteAnimation::setFrameTime(float timeperFrame)
{
	m_timeperFrame = timeperFrame;
}

float SFML_SpriteAnimation::getFrameTime()
{
	return m_timeperFrame;
}

float SFML_SpriteAnimation::getTimePerFrame()
{
	return m_timeperFrame;
}

void SFML_SpriteAnimation::incrementFrameNumber()
{
	m_currentFrameNumber++;
	if (m_currentFrameNumber >= m_frameRectangleList.size() - 1)
	{
		if (m_isLoop)
			resetCurrentAnimation();
		else
			m_currentFrameNumber = m_frameRectangleList.size() - 1;
	}
}

bool SFML_SpriteAnimation::isCompleted()
{
	if (m_currentFrameNumber >= m_frameRectangleList.size() - 1)
		return true;
	else
		return false;
}

bool SFML_SpriteAnimation::isLooped() const
{
	return m_isLoop;
}

void SFML_SpriteAnimation::setLoop(bool loop)
{
	m_isLoop = loop;
}

bool SFML_SpriteAnimation::canbeInterrupted() const
{
	return m_canbeInterrupted;
}

void SFML_SpriteAnimation::setCanbeInterrupted(bool trueorfalse)
{
	m_canbeInterrupted = trueorfalse;
}
