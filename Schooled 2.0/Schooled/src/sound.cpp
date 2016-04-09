#include "Sound.h"
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "GameEngine.h"
#include <iostream>

namespace FMOD_util
{
	int FMODErrorCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			std::cerr << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
			exit(-1);
			return -1;
		}
		return 0;
	}
}

using FMOD_util::FMODErrorCheck;

namespace Sound
{
	Instance::Instance() {};

	Instance::Instance(Description *desc)
	{
		FMODErrorCheck(desc->getDesc()->createInstance(&instance));
	};

	Instance::~Instance() 
	{
		release();
	};

	int Instance::start()
	{
		return (FMODErrorCheck(instance->start()));
	};

	int Instance::stop(FMOD_STUDIO_STOP_MODE mode)
	{
		return (FMODErrorCheck(instance->stop(mode)));
	};

	int Instance::release()
	{
		return FMODErrorCheck(instance->release());
	}


	Description::Description() {};

	Description::Description(std::string eventName, bool loadSample)
	{
		FMOD::Studio::System* system = GameEngine::getSoundSystem();
		FMODErrorCheck(system->getEvent(("event:/" + eventName).c_str(), &description));

		// Load sample data into memory
		if (loadSample)
		{
			FMODErrorCheck(description->loadSampleData());
		}
	};

	Description::~Description()
	{
		FMODErrorCheck(description->releaseAllInstances());
	};
}
