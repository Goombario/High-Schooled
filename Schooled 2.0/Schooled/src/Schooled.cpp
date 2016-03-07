#include "Schooled.h"
#include <fstream>
#include <iostream>



std::string schooled::getSetting(std::string a_key)
{
	// Load settings from the file
	std::ifstream stream("Settings.txt");
	std::string line;

	if (!stream)
	{
		std::cerr << "File open failed. (schooled::getSetting)" << std::endl;
		exit(1);
	}

	// Go through the file, if it finds the matching key return its value
	while (std::getline(stream, line))
	{
		if (line.substr(0, line.find(':')) == a_key)
		{
			return line.substr(line.find(':') + 2);
		}
	}

	return "";
}

std::string schooled::getResourcePath(const std::string &subDir)
{
	return "../res/" + subDir + "/";
}