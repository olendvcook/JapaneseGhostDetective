#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include "Textures.h"

//Class that holds all the spritesheets so that each entity does not need to store a copy
//Entitie Sprites hold pointers to these spritesheets

//TODO:: different texture objects for different states?

//spritesheet type
enum SPRITESHEETS { sBUTTONS, sPLAYER, sGHOST, sBACKGROUND, sGAMEOVER, sGRAVE, sGAMEBACKGROUND, sPAUSED, sCOMPLETE, sBALL};
class Textures
{
private:
	//vetor that holds the textures
	std::vector<sf::Texture> mSheets;
public:
	Textures(void);
	virtual ~Textures(void);

	sf::Texture* getTexture(SPRITESHEETS pSheet);
};

