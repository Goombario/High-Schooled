//
// Input Mapping Demo
// By Mike Lewis, June 2011
// http://scribblings-by-apoch.googlecode.com/
//
// Wrapper class for managing input contexts
//
// Modified by Graham Watson

#include "pch.h"
#include "InputContext.h"
#include "tinyxml2.h"
#include "FileIO.h"

#include <fstream>


using namespace InputMapping;


//
// Construct and initialize an input context given data in a file
//
InputContext::InputContext(const std::string& contextName)
	: Conversions(NULL)
{
	// Load the context
	tinyxml2::XMLDocument contexts;
	tinyxml2::CheckXMLResult(contexts.LoadFile("Contexts.xml"));
	tinyxml2::XMLNode *contextRoot = contexts.RootElement()->FirstChild();
	tinyxml2::XMLElement *contextElement;

	// Check if context exists
	while (contextRoot->ToElement()->Attribute("name") != contextName)
	{
		contextRoot = contextRoot->NextSiblingElement();
		if (contextRoot == nullptr)
		{
			tinyxml2::CheckXMLResult(tinyxml2::XML_ERROR_PARSING_ATTRIBUTE);
			exit(-1);
		}
	}
	

	// Load the input map
	tinyxml2::XMLDocument inputs;
	tinyxml2::CheckXMLResult(inputs.LoadFile("InputMap.xml"));
	tinyxml2::XMLNode *inputRoot = inputs.RootElement()->FirstChild();
	tinyxml2::XMLElement *inputElement;

	// Get the correct context from the input map
	while (inputRoot->ToElement()->Attribute("name") != contextName)
	{
		inputRoot = inputRoot->NextSiblingElement();
		if (inputRoot == nullptr)
		{
			tinyxml2::CheckXMLResult(tinyxml2::XML_ERROR_PARSING_ATTRIBUTE);
			exit(-1);
		}
	}


	// Gets all the actions
	contextElement = contextRoot->FirstChildElement("Action");
	inputElement = inputRoot->FirstChildElement("Action");
	while (contextElement != nullptr)
	{
		// Gets the action name
		std::string actionName = contextElement->GetText();	// Gets the name of the action from the context
		Action action = ActionLookup.at(actionName);	// Gets the high level Action ID
		
		// Checks to make sure the correct action is selected in the input map
		if (inputElement->FirstChildElement("Input")->GetText() != actionName)
		{
			inputElement = inputRoot->FirstChildElement("Action");
			while (inputElement->FirstChildElement("Input")->GetText() != actionName)
			{
				inputElement = inputElement->NextSiblingElement("Action");
				if (inputElement == nullptr)
				{
					std::cerr << "Error: Action not found in context " << contextName << std::endl;
					exit(-1);
				}
			}
		}

		// Sets the Raw ID to the action
		std::string buttonKey = inputElement->FirstChildElement("Raw")->GetText();	// Gets the name of the raw input ID
		FzlKey button = InputMapping::FzlMap.at(buttonKey);	// Gets the raw ID
		
		// Sends the data to the action map
		ActionMap[button] = action;

		// Get the next action
		contextElement = contextElement->NextSiblingElement("Action");
		inputElement = inputElement->NextSiblingElement("Action");
	}
	
	// Gets all the states
	contextElement = contextRoot->FirstChildElement("State");
	inputElement = inputRoot->FirstChildElement("State");
	while (contextElement != nullptr)
	{
		// Gets the state name
		std::string stateName = contextElement->GetText();	// Gets the name of the state from the context
		State state = StateLookup.at(stateName);	// Gets the high level State ID

		// Checks to make sure the correct state is selected in the input map
		if (inputElement->FirstChildElement("Input")->GetText() != stateName)
		{
			inputElement = inputRoot->FirstChildElement("State");
			while (inputElement->FirstChildElement("Input")->GetText() != stateName)
			{
				inputElement = inputElement->NextSiblingElement("State");
				if (inputElement == nullptr)
				{
					std::cerr << "Error: State not found in context " << contextName << std::endl;
					exit(-1);
				}
			}
		}

		// Sets the Raw ID to the state
		std::string buttonKey = inputElement->FirstChildElement("Raw")->GetText();	// Gets the name of the raw input ID
		FzlKey button = InputMapping::FzlMap.at(buttonKey);	// Gets the raw ID

		// Sends the data to the state map
		StateMap[button] = state;

		// Get the next state
		contextElement = contextElement->NextSiblingElement("State");
		inputElement = inputElement->NextSiblingElement("State");
	}

	// Gets all the ranges
	contextElement = contextRoot->FirstChildElement("Range");
	inputElement = inputRoot->FirstChildElement("Range");
	while (contextElement != nullptr)
	{
		// Gets the range name
		std::string rangeName = contextElement->GetText();	// Gets the name of the range from the context
		Range range = RangeLookup.at(rangeName);	// Gets the high level Range ID

		// Checks to make sure the correct range is selected in the input map
		if (inputElement->FirstChildElement("Input")->GetText() != rangeName)
		{
			inputElement = inputRoot->FirstChildElement("Range");
			while (inputElement->FirstChildElement("Input")->GetText() != rangeName)
			{
				inputElement = inputElement->NextSiblingElement("Range");
				if (inputElement == nullptr)
				{
					std::cerr << "Error: Range not found in context " << contextName << std::endl;
					exit(-1);
				}
			}
		}

		// Sets the Raw ID to the range
		std::string axisKey = inputElement->FirstChildElement("Raw")->GetText();	// Gets the name of the raw input ID
		RawInputAxis axis = InputMapping::RawInputAxisMap.at(axisKey);	// Gets the raw ID

		// Sends the data to the range map
		RangeMap[axis] = range;

		double sensitivity;
		tinyxml2::CheckXMLResult(inputElement->FirstChildElement("Sensitivity")->QueryDoubleText(&sensitivity));
		SensitivityMap[range] = sensitivity;

		// Get the next range
		contextElement = contextElement->NextSiblingElement("Range");
		inputElement = inputElement->NextSiblingElement("Range");
	}

	Conversions = new RangeConverter(contextRoot);
}

//
// Destruct and clean up an input context
//
InputContext::~InputContext()
{
	delete Conversions;
}


//
// Attempt to map a raw button to an action
//
bool InputContext::MapButtonToAction(FzlKey button, Action& out) const
{
	std::map<FzlKey, Action>::const_iterator iter = ActionMap.find(button);
	if(iter == ActionMap.end())
		return false;

	out = iter->second;
	return true;
}

//
// Attempt to map a raw button to a state
//
bool InputContext::MapButtonToState(FzlKey button, State& out) const
{
	std::map<FzlKey, State>::const_iterator iter = StateMap.find(button);
	if(iter == StateMap.end())
		return false;

	out = iter->second;
	return true;
}

//
// Attempt to map a raw axis to a range
//
bool InputContext::MapAxisToRange(RawInputAxis axis, Range& out) const
{
	std::map<RawInputAxis, Range>::const_iterator iter = RangeMap.find(axis);
	if(iter == RangeMap.end())
		return false;

	out = iter->second;
	return true;
}


//
// Retrieve the sensitivity associated with a given range
//
double InputContext::GetSensitivity(Range range) const
{
	std::map<Range, double>::const_iterator iter = SensitivityMap.find(range);
	if(iter == SensitivityMap.end())
		return 1.0;

	return iter->second;
}

