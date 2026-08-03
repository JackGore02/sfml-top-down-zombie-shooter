#include "SFML_NPCSpriteObject.h"


SFML_NPCSpriteObject::SFML_NPCSpriteObject()
	: m_targetLocation(0, 0)
	, m_moveSpeed(100.0f)
	, m_state(IDLE)
	, m_idleAnimationID(-1)
	, m_walkingAnimationID(-1)
	, m_attackingAnimationID(-1)
	, m_visionConeAngle(90.0f)
	, m_visionRange(1200.0f)
	, m_isInAttackingState(false)
{

}

SFML_NPCSpriteObject::~SFML_NPCSpriteObject()
{
}

int SFML_NPCSpriteObject::setIdleAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_idleAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_idleAnimationID;
}

int	SFML_NPCSpriteObject::setWalkingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_walkingAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	return m_walkingAnimationID;
}

int SFML_NPCSpriteObject::setAttackingAnimation(std::string spriteSheetFilename, std::string spriteInfoFilename, float animationTime)
{
	m_attackingAnimationID = addAnimation(spriteSheetFilename, spriteInfoFilename, animationTime);
	m_animatedSpriteList[m_attackingAnimationID]->setLoop(false);
	m_animatedSpriteList[m_attackingAnimationID]->setCanbeInterrupted(false);
	return m_attackingAnimationID;
}

void SFML_NPCSpriteObject::toIdleState()
{
	m_state = IDLE;
	m_isInAttackingState = false;
	setCurrentAnimation(m_idleAnimationID);
}

void SFML_NPCSpriteObject::toWalkingState()
{
	m_state = WALKING;
	m_isInAttackingState = false;
	setCurrentAnimation(m_walkingAnimationID);
}

void SFML_NPCSpriteObject::toAttackingState()
{
	// Rewind a completed attack while the player remains in range. 
	if (m_state == ATTACKING && m_currentAnimation)
	{
		if (m_currentAnimation->isCompleted())
			restart();

		m_isInAttackingState = true;
		return;
	}

	if (m_currentAnimation->canbeInterrupted() || m_currentAnimation->isCompleted())
	{
		m_state = ATTACKING;
		setCurrentAnimation(m_attackingAnimationID);
		m_isInAttackingState = true;
	}
	else
		m_isInAttackingState = false;
}

void SFML_NPCSpriteObject::setTargetLocation(sf::Vector2f targetLocation)
{
	m_targetLocation = targetLocation;
}

void SFML_NPCSpriteObject::setMovementSpeed(float speed)
{
	m_moveSpeed = speed;
}

void SFML_NPCSpriteObject::update(float elapsedTime, sf::Vector2f playerPosition)
{
	const float PI = 3.1415f;

	// Get the NPC's current location 
	sf::Vector2f currentPosition = getPosition();

	// Check if the player is within vision range
	if (isInVision(playerPosition) || isInCircle(playerPosition, 400))
	{
		// Calculate the vector from the NPC's current position to the target point
		sf::Vector2f faceDirection = playerPosition - currentPosition;

		// Calculate the distance (L2-norm) between the current location and the target position
		float distance = sqrt(faceDirection.x * faceDirection.x + faceDirection.y * faceDirection.y);

		// If the distance is less than the attack distance
		float attackDistance = 200.f;
		if (distance < attackDistance)
		{
			// NPC has reached its attack range
			toAttackingState();
		}
		// Otherwise if the distance is less than the displacement to cover in this frame
		else if (distance < elapsedTime * m_moveSpeed)
		{
			// That means this NPC has reached its destination point
			setPosition(playerPosition);

			toAttackingState();
		}
		else
		{
			// Otherwise it needs to keep moving 

			// Rotate the NPC to face the target rotation
			float angle = atan2(faceDirection.y, faceDirection.x);
			setRotation(angle / PI * 180);

			// Calculate the movement vector
			sf::Vector2f unitMoveVector = sf::Vector2f(cos(angle), sin(angle));
			sf::Vector2f totalMoveVector = unitMoveVector * elapsedTime * m_moveSpeed;
			move(totalMoveVector);

			// Set the animation to walking
			toWalkingState();
		}
	}
	else
	{
		// Calculate the vector from the NPC's current position to the target point
		sf::Vector2f faceDirection = m_targetLocation - currentPosition;

		// Calculate the distance (L2-norm) between the current location and the target position
		float distance = sqrt(faceDirection.x * faceDirection.x + faceDirection.y * faceDirection.y);

		// If the distance is less than the displacement to cover in this frame
		if (distance < elapsedTime * m_moveSpeed)
		{
			// That means the NPC has reached its destination point
			setPosition(m_targetLocation);

			// Set the animation to idle
			toIdleState();
		}
		else
		{
			// Otherwise it needs to keep moving 

			// Rotate the NPC to face the target rotation
			float angle = atan2(faceDirection.y, faceDirection.x);
			setRotation(angle / PI * 180);

			// Calculate the movement vector
			sf::Vector2f unitMoveVector = sf::Vector2f(cos(angle), sin(angle));
			sf::Vector2f totalMoveVector = unitMoveVector * elapsedTime * m_moveSpeed;
			move(totalMoveVector);

			// Set the animation to walking
			toWalkingState();
		}
	}

	SFML_AnimatedSpriteObject::update(elapsedTime);
}

bool SFML_NPCSpriteObject::isInVision(sf::Vector2f targetpoint)
{
	const float PI = 3.1415f;

	// Get the NPC's current location
	sf::Vector2f currentPosition = getPosition();

	// Calculate the vector from the NPC's current position to the target point
	sf::Vector2f vector = targetpoint - currentPosition;

	// Calculate the distance (L2-norm) between the current location and the target position
	float distance = sqrt(vector.x * vector.x + vector.y * vector.y);

	// Check if the distance is outside the vision range
	if (distance > m_visionRange)
		return false;

	// Get the NPC's orientation. Note that getRotation() always produces the angle in degrees and
	// in the 0 - 360 range
	float faceAngle = getRotation();

	// Calculate the angle of the vector 
	float targetAngle = atan2(vector.y, vector.x);

	// Convert to degrees
	targetAngle = targetAngle * 180 / PI;

	// The angle could be outside 0 - 360 range, so we need to convert it to 0 - 360 range
	while (targetAngle > 360)
		targetAngle = targetAngle - 360;

	while (targetAngle < 0)
		targetAngle = targetAngle + 360;

	// Calculate the absolute difference between targetAngle and faceAngle
	float angleDifference = fabs(targetAngle - faceAngle);
	angleDifference = std::min(angleDifference, 360.f - angleDifference);

	// If within half of the vision cone angle then it is within the vision cone
	if (angleDifference <= m_visionConeAngle / 2)
		return true;
	else
		return false;
}

bool SFML_NPCSpriteObject::isInCircle(sf::Vector2f targetpoint, float radius)
{
	// Get the NPC's current location 
	sf::Vector2f currentPosition = getPosition();

	// Calculate the distance (L2-norm) between the current location and the target position
	float distance = sqrt(pow(targetpoint.x - currentPosition.x, 2) + pow(targetpoint.y - currentPosition.y, 2));

	// Check if the distance is within the circle radius
	if (distance <= radius)
		return true;
	else
		return false;
}

bool SFML_NPCSpriteObject::isInAttackingState()
{
	return m_state == ATTACKING && m_currentAnimation && !m_currentAnimation->isCompleted();
}
