#include "Player.h"

Player::Player(
		sf::Vector2f pPosition, 
		sf::Vector2f pVelocity, 
		sf::Vector2i pSize, 
		sf::Texture *pTexture,  
		float pAngle, 
		float pAngularVelocity) :
	AnimatedSprite(pPosition, pVelocity, pSize, pTexture, pAngle, pAngularVelocity)
{
	isLeft = false;
	isRight = false;
	//add animations idle/walk/jump
	mAnimations.insert(mAnimations.begin(), Animation(4, 0.2, true));
	//mSprite.scale(4,4);
	//start playing idle animation
	AnimatedSprite::startAnimation();
	mPlayerState = pNONE;
}

Player::~Player(void)
{
}

//update depending on player state
void Player::update()
{
	//right now just moves and switches animations
	if(isRight && !isLeft)
	{
		if(mVelocity.x >= mMaxSpeed)
		{
			mVelocity.x = mMaxSpeed;
		}
		else
		{
			mVelocity.x += mSpeed;
		}
	}
	else if(isLeft && !isRight)
	{
		if(abs(mVelocity.x) >= abs(mMaxSpeed))
		{
			mVelocity.x = -mMaxSpeed;
		}
		else
		{
			mVelocity.x -= mSpeed;
		}
	}
	else
	{
		if (mVelocity.x > 0)
			mVelocity.x -= mSpeed;
		else if (mVelocity.x < 0)
			mVelocity.x += mSpeed;
		else
			mVelocity.x = 0;
	}

	if(mPosition.x <= 0 + mSpriteSize.x/2)
	{
		mPosition.x = 1 + mSpriteSize.x/2;
		mVelocity.x = -(mVelocity.x + mSpeed);
	}
	if(mPosition.x >= WindowWidth - 0 - mSpriteSize.x/2)
	{
		mPosition.x = WindowWidth - 1 - mSpriteSize.x/2;
		mVelocity.x = -(mVelocity.x - mSpeed);
	}
	//call superclass update
	AnimatedSprite::update();
}