/*
Copyright (c) 2017 InversePalindrome
Nihil - StateComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Component.hpp"
#include "EntityState.hpp"


class StateComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const StateComponent& component);

public:
	StateComponent();

	EntityState getState() const;

	void setState(EntityState state);

private:
	EntityState state;
};

std::ostream& operator<<(std::ostream& os, const StateComponent& component);
