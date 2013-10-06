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
	//this is how u random
	std::random_device rd; 
    std::mt19937 eng(rd()); 
    std::uniform_real_distribution<> dis(0.12, 0.17); 
	//-----------------------------------------------
	mAnimations.insert(mAnimations.begin(), Animation(8,dis(eng), true));
	AnimatedSprite::startAnimation();

	mSprite.scale(0.7,0.7);
	mSpriteSize.x *= 0.7;
	mSpriteSize.y *= 0.7;
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