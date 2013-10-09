#include "Game.h"

//takes in pointer to class that hold spritesheets so entities can be created with certain spritesheet
Game::Game(Textures *pSpriteSheet) :
	mTextures(pSpriteSheet),
	mPlayer(sf::Vector2f(WindowWidth/2,WindowHeight -40), sf::Vector2f(0,0), sf::Vector2i(96,16), (pSpriteSheet->getTexture(sPLAYER))),
	mBall(sf::Vector2f(WindowWidth/2,WindowHeight -60), sf::Vector2f(0,0), sf::Vector2i(16,16), (pSpriteSheet->getTexture(sBALL))),
	mNumofLives(3),
	mTownHealth(100),
	mCurrentLevel(0),
	mMaxLevel(9)
{

	mBackground.setTexture(*pSpriteSheet->getTexture(sGAMEBACKGROUND));
	
	//text stuff
	mFont.loadFromFile("Assets/galaxymonkey.ttf");
	mTextLives.setFont(mFont);
	mTextTown.setFont(mFont);
	mTextLives.setColor(sf::Color::White);
	mTextTown.setColor(sf::Color::White);
	mTextTown.setScale(0.6,0.6);
	mTextLives.setScale(0.6,0.6);
	mTextTown.setPosition(30, WindowHeight - 25);
	mTextLives.setPosition(WindowWidth - 100, WindowHeight - 25);

}

Game::~Game(void)
{
}

//TODO: paramertertize this method to input the valuves to be used for the enemies
void Game::addEnemy(float pX, float pY)
{
	//if no delete before removed u get memory leaks
	mEnemies.insert(mEnemies.begin(), new Enemy(sf::Vector2f(pX, pY), sf::Vector2f(0,.7), sf::Vector2i(64,64), (mTextures->getTexture(sGHOST))));
}

void Game::removeEnemy(int pIndex)
{
	delete(mEnemies.at(pIndex));
	mEnemies.erase((mEnemies.begin() + pIndex));
}

void Game::addGrave(float pX, float pY, float pReleaseTimeMin, float pReleaseTimeMax)
{
	mGraves.insert(mGraves.begin(), new Grave(sf::Vector2f(pX,pY), sf::Vector2f(0,0), sf::Vector2i(64,80),(mTextures->getTexture(sGRAVE)), pReleaseTimeMin, pReleaseTimeMax));
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
	std::stringstream ss;
	ss << "Lives: " << mNumofLives;
	mTextLives.setString(ss.str());
	ss.str(std::string());
	ss << "Town Health: " << mTownHealth;
	mTextTown.setString(ss.str());


	mPlayer.update();

	if(mBall.getVelocity().y == 0)
	{
		mBall.setPosition(mPlayer.getPosition().x, WindowHeight -60);
	}

	//if ball hits paddle
	if(mBall.getBounds().intersects(mPlayer.getBounds()))
	{
		//120 degrees in radians
		float mMaxBounceAngle = 3.0943951;
		float relativeIntersectX = -(mPlayer.getPosition().x - mBall.getPosition().x);
		float normalizedRelativeIntersectionX = (relativeIntersectX/mPlayer.getSize().x/2);
		float bounceAngle = normalizedRelativeIntersectionX * mMaxBounceAngle;

		float newSpeedX = (4 * sin(bounceAngle) + (mPlayer.getVelocity().x * 0.1));
		float newSpeedY = 4 * -cos(bounceAngle);

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
		}
	}
	
	if(mBall.getPosition().y >= WindowHeight - mBall.getSize().y)
	{
		mNumofLives--;
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

	mBall.update();

	if(mGraves.size() <= 0 && mEnemies.size() <= 0)
	{
		nextlevel();
	}

	//check for gameover state
	if(mTownHealth <= 0)
		mGameState = gGAMEOVER;
}

//just call draw of all entities
void Game::draw(sf::RenderWindow *window, float pInterpolation)
{
	window->draw(mBackground);


	window->draw(mTextLives);
	window->draw(mTextTown);


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
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if(mBall.getVelocity().y == 0)
				mBall.setVelocity(mPlayer.getVelocity().x, -4);
		}

		//created to test for memory leaks
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			mGameState = gPAUSED;
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

	mGraves.clear();
	mGraves.shrink_to_fit();

	if(mCurrentLevel >= mMaxLevel)
	{
		//switch to game win state
		mGameState = gCOMPLETE;
	}
	else
	{
		switch(mCurrentLevel)
		{
		case(1):
			addGrave(320,100,8,12);
			addGrave(440,100,8,12);
			addGrave(200,100,8,12);
			break;
		case(2):
			addGrave(320,100,6,9);
			addGrave(440,100,8,10);
			addGrave(200,100,8,10);
			addGrave(320,200,8,10);
			break;
		case(3):
			addGrave(370,100,6,9);
			addGrave(270,100,6,9);
			addGrave(320,200,8,10);
			addGrave(440,200,8,10);
			addGrave(200,200,8,10);
			break;
		case(4):
			addGrave(320,100,5,7);
			addGrave(380,100,7,9);
			addGrave(260,100,7,9);
			addGrave(320,200,8,10);
			addGrave(380,200,8,10);
			addGrave(260,200,8,10);
			break;
		case(5):
			addGrave(320,100,5,7);
			addGrave(380,100,7,9);
			addGrave(260,100,7,9);
			addGrave(200,100,7,9);
			addGrave(440,100,7,9);
			addGrave(320,200,8,10);
			addGrave(380,200,8,10);
			addGrave(260,200,8,10);
			addGrave(200,200,8,10);
			addGrave(440,200,8,10);
			break;
		case(6):
			addGrave(320,100,5,7);
			addGrave(380,100,7,9);
			addGrave(260,100,7,9);
			addGrave(200,100,7,9);
			addGrave(440,100,7,9);
			addGrave(320,200,8,10);
			addGrave(380,200,8,10);
			addGrave(260,200,8,10);
			addGrave(200,200,8,10);
			addGrave(440,200,8,10);
			addGrave(320,300,8,10);
			addGrave(380,300,8,10);
			addGrave(260,300,8,10);
			addGrave(200,300,8,10);
			addGrave(440,300,8,10);
			break;
		case(7):
			
			addGrave(320,100,5,7);
			addGrave(440,100,6,9);
			addGrave(200,100,6,9);
			addGrave(80,100,6,9);
			addGrave(560,100,6,9);
			addGrave(320,200,8,10);
			addGrave(440,200,8,10);
			addGrave(200,200,8,10);
			addGrave(80,200,8,10);
			addGrave(560,200,8,10);
			break;
		case(8):
			
			addGrave(320,100,5,7);
			addGrave(440,100,6,9);
			addGrave(200,100,6,9);
			addGrave(80,100,6,9);
			addGrave(560,100,6,9);
			addGrave(320,200,6,9);
			addGrave(440,200,6,9);
			addGrave(200,200,6,9);
			addGrave(80,200,6,9);
			addGrave(560,200,6,9);
			addGrave(320,300,8,10);
			addGrave(440,300,8,10);
			addGrave(200,300,8,10);
			addGrave(80,300,8,10);
			addGrave(560,300,8,10);
			
			break;
		}
	}
	mPlayer.setPosition(WindowWidth/2,WindowHeight -40);
	mBall.setVelocity(0,0);
	mBall.setPosition(mPlayer.getPosition().x, WindowHeight -60);
}