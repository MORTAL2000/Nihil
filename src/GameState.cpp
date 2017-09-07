/*
Copyright (c) 2017 InversePalindrome
Nihil - GameState.cpp
InversePalindrome.com
*/


#include "GameState.hpp"
#include "StateMachine.hpp"


GameState::GameState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	world(b2Vec2(0.f, -9.8f)),
	entityManager(world, stateData.resourceManager, stateData.soundManager, stateData.inputHandler, collisionsData),
	map(sf::Vector2f(8192.f, 1536.f), stateData.games.front(), world, entityManager.getComponentSerializer(), stateData.resourceManager, collisionsData),
	camera(stateData.window.getDefaultView()),
	collisionHandler(entityManager.getEvents()),
	healthBar(stateData.resourceManager),
	coinDisplay(stateData.resourceManager)
{
	world.SetContactListener(&collisionHandler);

	healthBar.setPosition(150.f, 120.f);

	coinDisplay.setPosition(1600.f, 120.f);
	coinDisplay.setNumberOfCoins(stateData.games.front().getCoins());
	
	entityManager.getEvents().subscribe<DestroyEntity>([this](const auto& event) 
	{ 
		callbacks.push_back([this, event]
		{ 
		    entityManager.destroyEntity(event.entity);
		}); 
	});
	entityManager.getEvents().subscribe<Teleported>([this, &stateData](const auto& event)
	{
		callbacks.push_back([this, event]
		{ 
			changeLevel(event.level);
		});
	});
	entityManager.getEvents().subscribe<GameOver>([this, &stateData](const auto& event)
	{
		callbacks.push_back([this, &stateData] 
		{
			changeLevel(stateData.games.front().getCurrentLevel());
		});
	});
	entityManager.getEvents().subscribe<PickedUpCoin>([this, &stateData](const auto& event)
	{
		stateData.games.front().setCoins(stateData.games.front().getCoins() + 1u);
		coinDisplay.setNumberOfCoins(stateData.games.front().getCoins());
	});
	entityManager.getEvents().subscribe<CombatOcurred>([this](const auto& event)
	{
		setDisplayHitpoints(event.victim);
	});

	stateData.window.setView(camera);

	changeLevel(stateData.games.front().getCurrentLevel());
}

void GameState::handleEvent(const sf::Event& event)
{
	if (this->stateData.inputHandler.isActive("Escape"))
	{
		this->saveData("Resources/Files/SavedGames.txt");
		this->stateMachine.pushState(StateID::Pause);
	}
}

void GameState::update(float deltaTime)
{
	const float timeStep = 1.f / 60.f;
	const std::size_t velocityIterations = 6u;
	const std::size_t positionIterations = 2u;

	this->world.Step(timeStep, velocityIterations, positionIterations);

	for (auto& callback : this->callbacks)
	{
		callback();
	}

	this->callbacks.clear();
	
	this->entityManager.update(deltaTime);
	this->updateCamera();
	this->coinDisplay.update(deltaTime);
}

void GameState::draw()
{
	this->stateData.window.setView(this->camera);
	this->stateData.window.draw(this->map);
	this->stateData.window.draw(this->entityManager);

	this->stateData.window.setView(this->stateData.window.getDefaultView());
	this->stateData.window.draw(this->healthBar);
	this->stateData.window.draw(this->coinDisplay);
}

void GameState::updateCamera()
{
	const auto& playerPosition = this->entityManager.getEntities().
		get_entities<ControllableComponent, PositionComponent>().back().get_component<PositionComponent>().getPosition();
	
	if (playerPosition.x > this->camera.getSize().x / 2.f && playerPosition.x < this->map.getBounds().width - this->camera.getSize().x / 2.f)
	{
		this->camera.setCenter(playerPosition.x, this->camera.getCenter().y);
		this->stateData.window.setView(this->camera);
	}
}

void GameState::changeLevel(const std::string& level)
{
	this->entityManager.destroyEntities();

	this->map.load("Resources/Files/" + level + ".tmx");

	if (!this->stateData.games.front().getLoadedLevels()[level])
	{
		this->entityManager.createBlueprint("Resources/Files/Blueprint-" + level + ".txt");
		this->stateData.games.front().getLoadedLevels()[level] = true;
	}
	else
	{
		this->entityManager.createEntities("Resources/Files/Entities-" + this->stateData.games.front().getName() + '-' + level + ".txt");
	}

	this->stateData.games.front().setCurrentLevel(level);

	this->healthBar.setHitpointsDisplay(this->entityManager.getEntities().get_entities<ControllableComponent, HealthComponent>()
		.back().get_component<HealthComponent>().getHitpoints());

	this->camera = this->stateData.window.getDefaultView();
	this->stateData.soundManager.stopAllSounds();
}

void GameState::setDisplayHitpoints(Entity entity)
{
	if (entity.has_component<ControllableComponent>())
	{
		this->healthBar.setHitpointsDisplay(entity.get_component<HealthComponent>().getHitpoints());
	}
}

void GameState::saveData(const std::string& pathFile)
{
	this->entityManager.saveEntities("Resources/Files/Entities-" + this->stateData.games.front().getName() + '-' + this->stateData.games.front().getCurrentLevel() + ".txt");
	
	std::ofstream outFile(pathFile);

	for (const auto& game : this->stateData.games)
	{
		outFile << game << '\n';
	}
}