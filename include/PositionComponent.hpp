/*
Copyright (c) 2017 InversePalindrome
Nihil - PositionComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"

#include <SFML/System/Vector2.hpp>


class PositionComponent : public Component
{
public:
	PositionComponent();
	PositionComponent(float xPosition, float yPosition);

	virtual std::ostream& operator<<(std::ostream& os) override;

	sf::Vector2f getPosition() const;

	void setPosition(const sf::Vector2f& position);

	void move(const sf::Vector2f& displacement);

private:
	sf::Vector2f position;
};