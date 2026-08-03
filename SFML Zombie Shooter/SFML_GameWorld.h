#pragma once

#include <SFML/Graphics.hpp>
#include "SFML_GameWorldLayer.h"
#include "SFML_SpriteObject.h"
#include "SFML_AnimatedSpriteObject.h"
#include "SFML_NPCSpriteObject.h"
#include "SFML_PlayableSpriteObject.h"
#include "SFML_BulletSpriteObject.h"
#include <vector>

using namespace std;

class SFML_GameWorld :
    public sf::Drawable
{
public:
                       SFML_GameWorld(int windowWidth, int windowHeight, sf::RenderWindow* parentWindow);
                       ~SFML_GameWorld();

    void               update(float elapsedTime);


private:
    void               processEvents(float elapsedTime);
    virtual void       draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // Static function to be used to sort SFML_GameWorldLayer array
    static bool layerSortFunction(SFML_GameWorldLayer* a, SFML_GameWorldLayer* b);

    // Member attributes to store render window dimension
    int                m_windowWidth;
    int                m_windowHeight;

    // The array to store the game world layers
    vector<SFML_GameWorldLayer*> m_gameWorldLayerList;

    // The member attribute to store the camera object
    SFML_2DCamera      m_camera;

    // The member attribute to store the camera position 
    sf::Vector2f       m_cameraPosition;

    // The member attribute to store the sprite object
    SFML_SpriteObject  m_spriteObject;

    // Member attribute to store pointer to the parent render window 
    sf::RenderWindow*  m_parentWindow;

    SFML_AnimatedSpriteObject* m_animatedObject;
    int                        m_idleAnimationID;

    mutable vector<SFML_NPCSpriteObject*> m_zombieCharacterList;
    int                           m_numberofZombies;

    SFML_PlayableSpriteObject* m_playerCharacter;

    mutable vector<SFML_BulletSpriteObject*> m_bulletList;

    mutable bool m_playAnimation;

	float m_playerDamageCooldownRemaining;

    sf::Texture        m_fullHealthTexture;
    sf::Sprite         m_fullHealthSprite;

    sf::Texture        m_halfHealthTexture;
    sf::Sprite         m_halfHealthSprite;

    sf::Texture        m_quarterHealthTexture;
    sf::Sprite         m_quarterHealthSprite;

    sf::Texture        m_deadHealthTexture;
    sf::Sprite         m_deadHealthSprite;

    sf::Sprite m_healthSprite;

    sf::Texture        m_borderTexture;
    sf::Sprite         m_borderSprite;

    sf::Texture        m_flashlightTexture;
    sf::Sprite         m_flashlightSprite;

    sf::Texture        m_knifeTexture;
    sf::Sprite         m_knifeSprite;

    sf::Texture        m_pistolTexture;
    sf::Sprite         m_pistolSprite;

    sf::Texture        m_rifleTexture;
    sf::Sprite         m_rifleSprite;

    sf::Sprite         m_iconSprite;

    sf::Texture        m_portraitTexture;
    sf::Sprite         m_portraitSprite;
};

