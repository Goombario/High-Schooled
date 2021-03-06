//
// Input Mapping Demo
// By Mike Lewis, June 2011
// http://scribblings-by-apoch.googlecode.com/
//
// Wrapper class for managing input contexts
//

#pragma once


// Dependencies
#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include "RawInputConstants.h"
#include "InputConstants.h"
#include "RangeConverter.h"
#include <map>


namespace InputMapping
{

	class InputContext
	{
		// Construction and destruction
	public:
		explicit InputContext(const std::string& contextfilename);
		~InputContext();

		// Mapping interface
	public:
		bool MapButtonToAction(FzlKey button, Action& out) const;
		bool MapButtonToState(FzlKey button, State& out) const;
		bool MapAxisToRange(RawInputAxis axis, Range& out) const;

		double GetSensitivity(Range range) const;

		const RangeConverter& GetConversions() const
		{
			return *Conversions;
		}

		// Internal tracking
	private:
		std::map<FzlKey, Action> ActionMap;
		std::map<FzlKey, State> StateMap;
		std::map<RawInputAxis, Range> RangeMap;

		std::map<Range, double> SensitivityMap;
		RangeConverter* Conversions;
	};

}

