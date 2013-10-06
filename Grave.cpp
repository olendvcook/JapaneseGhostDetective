#include "Grave.h"


Grave::Grave(sf::Vector2f pPosition, 
		sf::Vector2f pVelocity, 
		sf::Vector2i pSize, 
		sf::Texture *pTexture,
		float pReleaseTime,
		float pAngle, 
		float pAngularVelocity) :
	//call superclass constructor
		Enemy(pPosition, pVelocity, pSize, pTexture, pAngle, pAngularVelocity),
		mReleaseTime(pReleaseTime),
		mClock()
{
	mAnimations.insert(mAnimations.begin(), Animation(8,.12, true));
	AnimatedSprite::startAnimation();
}


Grave::~Grave(void)
{
}

bool Grave::checkIfTime()
{
	if(mClock.getElapsedTime().asSeconds() > mReleaseTime)
	{
		mClock.restart();
		return true;
	}
	return false;
}