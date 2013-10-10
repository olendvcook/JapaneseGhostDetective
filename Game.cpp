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

void Game::reset()
{
	mCurrentLevel = 0;
	mTownHealth = 100;
	mNumofLives = 3;
	quit();
	mPlayer.setPosition(WindowWidth/2,WindowHeight -40);
	mPlayer.setIsLeft(false);
	mPlayer.setIsRight(false);
	mBall.setVelocity(0,0);
	mBall.setPosition(mPlayer.getPosition().x, WindowHeight -60);
}

Game::~Game(void)
{
}

void Game::addEnemy(float pX, float pY)
{
	//if no delete before removed u get memory leaks
	mEnemies.insert(mEnemies.begin(), new Enemy(sf::Vector2f(pX, pY), sf::Vector2f(0,.7), sf::Vector2i(32,32), (mTextures->getTexture(sGHOST))));
}

void Game::removeEnemy(int pIndex)
{
	//so delete :D
	delete(mEnemies.at(pIndex));
	mEnemies.erase((mEnemies.begin() + pIndex));
}

//same for every vector/ add and deletes
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
	//update text
	std::stringstream ss;
	ss << "Lives: " << mNumofLives;
	mTextLives.setString(ss.str());
	ss.str(std::string());
	ss << "Town Health: " << mTownHealth;
	mTextTown.setString(ss.str());


	mPlayer.update();

	mBall.update();

	//while ball isnt moving make it follow paddle
	if(mBall.getVelocity().y == 0)
	{
		mBall.setPosition(mPlayer.getPosition().x, WindowHeight -60);
	}

	//if ball hits paddle
	if(mBall.getBounds().intersects(mPlayer.getBounds()))
	{
		//angle in radians
		float mMaxBounceAngle = 3.0943951;
		//get the relative intersect X value
		//this means how far away the center of ball is from center of paddle
		float relativeIntersectX = -(mPlayer.getPosition().x - mBall.getPosition().x);
		//normalize it
		float normalizedRelativeIntersectionX = (relativeIntersectX/mPlayer.getSize().x/2);
		//calculate bounce off of the normalized intersection
		float bounceAngle = normalizedRelativeIntersectionX * mMaxBounceAngle;

		//calculate new speeds
		float newSpeedX = (4 * sin(bounceAngle) + (mPlayer.getVelocity().x * 0.1));
		float newSpeedY = 4 * -cos(bounceAngle);

		mBall.setVelocity(newSpeedX, newSpeedY);

	}

	//for each enemy in enemies
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
		//kill enemy if off screen X Direction
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

		//enemy colision with ball
		if(mBall.getBounds().intersects(mEnemies[i]->getBounds()))
		{
			//if ball position is on the sides of enemy bounce in X direction
			if(mBall.getPosition().y >= mEnemies[i]->getPosition().y - mEnemies[i]->getSize().y/2 &&
				mBall.getPosition().y <= mEnemies[i]->getPosition().y + mEnemies[i]->getSize().y/2)
			{
				mBall.setVelocity(- mBall.getVelocity().x, mBall.getVelocity().y);
			}
			//else if ball is top or bottom bounce in Y direction
			else
			{
				mBall.setVelocity(mBall.getVelocity().x, -mBall.getVelocity().y);
			}
			removeEnemy(i);
			continue;
		}
	}

	//foreach grave in graves
	for(int i = 0; i < mGraves.size(); i++)
	{
		mGraves[i]->update();

		//check if its time to make a ghost
		if(mGraves[i]->checkIfTime())
		{
			addEnemy(mGraves[i]->getPosition().x, mGraves[i]->getPosition().y);
		}

		//colision with ball
		if(mBall.getBounds().intersects(mGraves[i]->getBounds()))
		{
			//if ball position is on the sides of enemy bounce in X direction
			if(mBall.getPosition().y >= mGraves[i]->getPosition().y - mGraves[i]->getSize().y/2 &&
				mBall.getPosition().y <= mGraves[i]->getPosition().y + mGraves[i]->getSize().y/2)
			{
				mBall.setVelocity(- mBall.getVelocity().x, mBall.getVelocity().y);
			}
			//else if ball is top or bottom bounce in Y direction
			else
			{
				mBall.setVelocity(mBall.getVelocity().x, -mBall.getVelocity().y);
			}
			removeGrave(i);
		}
	}
	
	//if ball falls out the bottom of screen
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

	//if no more enemies
	if(mGraves.size() <= 0 && mEnemies.size() <= 0)
	{
		nextlevel();
	}

	//check for gameover state
	if(mTownHealth <= 0)
		mGameState = gGAMEOVER;
}

//just call draw of all entities
//entities drawn first will be back layers
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
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			mPlayer.setIsLeft(true);
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			mPlayer.setIsRight(true);
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
		}
		
		//release ball
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			if(mBall.getVelocity().y == 0)
				mBall.setVelocity(mPlayer.getVelocity().x, -4);
		}

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

//level handler basically
void Game::nextlevel()
{
	mCurrentLevel++;

	if(mCurrentLevel >= mMaxLevel)
	{
		//switch to game win state
		mGameState = gCOMPLETE;
	}
	else
	{
		//make the level
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