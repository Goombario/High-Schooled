#include "sound_effects.h"
#include "fmod_studio.hpp"
#include "FMOD_util.h"
#include "GameEngine.h"

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
