//
// Input Mapping Demo
// By Mike Lewis, June 2011
// http://scribblings-by-apoch.googlecode.com/
//
// Wrapper class for converting raw input range values to sensitivity-calibrated range values
//
// Modified by Graham Watson

#include "pch.h"

#include "RangeConverter.h"
#include "FileIO.h"
#include "tinyxml2.h"

#include <fstream>


using namespace InputMapping;


//
// Construct the converter and load the conversion table provided
//
RangeConverter::RangeConverter(tinyxml2::XMLNode * contextRoot)
{

	tinyxml2::XMLDocument ranges;
	tinyxml2::CheckXMLResult(ranges.LoadFile("InputRanges.xml"));
	tinyxml2::XMLNode *rangesRoot = ranges.RootElement();
	tinyxml2::XMLElement *rangesElement;
	tinyxml2::XMLElement *contextElement;

	contextElement = contextRoot->FirstChildElement("Range");
	while (contextElement != nullptr)
	{
		rangesElement = rangesRoot->FirstChildElement();

		// For some reason, they don't agree with each other, so this is necessary.
		while (static_cast<std::string>(rangesElement->Attribute("name"))
			!= static_cast<std::string>(contextElement->GetText()))
		{
			rangesElement = rangesElement->NextSiblingElement();
			if (rangesElement == nullptr)
			{
				std::cerr << "Error: Range data not found in range " << contextElement->GetText() << std::endl;
				exit(-1);
			}
		}

		std::string rangeName = contextElement->GetText();	// Gets the name of the range from the context
		Range range = RangeLookup.at(rangeName);	// Gets the high level Range ID

		Converter converter;
		tinyxml2::CheckXMLResult(rangesElement->FirstChildElement("MinimumInput")->QueryDoubleText(&converter.MinimumInput));
		tinyxml2::CheckXMLResult(rangesElement->FirstChildElement("MaximumInput")->QueryDoubleText(&converter.MaximumInput));
		tinyxml2::CheckXMLResult(rangesElement->FirstChildElement("MinimumOutput")->QueryDoubleText(&converter.MinimumOutput));
		tinyxml2::CheckXMLResult(rangesElement->FirstChildElement("MaximumOutput")->QueryDoubleText(&converter.MaximumOutput));

		if ((converter.MaximumInput < converter.MinimumInput) || (converter.MaximumOutput < converter.MinimumOutput))
			throw std::exception("Invalid input range conversion");

		ConversionMap.insert(std::make_pair(range, converter));

		contextElement = contextElement->NextSiblingElement("Range");
	}
}


