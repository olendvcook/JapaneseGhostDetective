#pragma once
#include "enemy.h"
#include <random>
class Grave :
	public Enemy
{
protected:
	sf::Clock mClock;
	float mReleaseTime;
	float mReleaseTimeMin;
	float mReleaseTimeMax;
public:
	Grave(sf::Vector2f pPosition, 
		sf::Vector2f pVelocity, 
		sf::Vector2i pSize, 
		sf::Texture *pTexture,
		float pReleaseTimeMin = 5,
		float pReleaseTimeMax = 10,
		float pAngle = 0, 
		float pAngularVelocity = 0);
	virtual ~Grave(void);

	bool checkIfTime();
};

