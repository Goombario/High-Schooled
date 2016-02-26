#ifndef SHARESTATE_H
#define SHARESTATE_H

#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include <vector>
#include <string>
#include <map>

// Functions used by multiple states
namespace shared
{
	// Get the list of room filenames
	std::vector<std::string> getRoomNames();

	// Initializes a vector of available keys
	void initValidKeys(std::vector<FzlKey>& validKeys);

	// Initializes a map of available keys for hold checking
	void initPreviouslyPressed(std::map<FzlKey, bool>& previouslyPressed, const std::vector<FzlKey>& validKeys);
}


#endif