#pragma once

#include <SFML/Graphics.hpp>

class SFML_2DCamera
{
public:
	                            SFML_2DCamera(int projectionWindowWidth, int projectionWindowHeight);
	                            ~SFML_2DCamera();

	// Function to get the visible area of the game world
	sf::FloatRect               getCameraWindow(float parallaxFactor);

	// Function to get the virtual camera's position
	void                        setPosition(sf::Vector2f target);

	// Functions to increase and decrease camera's zoom level
	void                        zoomUp(float elapsedTime);
	void                        zoomDown(float elapsedTime);

	// Member functions to get camera view and projection
	sf::Transform               getViewTransform() const;
	sf::Transform               getProjTransform() const;

private:
	// Member attribute to store the visible area width and height
	sf::Vector2f                m_cameraWindowSize;

	// Member attribute to store camera position
	sf::Vector2f                m_cameraPosition;

	// Member attribute to store camera zoom level to emulate height 
	float                       m_cameraZoom;
};

