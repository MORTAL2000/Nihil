/*
Copyright (c) 2017 InversePalindrome
Nihil - ComponentParser.cpp
InversePalindrome.com
*/


#include "ComponentParser.hpp"
#include "UnitConverter.hpp"

#include <brigand/algorithms/for_each.hpp>

#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <fstream>


ComponentParser::ComponentParser(Entities& entities, ResourceManager& resourceManager, b2World& world) :
	entities(entities),
	world(world),
	currentEntityID(0u)
{
	componentParsers["Controllable"] = [this](auto& entity, auto& line)
	{
		entity.set_tag<Controllable>(true);
	};

	componentParsers["Pickup"] = [this](auto& entity, auto& line)
	{
		entity.set_tag<Pickup>(true);
	};
	
	componentParsers["PositionA"] = [this](auto& entity, auto& line) 
	{
		entity.add_component<PositionComponent>(std::make_from_tuple<PositionComponent>(this->parse<float, float>(line)));
	};

	componentParsers["PositionB"] = [this](auto& entity, auto& line)
	{
		entity.add_component<PositionComponent>();
	};


	componentParsers["State"] = [this](auto& entity, const auto& line)
	{
		entity.add_component<StateComponent>();
	};

	componentParsers["PhysicsA"] = [this, &world](auto& entity, auto& line)
	{
		auto& params = this->parse<float, float, std::size_t, std::size_t, std::int32_t, float, float>(line);

		entity.add_component<PhysicsComponent>(world, b2Vec2(std::get<0>(params), std::get<1>(params)),
			static_cast<b2BodyType>(std::get<2>(params)), static_cast<ObjectType>(std::get<3>(params)), std::get<4>(params), std::get<5>(params), std::get<6>(params));
	};

	componentParsers["PhysicsB"] = [this, &world](auto& entity, auto& line)
	{
		auto& params = this->parse<float, float, std::size_t, std::size_t, std::int32_t>(line);

		entity.add_component<PhysicsComponent>(world, b2Vec2(std::get<0>(params), std::get<1>(params)),
			static_cast<b2BodyType>(std::get<2>(params)), static_cast<ObjectType>(std::get<3>(params)), std::get<4>(params));
	};

	componentParsers["AI"] = [this](auto& entity, auto& line)
	{
		entity.add_component<AIComponent>(std::make_from_tuple<AIComponent>(this->parse<float, float, float>(line)));
	};

	componentParsers["SpriteA"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = this->parse<std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, float, float>(line);

		entity.add_component<SpriteComponent>(resourceManager, static_cast<TexturesID>(std::get<0>(params)),
			sf::IntRect(std::get<1>(params), std::get<2>(params), std::get<3>(params), std::get<4>(params)), sf::Vector2f(std::get<5>(params), std::get<6>(params)));
	};

	componentParsers["SpriteB"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = this->parse<std::size_t, std::size_t, std::size_t, std::size_t, std::size_t>(line);

		entity.add_component<SpriteComponent>(resourceManager, static_cast<TexturesID>(std::get<0>(params)),
			sf::IntRect(std::get<1>(params), std::get<2>(params), std::get<3>(params), std::get<4>(params)));
	};

	componentParsers["SpriteC"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = this->parse<std::size_t>(line);

		entity.add_component<SpriteComponent>(resourceManager, static_cast<TexturesID>(std::get<0>(params)));
	};

	componentParsers["Health"] = [this](auto& entity, auto& line)
	{
		entity.add_component<HealthComponent>(std::make_from_tuple<HealthComponent>(this->parse<std::size_t>(line)));
	};

	componentParsers["Attack"] = [this](auto& entity, auto& line)
	{
		entity.add_component<AttackComponent>(std::make_from_tuple<AttackComponent>(this->parse<std::size_t>(line)));
	};

	componentParsers["Animation"] = [this](auto& entity, auto& line)
	{
		entity.add_component<AnimationComponent>(std::make_from_tuple<AnimationComponent>(this->parse<std::string>(line)));
	};

	componentParsers["Sound"] = [this](auto& entity, auto& line)
	{
		entity.add_component<SoundComponent>();
	};

	componentParsers["Particle"] = [this, &resourceManager](auto& entity, auto& line)
	{
		auto& params = this->parse<std::string>(line);
		
		entity.add_component<ParticleComponent>(std::get<0>(params), resourceManager);
	};
	
	componentParsers["Parent"] = [this](auto& entity, auto& line)
	{
		entity.add_component<ParentComponent>(std::make_from_tuple<ParentComponent>(this->parse<std::size_t>(line)));
	};

	componentParsers["Child"] = [this](auto& entity, auto& line)
	{
		entity.add_component<ChildComponent>(std::make_from_tuple<ChildComponent>(this->parse<std::size_t>(line)));
	};

	componentParsers["Automated"] = [this](auto& entity, auto& line)
	{
		auto& params = this->parse<std::string>(line);

		entity.add_component<AutomatedComponent>(std::get<0>(params));
	};
}

Entity ComponentParser::parseComponents(const std::string& pathFile)
{
	auto entity = this->createEntity();

	std::ifstream inFile(pathFile);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string componentName;

		iStream >> componentName;

		line.erase(std::begin(line), std::begin(line) + componentName.size());
		boost::remove_erase_if(line, boost::is_any_of(",()"));
		
		this->componentParsers[componentName](entity, line);
	}

	brigand::for_each<ComponentList>([&entity, this](auto componentType)
	{
		using Component = decltype(componentType)::type;

		if (entity.has_component<Component>())
		{
			entity.get_component<Component>().setEntity(currentEntityID);
		}
	});

	return entity;
}

void ComponentParser::parseBlueprint(const std::string& pathFile)
{
	std::ifstream inFile(pathFile);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::string entityFile;
		float xPosition = 0.f, yPosition = 0.f;

		iStream >> entityFile >> xPosition >> yPosition;
		
		auto entity = this->parseComponents(entityFile);

		if (entity.has_component<PositionComponent>())
		{
			entity.get_component<PositionComponent>().setPosition(sf::Vector2f(xPosition, yPosition));
		}
		if (entity.has_component<PhysicsComponent>())
		{
			entity.get_component<PhysicsComponent>().setPosition(
				b2Vec2(UnitConverter::pixelsToMeters(xPosition), UnitConverter::pixelsToMeters(-yPosition)));
		}
	}
}

void ComponentParser::parseEntities(const std::string& pathFile)
{
	std::ifstream inFile(pathFile);
	std::string line;

	std::unordered_map<std::size_t, Entity> entitiesIDs;

	while (std::getline(inFile, line))
	{
		std::istringstream iStream(line);

		std::size_t entityID = 0u;
		std::string componentName;

		iStream >> entityID >> componentName;

		Entity entity;

		if (entitiesIDs.count(entityID))
		{
			entity = entitiesIDs[entityID];
			entity.sync();
		}
		else
		{
			entity = this->createEntity();
			entitiesIDs.emplace(entityID, entity);
		}

		line.erase(std::begin(line), std::begin(line) + std::to_string(entityID).size() + componentName.size() + 1u);
	
		this->componentParsers[componentName](entity, line);
	}
}

Entity ComponentParser::createEntity()
{
	++this->currentEntityID;

	return this->entities.create_entity();
}