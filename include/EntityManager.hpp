/*
Copyright (c) 2017 InversePalindrome
Nihil - EntityManager.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "System.hpp"
#include "InputHandler.hpp"
#include "CollisionData.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"
#include "ComponentParser.hpp"

#include <Box2D/Dynamics/b2World.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <tuple>
#include <string>
#include <memory>
#include <unordered_map>


class EntityManager 
{
public:
	EntityManager(Events& events, b2World& world, ResourceManager& resourceManager, SoundManager& soundManager, InputHandler& inputHandler, CollisionsData& collisionData);
	EntityManager(const EntityManager& entityManager) = delete;
    EntityManager& operator=(const EntityManager& entityManager) = delete;

	Entities& getEntities();
	Events& getEvents();

	void update(float deltaTime);
	void draw(sf::RenderTarget& target);

	void createEntity(const std::string& filePath);
	void createEntity(const std::string& filePath, const sf::Vector2f& position);
	void createEntities(const std::string& filePath);

	void destroyEntity(Entity entity);
	void destroyEntities();

private:
	Entities entityManager;
	Events& eventManager;

	b2World& world;

	ComponentParser componentParser;

	std::unordered_map<std::string, std::unique_ptr<System>> systems;
};