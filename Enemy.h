#pragma once
#include "animatedsprite.h"

//Example of extended Animated Sprite class to create simple enemies
//Should extend this for different types of enemies

//in this game ghost is implimented in enemy because ghost is so simple

//update depending on what state enemy is in
//TODO: is states for enemies as dumb as states for player?
enum ENEMYSTATE { eLEFT, eRIGHT, eNONE };

//extends AnimatedSprite so it has all of AnimatedSprites vars and methods
class Enemy :
	public AnimatedSprite
{
protected:
	ENEMYSTATE mEnemyState;
public:
	Enemy(sf::Vector2f pPosition, 
		sf::Vector2f pVelocity, 
		sf::Vector2i pSize, 
		sf::Texture *pTexture, 
		float pAngle = 0, 
		float pAngularVelocity = 0);
	virtual ~Enemy(void);

	//override animated sprite update method
	void update();

	void setEnemyState(ENEMYSTATE pEnemyState) { mEnemyState = pEnemyState; }
};

