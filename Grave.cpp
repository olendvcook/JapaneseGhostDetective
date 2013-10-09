#include "Grave.h"


Grave::Grave(sf::Vector2f pPosition, 
		sf::Vector2f pVelocity, 
		sf::Vector2i pSize, 
		sf::Texture *pTexture,
		float pReleaseTimeMin,
		float pReleaseTimeMax,
		float pAngle, 
		float pAngularVelocity) :
	//call superclass constructor
		Enemy(pPosition, pVelocity, pSize, pTexture, pAngle, pAngularVelocity),
		mClock(),
		mReleaseTimeMin(pReleaseTimeMin),
		mReleaseTimeMax(pReleaseTimeMax)
{
	std::random_device rd; 
    std::mt19937 eng(rd()); 
    std::uniform_real_distribution<> dis(mReleaseTimeMin, mReleaseTimeMax);
	mReleaseTime = floor(dis(eng));
	//this is how u random 
    std::uniform_real_distribution<> dis2(0.12, 0.17); 
	//-----------------------------------------------
	mAnimations.insert(mAnimations.begin(), Animation(8,dis2(eng), true));
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
		std::random_device rd; 
		std::mt19937 eng(rd()); 
		std::uniform_real_distribution<> dis(mReleaseTimeMin, mReleaseTimeMax);
		mReleaseTime = floor(dis(eng));
		mClock.restart();
		return true;
	}
	return false;
}