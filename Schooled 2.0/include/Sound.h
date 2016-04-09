#ifndef SOUND_H
#define SOUND_H

#include <string>
#include "fmod_studio_common.h"

namespace FMOD
{
	namespace Studio
	{
		class EventDescription;
		class EventInstance;
	}
}

// Utility namespace
namespace FMOD_util
{
	// Checks for errors. Returns -1 if failed.
	int FMODErrorCheck(FMOD_RESULT result);
}

// Manipulating sound classes
namespace Sound
{
	// Contains the description of the sound event
	class Description
	{
	public:
		// Create an empty Description object
		Description();

		/* Create a SndDescription object containing the description
		* @param eventName	The name of the event to get
		* @param loadSample	If the sample is to be loaded into memory.
		*/
		Description(std::string eventName, bool loadSample = false);

		// Destructor
		~Description();

		// Creates an instance of the description
		FMOD::Studio::EventDescription *getDesc() { return description; }

	private:
		FMOD::Studio::EventDescription *description = nullptr;
	};

	// Contains an instance of a sound event
	class Instance
	{
	public:
		// Create an empty Instance object
		Instance();

		// Create an Instance of a Description
		Instance(Description*);

		// Destructor (release the instance)
		~Instance();

		// Play the sound event
		int start();

		/* Stop the sound event
		* @param mode determines how the sound event stops.
		*/
		int stop(FMOD_STUDIO_STOP_MODE mode = FMOD_STUDIO_STOP_IMMEDIATE);

		// release the event
		int release();

	private:
		FMOD::Studio::EventInstance *instance = nullptr;
	};
}


#endif