/*
Copyright (c) 2017 InversePalindrome
Nihil - MenuState.cpp
InversePalindrome.com
*/


#include "MenuState.hpp"
#include "StateMachine.hpp"

#include <SFGUI/Image.hpp>


MenuState::MenuState(StateMachine& stateMachine, StateData& stateData) :
	State(stateMachine, stateData),
	playButton(sfg::Button::Create()),
	settingsButton(sfg::Button::Create()),
	charactersButton(sfg::Button::Create())
{
	playButton->SetImage(sfg::Image::Create(stateData.resourceManager.getImage(ImagesID::PlayButton)));
	playButton->SetPosition(sf::Vector2f(740.f, 820.f));
	playButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToPlay(); });
	
	settingsButton->SetImage(sfg::Image::Create(stateData.resourceManager.getImage(ImagesID::SettingsButton)));
	settingsButton->SetPosition(sf::Vector2f(740.f, 1000.f));
	settingsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToSettings(); });
	
	charactersButton->SetImage(sfg::Image::Create(stateData.resourceManager.getImage(ImagesID::CharactersButton)));
	charactersButton->SetPosition(sf::Vector2f(740.f, 1180.f));
	charactersButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] { transitionToCharacters(); });
	
	stateData.guiManager.setProperty("Button", "BackgroundColor", sf::Color::Transparent);
	stateData.guiManager.setProperty("Button", "BorderColor", sf::Color::Transparent);
	stateData.guiManager.setProperty("Button:PRELIGHT", "BackgroundColor", sf::Color::Transparent);

	stateData.guiManager.addWidget(playButton);
	stateData.guiManager.addWidget(settingsButton);
	stateData.guiManager.addWidget(charactersButton);
}

void MenuState::handleEvent(const sf::Event& event)
{

}

void MenuState::update(float deltaTime)
{
	
}

void MenuState::draw()
{
}

void MenuState::transitionToPlay()
{
	this->playButton->Show(false);
	this->settingsButton->Show(false);
	this->charactersButton->Show(false);

	this->stateMachine.pushState(StateID::Game);
}

void MenuState::transitionToSettings()
{
	this->playButton->Show(false);
	this->settingsButton->Show(false);
	this->charactersButton->Show(false);

	this->stateMachine.pushState(StateID::Settings);
}

void MenuState::transitionToCharacters()
{
	this->playButton->Show(false);
	this->settingsButton->Show(false);
	this->charactersButton->Show(false);

	this->stateMachine.pushState(StateID::Characters);
}