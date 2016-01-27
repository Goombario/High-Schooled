#ifndef SHARESTATE_H
#define SHARESTATE_H

#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include <vector>
#include <string>
#include <map>
using std::string;
using std::vector;

namespace shared
{
	// Get the list of room filenames
	vector<string> getRoomNames();

	// Initializes a vector of available keys
	void initValidKeys(std::vector<FzlKey>& validKeys);

	// Initializes a map of available keys for hold checking
	void initPreviouslyPressed(std::map<FzlKey, bool>& previouslyPressed, const std::vector<FzlKey>& validKeys);
}


#endif