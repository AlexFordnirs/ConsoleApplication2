#pragma once
//#include"stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream> 
//#include "map.h"
//#include "view.h"
#include <vector>
#include <sstream>
#include <string>

using namespace sf;
using namespace std;

class Bullet
{
public:
	Vector2f position;
	Vector2f direction;
	float passedDistance;

	Image mage1_img;
	Texture mage1_texture;
	Sprite mage1_sprite;
	//	float CurrentFrame1 = 0;
	Clock clock;
	//	int bol[20];
	//Bullet();

	Bullet(float CurrentFrame1, Vector2f position, Vector2f direction);

	void update(float time);

	Vector2f getPosition();

	float getPassed();
	Sprite getSprite();
};