/*
Copyright (c) 2017 InversePalindrome
Nihil - EntityManager.cpp
InversePalindrome.com
*/


#include "EntityManager.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
#include "ControlSystem.hpp"


EntityManager::EntityManager(b2World& world, ResourceManager& resourceManager, InputHandler& inputHandler) :
	resourceManager(resourceManager),
	inputHandler(inputHandler)
{
	entityManager.set_event_manager(eventManager);

	systems["Render"] = std::make_unique<RenderSystem>(entityManager, eventManager);
	systems["Control"] = std::make_unique<ControlSystem>(entityManager, eventManager, inputHandler);
	systems["Physics"] = std::make_unique<PhysicsSystem>(entityManager, eventManager, world);
	
	entityManager.create_entity<Controllable>(PhysicsComponent(world, b2Vec2(5.f, 5.f), b2Vec2(0.f, 0.f), 10.f, 1.f), PositionComponent{ {} }, SpriteComponent{ resourceManager.getTexture(TexturesID::Skeleton) });
}

Entities& EntityManager::getEntities()
{
	return this->entityManager;
}

void EntityManager::update(float deltaTime)
{
	for (auto& system : this->systems)
	{
		system.second->update(deltaTime);
	}
}

void EntityManager::draw(sf::RenderTarget& target)
{
	dynamic_cast<RenderSystem*>(this->systems.at("Render").get())->draw(target);
}