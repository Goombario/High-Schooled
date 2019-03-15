#include "ShareState.h"
#include <fstream>
#include "Schooled.h"
#include "input/RawInputConstants.h"

using std::ifstream;
using std::to_string;

namespace shared
{
	std::vector<std::string> getRoomNames()
	{
		std::vector<std::string> roomNameList;
		int counter = 1;
		ifstream stream;
		std::string filename;
		bool good = true;

		while (good)
		{
			filename = "Level_" + to_string(counter) + ".txt";
			stream.open(schooled::getResourcePath("rooms") + filename);
			if (stream.good())
			{
				roomNameList.push_back(filename);
				counter++;
			}
			else
				good = false;
			stream.close();
		}
		return roomNameList;
	}

	void initValidKeys(std::vector<FzlKey>& validKeys)
	{
		// Add all keys to the valid key list
		for (auto it = InputMapping::FzlMap.begin(); it != InputMapping::FzlMap.end(); it++)
		{
			validKeys.push_back((*it).second);
		}
	}

	void initPreviouslyPressed(std::map<FzlKey, bool>& previouslyPressed, std::vector<FzlKey> const& validKeys)
	{
		for (FzlKey key : validKeys)
		{
			previouslyPressed[key] = false;
		}
	}

	void fillPreviouslyPressed(std::map<FzlKey, bool>& previouslyPressed, std::vector<FzlKey> const& validKeys)
	{
		for (FzlKey key : validKeys)
		{
			previouslyPressed.at(key) = true;
		}
	}
}