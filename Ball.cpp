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
