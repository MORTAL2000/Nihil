/*
Copyright (c) 2017 InversePalindrome
Nihil - AnimationParser.hpp
InversePalindrome.com
*/


#pragma once

#include <Thor/Animations/FrameAnimation.hpp>
#include <Thor/Animations/ColorAnimation.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <string>
#include <fstream>
#include <sstream>


namespace Parsers
{
	void parseFrameAnimations(const std::string& pathFile, thor::FrameAnimation& animation,
		std::size_t& animationID, float& animationTime);

	thor::ColorGradient parseColors(const std::string& pathFile);
}