#include "SFML_GameWorld.h"

SFML_GameWorld::SFML_GameWorld(int windowWidth, int windowHeight, sf::RenderWindow* parentWindow)
	: m_windowWidth(windowWidth)
	, m_windowHeight(windowHeight)
	, m_camera(windowWidth, windowHeight)
	, m_playAnimation(false)
	, m_playerDamageCooldownRemaining(0.f)
{
	// Create a gameworld layer object using the wall.png texture
	SFML_GameWorldLayer* background = new SFML_GameWorldLayer("Media/Textures/wall.png", 1.0f, sf::Vector2f(0, 0), windowWidth, windowHeight);

	// Insert it into the array
	m_gameWorldLayerList.push_back(background);

	// Create a gameworld layer object using the fog.png texture 
	SFML_GameWorldLayer* middleground = new SFML_GameWorldLayer("Media/Textures/fog.png", 0.8f, sf::Vector2f(100, 100), windowWidth, windowHeight);

	// Insert it into the array
	m_gameWorldLayerList.push_back(middleground);

	// Create a gameworld layer object using the wire.png texture 
	SFML_GameWorldLayer* foreground = new SFML_GameWorldLayer("Media/Textures/wire.png", 0.3f, sf::Vector2f(0, 0), windowWidth, windowHeight);

	// Insert it into the array
	m_gameWorldLayerList.push_back(foreground);

	// Sort the game world in descending order
	sort(m_gameWorldLayerList.begin(), m_gameWorldLayerList.end(), layerSortFunction);

	// Set the camera's initial position to (0, 0)
	m_cameraPosition = sf::Vector2f(0, 0);

	// Set default texture using solider.png image
	m_spriteObject.setDefaultTexture("Media/Textures/soldier.png", "Media/SpriteInfo/soldier.txt");

	// Set position to (0, 0)
	m_spriteObject.setPosition(0, 0);

	// Store the address of the parent render window in m_parentWindow
	m_parentWindow = parentWindow;

	m_animatedObject = new SFML_AnimatedSpriteObject();
	m_idleAnimationID = m_animatedObject->addAnimation("Media/Textures/blood.png", "Media/SpriteInfo/blood.txt", 1.0f);
	m_animatedObject->setCurrentAnimation(m_idleAnimationID);
	m_animatedObject->getCurrentAnimation()->setLoop(false);

	m_numberofZombies = 10;
	for (int i = 0; i < m_numberofZombies; i++)
	{
		SFML_NPCSpriteObject* npcSprite = new SFML_NPCSpriteObject();
		npcSprite->setIdleAnimation("Media/Textures/zombie-idle.png", "Media/SpriteInfo/zombie-idle.txt", 1.0f);
		npcSprite->setWalkingAnimation("Media/Textures/zombie-move.png", "Media/SpriteInfo/zombie-move.txt", 1.0f);
		npcSprite->setAttackingAnimation("Media/Textures/zombie-attack.png", "Media/SpriteInfo/zombie-attack.txt", 1.0f);

		npcSprite->setPosition(rand() % 4000 - 2000, rand() % 4000 - 2000);
		npcSprite->setTargetLocation(sf::Vector2f(rand() % 4000 - 2000, rand() % 4000 - 2000));
		m_zombieCharacterList.push_back(npcSprite);
	}

	m_playerCharacter = new SFML_PlayableSpriteObject();
	m_playerCharacter->setFlashlightIdleAnimation("Media/Textures/hero_flashlight_idle.png", "Media/SpriteInfo/hero_flashlight_idle.txt", 1.0f);
	m_playerCharacter->setFlashlightWalkingAnimation("Media/Textures/hero_flashlight_move.png", "Media/SpriteInfo/hero_flashlight_move.txt", 1.0f);
	m_playerCharacter->setFlashlightMeleeAnimation("Media/Textures/hero_flashlight_melee.png", "Media/SpriteInfo/hero_flashlight_melee.txt", 1.0f);

	m_playerCharacter->setKnifeIdleAnimation("Media/Textures/hero_knife_idle.png", "Media/SpriteInfo/hero_knife_idle.txt", 1.0f);
	m_playerCharacter->setKnifeWalkingAnimation("Media/Textures/hero_knife_move.png", "Media/SpriteInfo/hero_knife_move.txt", 1.0f);
	m_playerCharacter->setKnifeMeleeAnimation("Media/Textures/hero_knife_melee.png", "Media/SpriteInfo/hero_knife_melee.txt", 1.0f);

	m_playerCharacter->setPistolIdleAnimation("Media/Textures/hero_pistol_idle.png", "Media/SpriteInfo/hero_pistol_idle.txt", 1.0f);
	m_playerCharacter->setPistolWalkingAnimation("Media/Textures/hero_pistol_move.png", "Media/SpriteInfo/hero_pistol_move.txt", 1.0f);
	m_playerCharacter->setPistolMeleeAnimation("Media/Textures/hero_pistol_melee.png", "Media/SpriteInfo/hero_pistol_melee.txt", 1.0f);
	m_playerCharacter->setPistolShootAnimation("Media/Textures/hero_pistol_shoot.png", "Media/SpriteInfo/hero_pistol_shoot.txt", 1.0f);

	m_playerCharacter->setRifleIdleAnimation("Media/Textures/hero_rifle_idle.png", "Media/SpriteInfo/hero_rifle_idle.txt", 1.0f);
	m_playerCharacter->setRifleWalkingAnimation("Media/Textures/hero_rifle_move.png", "Media/SpriteInfo/hero_rifle_move.txt", 1.0f);
	m_playerCharacter->setRifleMeleeAnimation("Media/Textures/hero_rifle_melee.png", "Media/SpriteInfo/hero_rifle_melee.txt", 1.0f);
	m_playerCharacter->setRifleShootAnimation("Media/Textures/hero_rifle_shoot.png", "Media/SpriteInfo/hero_rifle_shoot.txt", 1.0f);

	m_playerCharacter->equipFlashlight();
	m_playerCharacter->toIdleState();

	m_playerCharacter->setPosition(0, 0);

	m_fullHealthTexture.loadFromFile("Media/HUD/health-bar-full.png");
	m_fullHealthSprite.setTexture(m_fullHealthTexture);
	sf::Vector2u fullHealthTextureSize = m_fullHealthTexture.getSize();
	m_fullHealthSprite.setTextureRect(sf::IntRect(0, 0, fullHealthTextureSize.x, fullHealthTextureSize.y));
	m_fullHealthSprite.setOrigin(fullHealthTextureSize.x / 2, fullHealthTextureSize.y / 2);
	m_fullHealthSprite.setPosition(170, 280);
	m_fullHealthSprite.setScale(3.f, 3.f);

	m_halfHealthTexture.loadFromFile("Media/HUD/health-bar-half.png");
	m_halfHealthSprite.setTexture(m_halfHealthTexture);
	sf::Vector2u halfHealthTextureSize = m_halfHealthTexture.getSize();
	m_halfHealthSprite.setTextureRect(sf::IntRect(0, 0, halfHealthTextureSize.x, halfHealthTextureSize.y));
	m_halfHealthSprite.setOrigin(halfHealthTextureSize.x / 2, halfHealthTextureSize.y / 2);
	m_halfHealthSprite.setPosition(120, 280);
	m_halfHealthSprite.setScale(3.f, 3.f);

	m_quarterHealthTexture.loadFromFile("Media/HUD/health-bar-quarter.png");
	m_quarterHealthSprite.setTexture(m_quarterHealthTexture);
	sf::Vector2u quarterHealthTextureSize = m_quarterHealthTexture.getSize();
	m_quarterHealthSprite.setTextureRect(sf::IntRect(0, 0, quarterHealthTextureSize.x, quarterHealthTextureSize.y));
	m_quarterHealthSprite.setOrigin(quarterHealthTextureSize.x / 2, quarterHealthTextureSize.y / 2);
	m_quarterHealthSprite.setPosition(75, 280);
	m_quarterHealthSprite.setScale(3.f, 3.f);

	m_deadHealthTexture.loadFromFile("Media/HUD/health-bar-dead.png");
	m_deadHealthSprite.setTexture(m_deadHealthTexture);
	sf::Vector2u deadHealthTextureSize = m_deadHealthTexture.getSize();
	m_deadHealthSprite.setTextureRect(sf::IntRect(0, 0, deadHealthTextureSize.x, deadHealthTextureSize.y));
	m_deadHealthSprite.setOrigin(deadHealthTextureSize.x / 2, deadHealthTextureSize.y / 2);
	m_deadHealthSprite.setPosition(170, 280);
	m_deadHealthSprite.setScale(3.f, 3.f);

	m_healthSprite = m_fullHealthSprite;

	m_borderTexture.loadFromFile("Media/HUD/border.png");
	m_borderSprite.setTexture(m_borderTexture);
	sf::Vector2u bkTextureSize = m_borderTexture.getSize();
	m_borderSprite.setTextureRect(sf::IntRect(0, 0, bkTextureSize.x, bkTextureSize.y));
	m_borderSprite.setColor(sf::Color(255, 255, 255, 200));
	m_borderSprite.setPosition(0, 0);

	m_flashlightTexture.loadFromFile("Media/HUD/flashlight-icon.png");
	m_flashlightSprite.setTexture(m_flashlightTexture);
	sf::Vector2u flashlightTextureSize = m_flashlightTexture.getSize();
	m_flashlightSprite.setTextureRect(sf::IntRect(0, 0, flashlightTextureSize.x, flashlightTextureSize.y));
	m_flashlightSprite.setOrigin(flashlightTextureSize.x / 2, flashlightTextureSize.y / 2);
	m_flashlightSprite.setPosition(170, 900);
	m_flashlightSprite.setScale(.25f, .25f);

	m_knifeTexture.loadFromFile("Media/HUD/knife-icon.png");
	m_knifeSprite.setTexture(m_knifeTexture);
	sf::Vector2u knifeTextureSize = m_knifeTexture.getSize();
	m_knifeSprite.setTextureRect(sf::IntRect(0, 0, knifeTextureSize.x, knifeTextureSize.y));
	m_knifeSprite.setOrigin(knifeTextureSize.x / 2, knifeTextureSize.y / 2);
	m_knifeSprite.setPosition(170, 900);
	m_knifeSprite.setScale(.2f, .2f);

	m_pistolTexture.loadFromFile("Media/HUD/pistol-icon.png");
	m_pistolSprite.setTexture(m_pistolTexture);
	sf::Vector2u pistolTextureSize = m_pistolTexture.getSize();
	m_pistolSprite.setTextureRect(sf::IntRect(0, 0, pistolTextureSize.x, pistolTextureSize.y));
	m_pistolSprite.setOrigin(pistolTextureSize.x / 2, pistolTextureSize.y / 2);
	m_pistolSprite.setPosition(170, 900);
	m_pistolSprite.setScale(.25f, .25f);

	m_rifleTexture.loadFromFile("Media/HUD/rifle-icon.png");
	m_rifleSprite.setTexture(m_rifleTexture);
	sf::Vector2u rifleTextureSize = m_rifleTexture.getSize();
	m_rifleSprite.setTextureRect(sf::IntRect(0, 0, rifleTextureSize.x, rifleTextureSize.y));
	m_rifleSprite.setOrigin(rifleTextureSize.x / 2, rifleTextureSize.y / 2);
	m_rifleSprite.setPosition(170, 900);
	m_rifleSprite.setScale(1.f, 1.f);

	m_iconSprite = m_flashlightSprite;

	m_portraitTexture.loadFromFile("Media/HUD/soldier-portrait.jpg");
	m_portraitSprite.setTexture(m_portraitTexture);
	sf::Vector2u portraitTextureSize = m_portraitTexture.getSize();
	m_portraitSprite.setTextureRect(sf::IntRect(0, 0, portraitTextureSize.x, portraitTextureSize.y));
	m_portraitSprite.setOrigin(portraitTextureSize.x / 2, portraitTextureSize.y / 2);
	m_portraitSprite.setPosition(175, 170);
	m_portraitSprite.setScale(.5f, .5f);

	
}

SFML_GameWorld::~SFML_GameWorld()
{
	// Clear the GameWorldLayer list
	for (vector<SFML_GameWorldLayer*>::iterator it = m_gameWorldLayerList.begin(); it != m_gameWorldLayerList.end(); it++)
	{
		delete (*it);
	}
	m_gameWorldLayerList.clear();

	// Clear the memory space used for the animated object
	delete m_animatedObject;
	delete m_playerCharacter;
	for (SFML_NPCSpriteObject* zombie : m_zombieCharacterList)
		delete zombie;
	for (SFML_BulletSpriteObject* bullet : m_bulletList)
		delete bullet;
}

void SFML_GameWorld::processEvents(float elapsedTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
	{
		m_camera.zoomUp(elapsedTime);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
	{
		m_camera.zoomDown(elapsedTime);
	}

	// Get the camera transform (world -> screen)
	sf::Transform transform;
	transform = m_camera.getProjTransform() * m_camera.getViewTransform();

	// Take inverse (screen -> world)
	sf::Transform inv = transform.getInverse();

	// Get mouse position (in screen space)
	sf::Vector2f mouseScreenPosition = sf::Vector2f(sf::Mouse::getPosition(*m_parentWindow));

	// Convert to world space coordinate
	sf::Vector2f wp = inv.transformPoint(mouseScreenPosition);

	// Get sprite global bounding rectangle
	sf::FloatRect rect = m_spriteObject.getGlobalBounds();

	if (rect.contains(wp))
	{
		// If mouse pointer inside the bounding rectangle, tint the sprite red
		m_spriteObject.setColor(sf::Color(255, 0, 0));
	}
	else
	{
		// Otherwise revert back to normal colour (white)
		m_spriteObject.setColor(sf::Color(255, 255, 255));
	}

	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		if (m_zombieCharacterList[counter]->isInVision(m_playerCharacter->getPosition()) || m_zombieCharacterList[counter]->isInCircle(m_playerCharacter->getPosition(), 400))
		{
			// The player is within the NPC's vision cone 
			m_zombieCharacterList[counter]->setColor(sf::Color(128, 0, 0));
		}
		else
		{
			// The player is outside the NPC's vision cone
			m_zombieCharacterList[counter]->setColor(sf::Color(255, 255, 255));
		}
	}

	SFML_BulletSpriteObject* bullet = m_playerCharacter->processEvents(elapsedTime, transform, mouseScreenPosition);
	if (bullet)
		m_bulletList.push_back(bullet);

	if (m_playerDamageCooldownRemaining > 0.f)
		m_playerDamageCooldownRemaining = std::max(0.f, m_playerDamageCooldownRemaining - elapsedTime);

	// Collision detection between zombie and player
	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		if (m_zombieCharacterList[counter]->getGlobalBounds().intersects(m_playerCharacter->getGlobalBounds()) && m_zombieCharacterList[counter]->isInAttackingState())
		{
			if (m_playerDamageCooldownRemaining <= 0.f)
			{
				m_playerCharacter->setPlayerHealth(25);
				m_playerDamageCooldownRemaining = 0.75f;
				break;
			}
		}
	}
}

void SFML_GameWorld::update(float elapsedTime)
{
	processEvents(elapsedTime);
	// Update the sprite object
	m_spriteObject.update(elapsedTime);
	m_camera.setPosition(m_cameraPosition);
	m_animatedObject->update(elapsedTime);
	if (m_playAnimation && m_animatedObject->getCurrentAnimation()->isCompleted())
		m_playAnimation = false;
	m_playerCharacter->update(elapsedTime);
	m_camera.setPosition(m_playerCharacter->getWorldPosition());

	for (int counter = 0; counter < m_gameWorldLayerList.size(); counter++)
	{
		m_gameWorldLayerList[counter]->followCamera(&m_camera);
		m_gameWorldLayerList[counter]->update(elapsedTime);
	}

	// Update each zombie object in the list
	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		m_zombieCharacterList[counter]->update(elapsedTime, m_playerCharacter->getPosition());
	}

	// Update all bullets
	for (unsigned int counter = 0; counter < m_bulletList.size(); counter++)
	{
		m_bulletList[counter]->update(elapsedTime);
	}

	vector<SFML_BulletSpriteObject*> bulletsToRemove;
	vector<SFML_NPCSpriteObject*> zombiesToRemove;
	for (unsigned int counter1 = 0; counter1 < m_bulletList.size(); counter1++)
	{
		// Check if bullet is active
		if (m_bulletList[counter1]->isActive())
		{
			for (unsigned int counter2 = 0; counter2 < m_zombieCharacterList.size(); counter2++)
			{
				if (m_bulletList[counter1]->getGlobalBounds().intersects(m_zombieCharacterList[counter2]->getGlobalBounds()))
				{
					cout << "Bullet Collision Detected" << endl;

					m_animatedObject->setPosition(m_zombieCharacterList[counter2]->getPosition());
					m_animatedObject->getCurrentAnimation()->resetCurrentAnimation();
					m_animatedObject->play();
					m_playAnimation = true;
					bulletsToRemove.push_back(m_bulletList[counter1]);
					if (find(zombiesToRemove.begin(), zombiesToRemove.end(), m_zombieCharacterList[counter2]) == zombiesToRemove.end())
						zombiesToRemove.push_back(m_zombieCharacterList[counter2]);
					break;
				}
			}
		}
	}

	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		if (m_zombieCharacterList[counter]->getGlobalBounds().intersects(m_playerCharacter->getGlobalBounds()) && m_playerCharacter->isInMeleeState())
		{
			cout << "Melee Collision Detected" << endl;

			m_animatedObject->setPosition(m_zombieCharacterList[counter]->getPosition());
			m_animatedObject->getCurrentAnimation()->resetCurrentAnimation();
			m_animatedObject->play();
			m_playAnimation = true;

			if (find(zombiesToRemove.begin(), zombiesToRemove.end(), m_zombieCharacterList[counter]) == zombiesToRemove.end())
				zombiesToRemove.push_back(m_zombieCharacterList[counter]);
		}
	}

	for (SFML_BulletSpriteObject* bullet : m_bulletList)
	{
		if (!bullet->isActive() && find(bulletsToRemove.begin(), bulletsToRemove.end(), bullet) == bulletsToRemove.end())
			bulletsToRemove.push_back(bullet);
	}

	

	// Remove bullets and zombies from the game world
	for (unsigned int i = 0; i < bulletsToRemove.size(); i++)
	{
		m_bulletList.erase(remove(m_bulletList.begin(), m_bulletList.end(), bulletsToRemove[i]), m_bulletList.end());
		delete bulletsToRemove[i];
	}

	for (unsigned int i = 0; i < zombiesToRemove.size(); i++)
	{
		m_zombieCharacterList.erase(remove(m_zombieCharacterList.begin(), m_zombieCharacterList.end(), zombiesToRemove[i]), m_zombieCharacterList.end());
		delete zombiesToRemove[i];
	}

	if (m_playerCharacter->getPlayerHealth() > 50)
	{
		m_healthSprite = m_fullHealthSprite;
	}
	else if (m_playerCharacter->getPlayerHealth() <= 50 && m_playerCharacter->getPlayerHealth() > 25)
	{
		m_healthSprite = m_halfHealthSprite;
	}
	else if (m_playerCharacter->getPlayerHealth() <= 25 && m_playerCharacter->getPlayerHealth() > 0)
	{
		m_healthSprite = m_quarterHealthSprite;
	}
	else if (m_playerCharacter->getPlayerHealth() <= 0)
	{
		m_healthSprite = m_deadHealthSprite;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		m_iconSprite = m_flashlightSprite;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		m_iconSprite = m_knifeSprite;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		m_iconSprite = m_pistolSprite;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		m_iconSprite = m_rifleSprite;
	}
}

void SFML_GameWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Render each game world layer with parallax factor >= 1.0
	for (unsigned int counter = 0; counter < m_gameWorldLayerList.size(); counter++)
	{
		if (m_gameWorldLayerList[counter]->getParallaxFactor() >= 1.0f)
			target.draw(*m_gameWorldLayerList[counter]);
	}

	// Get the camera view-projection transform and set it to the renderstate
	sf::RenderStates renderState;
	renderState.transform = m_camera.getProjTransform() * m_camera.getViewTransform();

	target.draw(*m_playerCharacter, renderState);

	if (m_playAnimation)
	{
		target.draw(*m_animatedObject, renderState);
	}

	// Draw each bullet object in the list
	for (unsigned int counter = 0; counter < m_bulletList.size(); counter++)
	{
		target.draw(*m_bulletList[counter], renderState);
	}

	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		target.draw(*m_zombieCharacterList[counter], renderState);
	}

	// Render each game world layer with parallax factor < 1.0
	for (unsigned int counter = 0; counter < m_gameWorldLayerList.size(); counter++)
	{
		if (m_gameWorldLayerList[counter]->getParallaxFactor() < 1.0f)
			target.draw(*m_gameWorldLayerList[counter]);
	}

	// Draw a semi-transparent black rectangle over the entire screen to simulate night time
	sf::RectangleShape tint(sf::Vector2f(m_windowWidth, m_windowHeight));
	tint.setFillColor(sf::Color(0, 0, 0, 150));
	target.draw(tint);

	target.draw(m_borderSprite);

	target.draw(m_healthSprite);

	target.draw(m_iconSprite);

	target.draw(m_portraitSprite);

	
	
}

bool SFML_GameWorld::layerSortFunction(SFML_GameWorldLayer* a, SFML_GameWorldLayer* b)
{
	return (a->getParallaxFactor() > b->getParallaxFactor());
}



