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
	//start playing idle animation
	AnimatedSprite::startAnimation();
}

Player::~Player(void)
{
}

//all this to just move left and right with acceleration
void Player::update()
{
	//move right
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
	//move left
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
	//come to a stop
	else
	{
		if (mVelocity.x > 0)
			mVelocity.x -= mSpeed;
		else if (mVelocity.x < 0)
			mVelocity.x += mSpeed;
		else
			mVelocity.x = 0;
	}

	//check bounds with right and left side of screen
	if(mPosition.x <= 0 + mSize.x/2)
	{
		mPosition.x = 1 + mSize.x/2;
		mVelocity.x = -(mVelocity.x + mSpeed);
	}
	if(mPosition.x >= WindowWidth - 0 - mSize.x/2)
	{
		mPosition.x = WindowWidth - 1 - mSize.x/2;
		mVelocity.x = -(mVelocity.x - mSpeed);
	}
	//call superclass update
	AnimatedSprite::update();
}