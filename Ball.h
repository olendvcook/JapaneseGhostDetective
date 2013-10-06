#pragma once
#include "animatedsprite.h"
class Ball :
	public AnimatedSprite
{
public:
	Ball(sf::Vector2f pPosition, 
		sf::Vector2f pVelocity, 
		sf::Vector2i pSize, 
		sf::Texture *pTexture, 
		float pAngle = 0, 
		float pAngularVelocity = 0);
	virtual ~Ball(void);
	void update();
};

