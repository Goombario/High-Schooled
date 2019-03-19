#include "TutorialState.h"
#include "BattleState.h"
#include "GameEngine.h"
#include "ShareState.h"
#include "Fizzle\Fizzle.h"
#include "Input\InputMapper.h"
#include "tinyxml2.h"

#include "Sprite.h"
#include "Tutorial.h"
#include "Sprite.h"
#include "Image.h"

#include <fstream>

using namespace tinyxml2;
using std::string;

namespace TutorialState
{
	TutorialState TutorialState::m_TutorialState;

	void TutorialState::Init()
	{
		// Initialize the mapper context
		// Tells the mapper to map a specific set of keys to a specific set of actions
		mapper = new InputMapping::InputMapper();
		mapper->PushContext("globalContext");
		mapper->PushContext("altTutorial");
		//mapper->PushContext("tutorial");

		// Tells the mapper to call the given function after the contexts have been mapped.
		mapper->AddCallback(mainCallback, 0);

		// Hold valid keys
		validKeys.clear();
		previouslyPressed.clear();
		shared::initValidKeys(validKeys);

		// Hold pressed keys
		shared::initPreviouslyPressed(previouslyPressed, validKeys);

		isEnd = false;
		loadData();
		p1Tut = new Tutorial::Tutorial("Nate", Side::LEFT);
		p2Tut = new Tutorial::Tutorial("Gym Teacher", Side::RIGHT);
	}

	void TutorialState::Cleanup()
	{
		// Cleanup all pointers
		delete background;
		delete p1Tut;
		delete p2Tut;
		delete mapper;
	}

	void TutorialState::Pause()
	{
		// Suspend sounds and potentially pop contexts
		mapper->PopContext();
		mapper->PopContext();
	}

	void TutorialState::Resume()
	{
		// Resume sounds and push contexts
		mapper->PushContext("globalContext");
		mapper->PushContext("altTutorial");
	}

	void TutorialState::HandleEvents(GameEngine* game)
	{
		// Go through all valid keys and set if they are down or not, and if they have been previously down.
		for (FzlKey key : validKeys)
		{
			if (FzlGetKey(key))
			{
				bool previouslyDown = previouslyPressed[key];
				previouslyPressed[key] = true;
				mapper->SetRawButtonState(key, true, previouslyDown);
			}
			else
			{
				previouslyPressed[key] = false;
				mapper->SetRawButtonState(key, false, true);
			}
		}
		
		mapper->Dispatch();
		mapper->Clear();
	}

	void mainCallback(InputMapping::MappedInput& inputs)
	{
		TutorialState *self = TutorialState::Instance();

		if (inputs.Actions.find(InputMapping::Action::EXIT_GAME) != inputs.Actions.end())
		{
			self->isEnd = true;
		}

		if (inputs.Actions.find(InputMapping::Action::P1_ADVANCE) != inputs.Actions.end())
		{
			self->p1Tut->moveSlide(1);
			self->p2Tut->moveSlide(1);
			inputs.EatAction(InputMapping::Action::P1_ADVANCE);
		}
		if (inputs.Actions.find(InputMapping::Action::P1_BACK) != inputs.Actions.end())
		{
			self->p1Tut->moveSlide(-1);
			self->p2Tut->moveSlide(-1);
			self->p1Tut->setReady(false);
			self->p2Tut->setReady(false);
			inputs.EatAction(InputMapping::Action::P1_BACK);
		}
		if (inputs.Actions.find(InputMapping::Action::P1_READY) != inputs.Actions.end())
		{
			self->p1Tut->setReady();
			self->p2Tut->setReady();
			inputs.EatAction(InputMapping::Action::P1_READY);
		}
	}

	void TutorialState::Update(GameEngine* game)
	{
		if (isEnd)
		{
			game->Quit();
		}

		p1Tut->update();
		p2Tut->update();

		if (p1Tut->isReady() && p2Tut->isReady())
		{
			game->PopState();
		}
	}

	void TutorialState::Draw(GameEngine* game)
	{
		background->draw();
		p1Tut->draw();
		p2Tut->draw();
		// Fizzle swaps buffer automatically at end
	}

	void TutorialState::loadData()
	{
		std::ifstream fileStream("Save.txt");

		std::string player1Name, player2Name, stageName;
		std::getline(fileStream, player1Name);
		std::getline(fileStream, player2Name);
		std::getline(fileStream, stageName);

		// Load player data from player file
		tinyxml2::XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/StageData.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Stage data file: "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		XMLElement *stageData;
		stageData = pRoot->FirstChildElement("Stage");

		// Check if stage data loaded
		std::string stageDataName = stageData->Attribute("name");
		while (stageDataName != stageName)
		{
			stageData = stageData->NextSiblingElement();
			if (stageData == nullptr)
			{
				std::cerr << "ERROR: Loading stageData: "
					<< XML_ERROR_FILE_READ_ERROR << std::endl;
				exit(-2);
			}
			stageDataName = stageData->Attribute("name");
		}

		XMLElement *stageElement = stageData->FirstChildElement("Background");
		if (stageElement == nullptr)
		{
			std::cerr << "ERROR: Loading Stage data file: Background "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		background = new Sprite::Sprite(stageElement);
		background->move(0, 0, false);

		fileStream.close();
	}
}

