#include "Enemy.h"


Enemy::Enemy(sf::Vector2f pPosition, 
		sf::Vector2f pVelocity, 
		sf::Vector2i pSize, 
		sf::Texture *pTexture,  
		float pAngle, 
		float pAngularVelocity) :
	//call superclass constructor
	AnimatedSprite(pPosition, pVelocity, pSize, pTexture, pAngle, pAngularVelocity)
{
	//insert animations based on Enemy spriteSheet
	mAnimations.insert(mAnimations.begin(), Animation(4, 0.2, true));
	mSprite.scale(0.5,0.5);
	mSpriteSize.x *= 0.5;
	mSpriteSize.y *= 0.5;
	//start idle animation
	AnimatedSprite::startAnimation();
}


Enemy::~Enemy(void)
{
}

//update based on what state we are in
void Enemy::update()
{
	//call superclass update that handles changing pos by velocity and animates
	AnimatedSprite::update();
}

