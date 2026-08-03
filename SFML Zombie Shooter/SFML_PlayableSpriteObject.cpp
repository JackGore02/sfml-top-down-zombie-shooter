#include "SFML_PlayableSpriteObject.h"


SFML_PlayableSpriteObject::SFML_PlayableSpriteObject()
	: m_readyToShoot(true)
	, m_rangeWeaponCooldown(0)
	, m_timeSinceLastShoot(0)
	, m_moveSpeed(200)
	, m_isInMeleeState(false)
	, m_playerHealth(100)
	, m_damageCooldown(true)
	, m_playerAction(IDLE)
	, m_equippedWeapon(FLASHLIGHT)
{
	m_bulletTexture.loadFromFile("Media/Textures/bullet.png");
}


SFML_PlayableSpriteObject::~SFML_PlayableSpriteObject()
{

}

SFML_BulletSpriteObject* SFML_PlayableSpriteObject::processEvents(float elapsedTime, sf::Transform transform, sf::Vector2f mouseScreenPosition)
{

	SFML_BulletSpriteObject* bullet = NULL;

	const float PI = 3.1415f;

	// Aim follows the pointer without requiring a mouse button. This makes aiming
	// usable with a trackpad, where holding a secondary click while clicking is
	// unreliable or impossible.
	bool leftButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool rightButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);

	// get character screen position
	sf::Vector2f charScreenPos = getScreenPosition(transform);

	// calculate vector difference
	sf::Vector2f differenceVector = mouseScreenPosition - charScreenPos;
	double magnitude = sqrt(differenceVector.x * differenceVector.x + differenceVector.y * differenceVector.y);

	if (magnitude > 0.001)
	{
		// calculate angle between the vector and the horizontal +ve x axis
		// horizontal positive x-axis is defined 0 degree, sprite facing right initially
		double angleinRadians = atan2(differenceVector.y, differenceVector.x);
		double angleinDegrees = angleinRadians / PI * 180;
		setRotation(static_cast<float>(angleinDegrees));
	}

	// Primary click attacks with the equipped weapon. Secondary click is always
	// melee, giving one- and two-finger trackpad clicks useful standalone actions.
	if (rightButtonPressed || (leftButtonPressed &&
		(m_equippedWeapon == FLASHLIGHT || m_equippedWeapon == KNIFE)))
		toMeleeState();
	else if (leftButtonPressed)
		bullet = shoot();

	float faceAngle = getRotation();

	sf::Vector2f moveDirection(0.f, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		float moveAngle = (faceAngle - 90) / 180 * PI;
		moveDirection += sf::Vector2f(cos(moveAngle), sin(moveAngle));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		float moveAngle = (faceAngle + 90) / 180 * PI;
		moveDirection += sf::Vector2f(cos(moveAngle), sin(moveAngle));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		float moveAngle = faceAngle / 180 * PI;
		moveDirection += sf::Vector2f(cos(moveAngle), sin(moveAngle));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		float moveAngle = (faceAngle + 180) / 180 * PI;
		moveDirection += sf::Vector2f(cos(moveAngle), sin(moveAngle));
	}

	float moveMagnitude = sqrt(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y);
	if (moveMagnitude > 0.001f)
	{
		moveDirection /= moveMagnitude;
		toWalkingState();
		move(moveDirection * m_moveSpeed * elapsedTime);
	}
	else
	{
		toIdleState();
	}

	if (m_rangeWeaponCooldown == 0)
		m_readyToShoot = false;
	else
	{
		m_timeSinceLastShoot += elapsedTime;
		if (m_timeSinceLastShoot >= m_rangeWeaponCooldown)
		{
			m_readyToShoot = true;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		equipFlashlight();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		equipKnife();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		equipPistol();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		equipRifle();
	}

	return bullet;
}

void SFML_PlayableSpriteObject::update(float elapsedTime)
{
	SFML_AnimatedSpriteObject::update(elapsedTime);
}

int	SFML_PlayableSpriteObject::setFlashlightIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_flashlightIdleAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_flashlightIdleAnimationID;
}

int	SFML_PlayableSpriteObject::setFlashlightWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_flashlightWalkingAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_flashlightWalkingAnimationID;
}

int	SFML_PlayableSpriteObject::setFlashlightMeleeAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_flashlightMeleeAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	m_animatedSpriteList[m_flashlightMeleeAnimationID]->setLoop(false);
	m_animatedSpriteList[m_flashlightMeleeAnimationID]->setCanbeInterrupted(false);
	return m_flashlightMeleeAnimationID;
}

int	SFML_PlayableSpriteObject::setKnifeIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_knifeIdleAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_knifeIdleAnimationID;
}

int	SFML_PlayableSpriteObject::setKnifeWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_knifeWalkingAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_knifeWalkingAnimationID;
}

int	SFML_PlayableSpriteObject::setKnifeMeleeAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_knifeMeleeAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	m_animatedSpriteList[m_knifeMeleeAnimationID]->setLoop(false);
	m_animatedSpriteList[m_knifeMeleeAnimationID]->setCanbeInterrupted(false);
	return m_knifeMeleeAnimationID;
}

int SFML_PlayableSpriteObject::setPistolIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_pistolIdleAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_pistolIdleAnimationID;
}

int SFML_PlayableSpriteObject::setPistolWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_pistolWalkingAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_pistolWalkingAnimationID;
}

int	SFML_PlayableSpriteObject::setPistolMeleeAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_pistolMeleeAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	m_animatedSpriteList[m_pistolMeleeAnimationID]->setLoop(false);
	m_animatedSpriteList[m_pistolMeleeAnimationID]->setCanbeInterrupted(false);
	return m_pistolMeleeAnimationID;
}

int	SFML_PlayableSpriteObject::setPistolShootAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_pistolShootAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	m_animatedSpriteList[m_pistolShootAnimationID]->setLoop(false);
	m_animatedSpriteList[m_pistolShootAnimationID]->setCanbeInterrupted(false);
	return m_pistolShootAnimationID;
}

int	SFML_PlayableSpriteObject::setRifleIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_rifleIdleAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_rifleIdleAnimationID;
}

int	SFML_PlayableSpriteObject::setRifleWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_rifleWalkingAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_rifleWalkingAnimationID;
}

int	SFML_PlayableSpriteObject::setRifleMeleeAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_rifleMeleeAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	m_animatedSpriteList[m_rifleMeleeAnimationID]->setLoop(false);
	m_animatedSpriteList[m_rifleMeleeAnimationID]->setCanbeInterrupted(false);
	return m_rifleMeleeAnimationID;
}

int	SFML_PlayableSpriteObject::setRifleShootAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_rifleShootAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	m_animatedSpriteList[m_rifleShootAnimationID]->setLoop(false);
	m_animatedSpriteList[m_rifleShootAnimationID]->setCanbeInterrupted(false);
	return m_rifleShootAnimationID;
}

void SFML_PlayableSpriteObject::equipFlashlight()
{
	if (m_equippedWeapon == FLASHLIGHT && m_currentAnimation)
		return;
	m_playerAction = IDLE;
	m_isInMeleeState = false;
	setCurrentAnimation(m_flashlightIdleAnimationID);
	m_equippedWeapon = FLASHLIGHT;
}

void SFML_PlayableSpriteObject::equipKnife()
{
	if (m_equippedWeapon == KNIFE && m_currentAnimation)
		return;
	m_playerAction = IDLE;
	m_isInMeleeState = false;
	setCurrentAnimation(m_knifeIdleAnimationID);
	m_equippedWeapon = KNIFE;
}

void SFML_PlayableSpriteObject::equipPistol()
{
	if (m_equippedWeapon == PISTOL && m_currentAnimation)
		return;
	m_playerAction = IDLE;
	m_isInMeleeState = false;
	setCurrentAnimation(m_pistolIdleAnimationID);
	m_equippedWeapon = PISTOL;
	m_rangeWeaponCooldown = 1;
	m_readyToShoot = true;
}

void SFML_PlayableSpriteObject::equipRifle()
{
	if (m_equippedWeapon == RIFLE && m_currentAnimation)
		return;
	m_playerAction = IDLE;
	m_isInMeleeState = false;
	setCurrentAnimation(m_rifleIdleAnimationID);
	m_equippedWeapon = RIFLE;
	m_rangeWeaponCooldown = 3;
	m_readyToShoot = true;
}

bool SFML_PlayableSpriteObject::toIdleState()
{

	if (m_currentAnimation->canbeInterrupted() || m_currentAnimation->isCompleted())
	{
		m_isInMeleeState = false;
		switch (m_equippedWeapon)
		{
		case FLASHLIGHT:
		{
			m_playerAction = IDLE;
			setCurrentAnimation(m_flashlightIdleAnimationID);
			break;
		}

		case KNIFE:
		{
			m_playerAction = IDLE;
			setCurrentAnimation(m_knifeIdleAnimationID);
			break;
		}

		case PISTOL:
		{
			m_playerAction = IDLE;
			setCurrentAnimation(m_pistolIdleAnimationID);
			break;
		}

		case RIFLE:
		{
			m_playerAction = IDLE;
			setCurrentAnimation(m_rifleIdleAnimationID);
			break;
		}

		default:
			break;
		}

		return true;
	}

	return false;
}

bool SFML_PlayableSpriteObject::toWalkingState()
{

	if (m_currentAnimation->canbeInterrupted() || m_currentAnimation->isCompleted())
	{
		m_isInMeleeState = false;
		switch (m_equippedWeapon)
		{
		case FLASHLIGHT:
		{
			m_playerAction = WALKING;
			setCurrentAnimation(m_flashlightWalkingAnimationID);
			break;
		}

		case KNIFE:
		{
			m_playerAction = WALKING;
			setCurrentAnimation(m_knifeWalkingAnimationID);
			break;
		}

		case PISTOL:
		{
			m_playerAction = WALKING;
			setCurrentAnimation(m_pistolWalkingAnimationID);
			break;
		}

		case RIFLE:
		{
			m_playerAction = WALKING;
			setCurrentAnimation(m_rifleWalkingAnimationID);
			break;
		}

		default:
			break;
		}

		return true;
	}
	else

		return false;
}

bool SFML_PlayableSpriteObject::toMeleeState()
{
	if (m_currentAnimation->canbeInterrupted() || m_currentAnimation->isCompleted())
	{
		switch (m_equippedWeapon)
		{
		case FLASHLIGHT:
		{
			m_playerAction = MELEE;
			setCurrentAnimation(m_flashlightMeleeAnimationID);
			m_isInMeleeState = true;
			break;
		}

		case KNIFE:
		{
			m_playerAction = MELEE;
			setCurrentAnimation(m_knifeMeleeAnimationID);
			m_isInMeleeState = true;
			break;
		}

		case PISTOL:
		{
			m_playerAction = MELEE;
			setCurrentAnimation(m_pistolMeleeAnimationID);
			m_isInMeleeState = true;
			break;
		}

		case RIFLE:
		{
			m_playerAction = MELEE;
			setCurrentAnimation(m_rifleMeleeAnimationID);
			m_isInMeleeState = true;
			break;
		}

		default:
			break;
		}

		return true;
	}
	else
		m_isInMeleeState = false;
		return false;
}

bool SFML_PlayableSpriteObject::toShootState()
{

	if (m_currentAnimation->canbeInterrupted() || m_currentAnimation->isCompleted())
	{
		m_isInMeleeState = false;
		switch (m_equippedWeapon)
		{
		case FLASHLIGHT:
			return false;

		case KNIFE:
			return false;

		case PISTOL:
		{
			m_playerAction = SHOOT;
			setCurrentAnimation(m_pistolShootAnimationID);
		}

		return true;

		case RIFLE:
		{
			m_playerAction = SHOOT;
			setCurrentAnimation(m_rifleShootAnimationID);
		}

		return true;

		default:
			return false;
		}
	}

	else
		return false;
}

void SFML_PlayableSpriteObject::setSpeed(float speed)
{
	m_moveSpeed = speed;
}

SFML_BulletSpriteObject* SFML_PlayableSpriteObject::shoot()
{
	if (!m_readyToShoot)
		return NULL;

	// Check if current weapon and state allows changing to shoot state
	if (toShootState())
	{
		SFML_BulletSpriteObject*	bullet = new SFML_BulletSpriteObject();
		bullet->applyTexture(&m_bulletTexture);
		bullet->setPosition(getWorldPosition());
		bullet->setMaxRange(1000);
		bullet->setSpeed(5000);

		const float PI = 3.14;
		float angle = getRotation() / 180 * PI;
		sf::Vector2f faceDirection = sf::Vector2f(cos(angle), sin(angle));
		bullet->setMoveDirection(faceDirection, true);

		m_timeSinceLastShoot = 0;
		m_readyToShoot = false;

		return bullet;
	}
	else
		return NULL;

}

bool SFML_PlayableSpriteObject::isInMeleeState()
{
	return m_playerAction == MELEE && m_currentAnimation && !m_currentAnimation->isCompleted();
}

int SFML_PlayableSpriteObject::getPlayerHealth()
{
	return m_playerHealth;
}

void SFML_PlayableSpriteObject::setPlayerHealth(int damage)
{
	m_playerHealth = std::max(0, m_playerHealth - damage);
}

bool SFML_PlayableSpriteObject::getDamageCooldown()
{
	return m_damageCooldown;
}

void SFML_PlayableSpriteObject::setDamageCooldown(bool damageable)
{
	m_damageCooldown = damageable;
}
