#include "GameScene.h"
#include "Camera.h"
#include "GameObject.h"
#include "Entity.h"
#include "Player.h"
#include "Dog.h"
#include "Enemy.h"
#include <iostream>
#include <vector>


GameScene::GameScene(sf::RenderWindow * window_) : GameScene(window, "")
{
}

GameScene::GameScene(sf::RenderWindow * window_, std::string backgroundTexture) : window(window_), backgroundTextureName(backgroundTexture)
{	
}


GameScene::~GameScene()
{
}

bool GameScene::Initialize() {
	triggered = false;

	player = new Player("player");
	player->LoadTexture("Assets/PlayerSpriteSheet.png");
	player->scale(3, 3);
	player->updateCentre();

	tempEnemy = new Enemy("enemy00");
	tempEnemy->LoadTexture("Assets/EnemySpriteSheet.png");
	tempEnemy->scale(3, 3);
	tempEnemy->updateCentre();
	tempEnemy->SetPlayerPosition(player->getPosition());
	


	camera = new Camera(window);
	camera->SetAsMainView();
	camera->SetFollowTarget(player);
	
	if (backgroundTextureName != "") {
		if (!SetBackground(backgroundTextureName)) {
			Destroy();
			return false;
		}
	}

	
	enemies.reserve(6);

	for (int i = 0; i < enemies.size(); i++) {
		enemies[i] = new Enemy("enemy" + i);
		enemies[i]->LoadTexture("Assets/EnemySpriteSheet.png");
		enemies[i]->scale(3, 3);
		enemies[i]->updateCentre();
		enemies[i]->SetPlayerPosition(player->getPosition());
	}
	
	return true;
}

void GameScene::Destroy() {
	for (Enemy* enemy : enemies)
	{
		delete enemy;
		enemy = nullptr;
	}

	enemies.clear();

	camera->Destroy();
}

void GameScene::HandleEvents(sf::Event event) const {

	camera->HandleEvents(event);

	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
			player->MoveTo(window->mapPixelToCoords(pixelPos));
		}

	}	
	
}

void GameScene::Update() {


	if (worldTimer.getElapsedTime().asSeconds() >= 10) {
		if (triggered) triggered = false;
		else triggered = true;
		worldTimer.restart();
	}

	if (triggered) {
		for (Enemy* enemy : enemies) {
			enemy->isTriggered = true;			
		}
	}

	else {
		for (Enemy* enemy : enemies) {
			enemy->isTriggered = false;
		}
	}

	for (Enemy* enemy : enemies) {
		enemy->SetPlayerPosition(player->getPosition());
		enemy->Update();
	}

	camera->Update();
	player->Update();
	tempEnemy->SetPlayerPosition(player->getPosition());
	tempEnemy->Update();
}

void GameScene::Render() {
	
	window->clear();
	camera->Render();
	window->draw(backgroundSprite);
	window->draw(*player->getDog());
	window->draw(*player);
	window->draw(*tempEnemy);
	window->display();
	
}

bool GameScene::SetBackground(std::string textureName)
{
	if (!backgroundTexture.loadFromFile(textureName)) {
		std::cout << "Could not load background image.";
		return false;
	}

	backgroundTexture.setRepeated(true);
	sf::FloatRect fBoundary(0.0f, 0.0f, 5000.0f, 5000.0f);
	sf::IntRect iBoundary(fBoundary);
	backgroundSprite = sf::Sprite(backgroundTexture, iBoundary);
	backgroundSprite.setPosition(fBoundary.left - camera->GetView().getSize().x, fBoundary.top - 5000.0f + camera->GetView().getSize().y);

	
	return true;
}
