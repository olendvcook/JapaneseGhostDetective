#include "Ball.h"


Ball::Ball(sf::Vector2f pPosition, 
		sf::Vector2f pVelocity, 
		sf::Vector2i pSize, 
		sf::Texture *pTexture,  
		float pAngle, 
		float pAngularVelocity) :
	//call superclass constructor
	AnimatedSprite(pPosition, pVelocity, pSize, pTexture, pAngle, pAngularVelocity)
{
	mAnimations.insert(mAnimations.begin(), Animation(1,.12, true));
	AnimatedSprite::startAnimation();
}


Ball::~Ball(void)
{
}

void Ball::update()
{
	if(mPosition.x <= 15 + mSpriteSize.x/2)
	{
		mPosition.x = 16 + mSpriteSize.x/2;
		mVelocity.x = -(mVelocity.x );
	}
	if(mPosition.x >= WindowWidth - 15 - mSpriteSize.x/2)
	{
		mPosition.x = WindowWidth - 16 - mSpriteSize.x/2;
		mVelocity.x = -(mVelocity.x);
	}
	if(mPosition.y <= 15 + mSpriteSize.y/2)
	{
		mPosition.y = 16 + mSpriteSize.y/2;
		mVelocity.y = -(mVelocity.y);
	}
	//call superclass update
	AnimatedSprite::update();
}