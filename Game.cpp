#include "Game.h"

//takes in pointer to class that hold spritesheets so entities can be created with certain spritesheet
Game::Game(Textures *pSpriteSheet) :
	mTextures(pSpriteSheet),
	mPlayer(sf::Vector2f(WindowWidth/2,WindowHeight -40), sf::Vector2f(0,0), sf::Vector2i(96,16), (pSpriteSheet->getTexture(sPLAYER))),
	mBall(sf::Vector2f(WindowWidth/2,WindowHeight -60), sf::Vector2f(0,0), sf::Vector2i(16,16), (pSpriteSheet->getTexture(sBALL))),
	mNumofLives(3),
	mTownHealth(11),
	mCurrentLevel(0),
	mMaxLevel(8)
{
	mBackground.setTexture(*pSpriteSheet->getTexture(sGAMEBACKGROUND));
	nextlevel();
}

Game::~Game(void)
{
}

//TODO: paramertertize this method to input the valuves to be used for the enemies
void Game::addEnemy(float pX, float pY)
{
	//if no delete before removed u get memory leaks
	mEnemies.insert(mEnemies.begin(), new Enemy(sf::Vector2f(pX, pY), sf::Vector2f(0,.7), sf::Vector2i(32,32), (mTextures->getTexture(sGHOST))));
}

void Game::removeEnemy(int pIndex)
{
	delete(mEnemies.at(pIndex));
	mEnemies.erase((mEnemies.begin() + pIndex));
}

void Game::addGrave(float pX, float pY, float pReleaseTime)
{
	mGraves.insert(mGraves.begin(), new Grave(sf::Vector2f(pX,pY), sf::Vector2f(0,0), sf::Vector2i(64,80),(mTextures->getTexture(sGRAVE)), pReleaseTime));
}

void Game::removeGrave(int pIndex)
{
	delete(mGraves.at(pIndex));
	mGraves.erase((mGraves.begin() + pIndex));
}
//update method used during game state
//TODO: think of a way to make Game update not so beastly and hacky, maybe some kind of event system
void Game::update()
{
	//call each entity update
	mPlayer.update();
	mBall.update();

	if(mBall.getVelocity().y == 0)
	{
		mBall.setPosition(mPlayer.getPosition().x, WindowHeight -60);
	}

	//if ball hits paddle
	if(mBall.getBounds().intersects(mPlayer.getBounds()))
	{
		//120 degrees in radians
		float mMaxBounceAngle = 2.0943951;
		float relativeIntersectX = -(mPlayer.getPosition().x - mBall.getPosition().x);
		float normalizedRelativeIntersectionX = (relativeIntersectX/mPlayer.getSize().x/2);
		float bounceAngle = normalizedRelativeIntersectionX * mMaxBounceAngle;

		float newSpeedX = (6 * sin(bounceAngle) + (mPlayer.getVelocity().x * 0.3));
		float newSpeedY = 6 * -cos(bounceAngle);

		mBall.setVelocity(newSpeedX, newSpeedY);

	}

	for(int i = 0; i < mEnemies.size(); i++)
	{	

		//update enemy
		mEnemies[i]->update();

		//collision detection with player
		if(mEnemies[i]->getBounds().intersects(mPlayer.getBounds()))
		{
			removeEnemy(i);
			continue;
		}
		//kill enemy if off screen
		if(mEnemies[i]->getPosition().x > WindowWidth || mEnemies[i]->getPosition().x < 0)
		{
			removeEnemy(i);
			continue;
		}
		if(mEnemies[i]->getPosition().y > WindowHeight || mEnemies[i]->getPosition().y < 0)
		{
			mTownHealth -= 10;
			removeEnemy(i);
			continue;
		}

		//colision with ball
		if(mBall.getBounds().intersects(mEnemies[i]->getBounds()))
		{
			if(mBall.getPosition().y >= mEnemies[i]->getPosition().y - mEnemies[i]->getSize().y/2 &&
				mBall.getPosition().y <= mEnemies[i]->getPosition().y + mEnemies[i]->getSize().y/2)
			{
				mBall.setVelocity(- mBall.getVelocity().x, mBall.getVelocity().y);
			}
			else
			{
				mBall.setVelocity(mBall.getVelocity().x, -mBall.getVelocity().y);
			}
			removeEnemy(i);
			continue;
		}
	}

	for(int i = 0; i < mGraves.size(); i++)
	{
		mGraves[i]->update();

		if(mGraves[i]->checkIfTime())
		{
			addEnemy(mGraves[i]->getPosition().x, mGraves[i]->getPosition().y);
		}

		//colision with ball
		if(mBall.getBounds().intersects(mGraves[i]->getBounds()))
		{
			if(mBall.getPosition().y >= mGraves[i]->getPosition().y - mGraves[i]->getSize().y/2 &&
				mBall.getPosition().y <= mGraves[i]->getPosition().y + mGraves[i]->getSize().y/2)
			{
				mBall.setVelocity(- mBall.getVelocity().x, mBall.getVelocity().y);
			}
			else
			{
				mBall.setVelocity(mBall.getVelocity().x, -mBall.getVelocity().y);
			}
			removeGrave(i);
			continue;
		}
	}
	
	if(mBall.getPosition().y >= WindowHeight - mBall.getSize().y)
	{
		//mNumofLives--;
		if(mNumofLives > 0)
		{
			mBall.setVelocity(0,0);
			mBall.setPosition(mPlayer.getPosition().x, WindowHeight -60);
		}
		else
		{
			mGameState = gGAMEOVER;
		}
	}

	if(mGraves.size() <= 0)
	{
		nextlevel();
	}
	//check for gameover state
	//if(mEnemies.size() == 0)
	//	mGameState = gGAMEOVER;
}

//just call draw of all entities
void Game::draw(sf::RenderWindow *window, float pInterpolation)
{
	window->draw(mBackground);
	mPlayer.draw(window, pInterpolation);
	mBall.draw(window, pInterpolation);
	
	for(int i = 0; i < mGraves.size(); i++)
	{
		mGraves[i]->draw(window, pInterpolation);
	}
	
	
	for(int i = 0; i < mEnemies.size(); i++)
	{
		mEnemies[i]->draw(window, pInterpolation);
	}
	
}

//get passed the input events do stuff based on event type
void Game::input(sf::Event *pEvent)
{
	switch(pEvent->type)
	{
	//change state of player based on what key is pressed
	case(sf::Event::KeyPressed):
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			mPlayer.setIsLeft(true);
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			mPlayer.setIsRight(true);
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			mBall.setVelocity(mPlayer.getVelocity().x, -5);
		}

		//created to test for memory leaks
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
		}
		break;
	case(sf::Event::KeyReleased):
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			mPlayer.setIsLeft(false);
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			mPlayer.setIsRight(false);
		break;
	default:
		break;
	}
}

//TODO: impliment reset
void Game::reset()
{
}

//handle memory leaks before quitting
void Game::quit()
{
	//we go down because when we remove from front, everything shifts down
	for(int i = mEnemies.size() - 1; i >= 0; i--)
	{
		removeEnemy(i);
	}
	for(int i = mGraves.size() - 1; i >= 0; i--)
	{
		removeGrave(i);
	}
}

void Game::nextlevel()
{
	mCurrentLevel++;

	if(mCurrentLevel >= mMaxLevel)
	{
		//switch to game win state
	}
	else
	{
		switch(mCurrentLevel)
		{
		case(1):
			addGrave(300,100,5);
			addGrave(300,200,5);
			addGrave(100,200,5);
			addGrave(200,200,5);
			break;
		case(2):
			addGrave(300,100,5);
			addGrave(300,200,5);
			addGrave(100,200,5);
			addGrave(200,200,5);
			break;
		case(3):
			addGrave(300,100,5);
			addGrave(300,200,5);
			addGrave(100,200,5);
			addGrave(200,200,5);
			break;
		case(4):
			addGrave(300,100,5);
			addGrave(300,200,5);
			addGrave(100,200,5);
			addGrave(200,200,5);
			break;
		case(5):
			addGrave(300,100,5);
			addGrave(300,200,5);
			addGrave(100,200,5);
			addGrave(200,200,5);
			break;
		case(6):
			addGrave(300,100,5);
			addGrave(300,200,5);
			addGrave(100,200,5);
			addGrave(200,200,5);
			break;
		case(7):
			addGrave(300,100,5);
			addGrave(300,200,5);
			addGrave(100,200,5);
			addGrave(200,200,5);
			break;
		case(8):
			addGrave(300,100,5);
			addGrave(300,200,5);
			addGrave(100,200,5);
			addGrave(200,200,5);
			break;
		}
	}
	mBall.setVelocity(0,0);
	mBall.setPosition(mPlayer.getPosition().x, WindowHeight -60);
}