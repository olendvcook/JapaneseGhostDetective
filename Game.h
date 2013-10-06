#pragma once
#include <SFML\Graphics.hpp>
#include "Includes.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "Textures.h"
#include "Enemy.h"
#include "Grave.h"
#include "Ball.h"

//Handles the gameplay state, holds its entities like enemies and player would hold score etc...
//Handles interaction between entities


//TODO: Gameplay states for easy seperate levels possibly, think about it more
class Game
{
private:
	//store entities
	Player mPlayer;
	Ball mBall;
	std::vector<Enemy*> mEnemies;
	std::vector<Grave*> mGraves;
	Textures *mTextures;
	sf::Sprite mBackground;
	int mNumofLives;
	int mTownHealth;
	int mCurrentLevel;
	int mMaxLevel;
public:
	Game(Textures *pSpriteSheet);
	virtual ~Game(void);

	void update();
	void draw(sf::RenderWindow *window, float pInterpolation);
	void input(sf::Event *pEvent);
	void reset();
	void addEnemy(float pX, float pY);
	void removeEnemy(int pIndex);
	void addGrave(float pX, float pY, float pReleaseTime);
	void removeGrave(int pIndex);
	void quit();
	void nextlevel();
};

