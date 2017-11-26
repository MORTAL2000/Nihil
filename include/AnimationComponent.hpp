/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationComponent.hpp
InversePalindrome.com
*/


#pragma once

#include "Direction.hpp"
#include "Component.hpp"
#include "StateComponent.hpp"

#include <Thor/Animations/Animator.hpp>
#include <Thor/Animations/FrameAnimation.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <map>
#include <optional>


class AnimationComponent : public Component
{
	friend std::ostream& operator<<(std::ostream& os, const AnimationComponent& component);
	using Animation = std::pair<EntityState, Direction>;

public:
	AnimationComponent(bool hasMultipleAnimations, const std::string& animationsFile);

	void setAnimations(bool hasMultipleAnimations, const std::string& animationsFile);

	std::optional<std::pair<EntityState, Direction>> getCurrentAnimation() const;

	void update(float deltaTime);
	void animate(sf::Sprite& sprite);

	void playAnimation(EntityState state, Direction direction, bool loop);

	bool isPlayingAnimation() const;
	bool hasAnimation(const Animation& animation) const;

private:
	std::string animationsFile;
	bool hasMultipleAnimations;
	std::map<Animation, std::pair<thor::FrameAnimation, float>> animations;
	thor::Animator<sf::Sprite, std::pair<EntityState, Direction>> stateAnimator;
	thor::Animator<sf::Sprite, std::size_t> singleAnimator;
};

std::ostream& operator<<(std::ostream& os, const AnimationComponent& component);