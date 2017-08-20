/*
Copyright (c) 2017 InversePalindrome
Nihil - Application.hpp
InversePalindrome.com
*/


#pragma once

#include "ECS.hpp"
#include "StateData.hpp"
#include "StateMachine.hpp"
#include "InputHandler.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"
#include "GUIManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


class Application
{
public:
	Application();
	Application(const Application& application) = delete;
	Application& operator=(const Application& application) = delete;

	void run();

private:
	InputHandler inputHandler;

	ResourceManager resourceManager;
	SoundManager soundManager;
	GUIManager guiManager;

	Events eventManager;

	sf::RenderWindow window;

	StateData stateData;
	StateMachine stateMachine;

	void handleEvents();
	void update(float deltaTime);
	void render();
};