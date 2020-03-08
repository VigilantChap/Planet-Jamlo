#include "Enemy.h"
#include "Camera.h"
#include <cstdlib>

#ifndef ENEMYSTATICVARIABLES
#define ENEMYSTATICVARIABLES
sf::Clock Enemy::timelapse;
std::default_random_engine Enemy::pgenerator;
#endif // !ENEMYSTATICVARIABLES

Enemy::Enemy(std::string ID) : Entity::Entity(ID)
{
	sourceRectImage = sf::IntRect(0, 0, 100, 100);
	setTextureRect(sourceRectImage);
	maxSpeed = 1;
	speed = 1;
	isTriggered = false;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	Entity::Update();
	if (abs(playerPosition.x) < 200|| abs(playerPosition.y) < 200)
	{
		isTriggered = true;
	}
	else
	{
		isTriggered = false;
	}
	
	Animate();
}

void Enemy::SetPlayerPosition(sf::Vector2<float> position)
{
	playerPosition = position;
	
	//printf("%f\n", playerPosition.x);
}

void Enemy::StartPatrolMovement()
{
	//Distributes Values
	std::normal_distribution<double> distributionX(playerPosition.x - getPosition().x, 10000);
	std::normal_distribution<double> distributionY(playerPosition.y - getPosition().y, 10000);

	destination.x = distributionX(pgenerator);
	destination.y = distributionY(pgenerator);

	MoveTo(destination);
	
}

bool Enemy::InView()
{

	
	return false;
}

void Enemy::Animate()
{

	if (Enemy::timelapse.getElapsedTime().asSeconds() > 0.5f)
	{
		if (isTriggered)
		{
			sourceRectImage.top = 0;

			if (sourceRectImage.left >= 500)
				sourceRectImage.left = 0;
			else
				sourceRectImage.left += 100;

			setTextureRect(sourceRectImage);
		}
		else
		{
			sourceRectImage.top = 100;
			if (sourceRectImage.left >= 300)
				sourceRectImage.left = 0;
			else
				sourceRectImage.left += 100;

			setTextureRect(sourceRectImage);
		}
		Enemy::timelapse.restart();
	}
}