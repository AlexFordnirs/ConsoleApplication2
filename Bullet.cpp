#include"stdafx.h"
#include <SFML\Graphics.hpp>
#include "Bullet.h"

Bullet::Bullet(float CurrentFrame1, Vector2f position, Vector2f direction)
{

	mage1_img.loadFromFile("img/Untitled-7.png");
	mage1_texture.loadFromImage(mage1_img);
	mage1_sprite.setTexture(mage1_texture);
	mage1_sprite.setTextureRect(IntRect(9, 2, 29, 47));


	this->position = position;
	this->direction = direction;
	this->passedDistance = 0.0;
}

void Bullet::update(float time)
{
	position.x += direction.x * time;
	position.y += direction.y * time;
	passedDistance += pow(pow(direction.x, 2.0) + pow(direction.y, 2.0), 1. / 2.);
	mage1_sprite.setPosition(position);
}

Vector2f Bullet::getPosition()
{
	return position;
}

float Bullet::getPassed()
{
	return passedDistance;
}
Sprite Bullet::getSprite()
{
	return mage1_sprite;

}