/*
Copyright (c) 2017 InversePalindrome
Nihil - PhysicsSystem.cpp
InversePalindrome.com
*/


#include "PhysicsSystem.hpp"
#include "EntityEvents.hpp"
#include "PhysicsComponent.hpp"
#include "UnitConverter.hpp"


PhysicsSystem::PhysicsSystem(Entities& entities, Events& events, b2World& world) :
	System(entities, events),
	world(world)
{
	events.subscribe<DirectionChanged>([this](const auto& event) { moveEntity(event.direction); });
}

void PhysicsSystem::update(float deltaTime)
{
	this->entities.for_each<PhysicsComponent, PositionComponent>(
		[this](auto entity, const auto& physics, auto& position)
	{
		this->convertPositionCoordinates(physics, position);
	});
}

void PhysicsSystem::moveEntity(Direction direction)
{
	auto& physics = this->entities.get_entities<Controllable, PhysicsComponent>().back()
		.get_component<PhysicsComponent>();

	const auto& currentVelocity = physics.getVelocity();
	
	b2Vec2 newVelocity(0.f, 0.f);
	
	switch (direction)
	{
	case Direction::Right:
		newVelocity.x = b2Min(currentVelocity.x + physics.getAccelerationRate(), physics.getMaxVelocity());
		break;
	case Direction::Left:
		newVelocity.x = b2Max(currentVelocity.x - physics.getAccelerationRate(), -physics.getMaxVelocity());
		break;
	case Direction::Up:
		const float jumpVelocity = 1.5 * physics.getMaxVelocity();
		newVelocity.y = jumpVelocity;
		break;
	}

	const auto& deltaVelocity = newVelocity - currentVelocity;
	const auto& impulse = b2Vec2(deltaVelocity.x * physics.getMass(), deltaVelocity.y * physics.getMass());
	
	physics.applyImpulse(impulse);
}

void PhysicsSystem::convertPositionCoordinates(const PhysicsComponent& physics, PositionComponent& position)
{
	position.setPosition(sf::Vector2f(UnitConverter::metersToPixels(physics.getPosition().x), UnitConverter::metersToPixels(-physics.getPosition().y)));
}