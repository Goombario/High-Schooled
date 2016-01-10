#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H

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


// Manipulating sound
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
		int stop(FMOD_STUDIO_STOP_MODE mode);

		// release the event
		int release();

	private:
		FMOD::Studio::EventInstance *instance = nullptr;
	};

	//// Pointer to Sound object
	//typedef Sound* SndPtr;
}


#endif