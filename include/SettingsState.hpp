/*
Copyright (c) 2017 InversePalindrome
Nihil - SettingsState.hpp
InversePalindrome.com
*/


#pragma once

#include "State.hpp"

#include <SFGUI/Label.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/RadioButtonGroup.hpp>

#include <SFML/Graphics/Sprite.hpp>


class SettingsState : public State
{
public:
	SettingsState(StateMachine& stateMachine, StateData& stateData);

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
	sf::Sprite background;

	sfg::Button::Ptr backButton;
	sfg::Label::Ptr soundLabel;
	sfg::Label::Ptr musicLabel;
	sfg::Scale::Ptr soundVolumeScale;
	sfg::Scale::Ptr musicVolumeScale;
	sfg::Adjustment::Ptr soundVolumeAdjustment;
	sfg::Adjustment::Ptr musicVolumeAdjustment;
	sfg::Scrollbar::Ptr soundScrollbar;
	sfg::Scrollbar::Ptr musicScrollbar;
	sfg::RadioButtonGroup::Ptr keyButtons;
	sfg::RadioButton::Ptr moveRightButton;
	sfg::RadioButton::Ptr moveLeftButton;
	sfg::RadioButton::Ptr jumpButton;

	void adjustSoundVolume();
	void adjustMusicVolume();
	void changeKeyBinding(sf::Keyboard::Key key);

	void transitionToMenu();
};