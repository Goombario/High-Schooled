//
// Input Mapping Demo
// By Mike Lewis, June 2011
// http://scribblings-by-apoch.googlecode.com/
//
// Wrapper class for mapping inputs
//

#ifndef INPUT_MAPPER_H
#define INPUT_MAPPER_H


// Dependencies
#include "Fizzle/DataTypes/FizzleDataTypes.h"
#include "RawInputConstants.h"
#include "InputConstants.h"

#include <map>
#include <set>
#include <list>
#include <string>


namespace InputMapping
{

	// Forward declarations
	class InputContext;


	// Helper structure
	struct MappedInput
	{
		std::set<Action> Actions;
		std::set<State> States;
		std::map<Range, double> Ranges;

		// Consumption helpers
		void EatAction(Action action)		{ Actions.erase(action); }
		void EatState(State state)			{ States.erase(state); }
		void EatRange(Range range)
		{
			std::map<Range, double>::iterator iter = Ranges.find(range);
			if (iter != Ranges.end())
				Ranges.erase(iter);
		}
	};


	// Handy type shortcuts
	typedef void(*InputCallback)(MappedInput& inputs);


	class InputMapper
	{
		// Construction and destruction
	public:
		InputMapper();
		~InputMapper();

		// Raw input interface
	public:
		void Clear();
		void SetRawButtonState(FzlKey button, bool pressed, bool previouslypressed);
		void SetRawAxisValue(RawInputAxis axis, double value);

		// Input dispatching interface
	public:
		void Dispatch() const;

		// Input callback registration interface
	public:
		void AddCallback(InputCallback callback, int priority);

		// Context management interface
	public:
		void PushContext(const std::string& name);
		void PopContext();

		// Internal helpers
	private:
		bool MapButtonToAction(FzlKey button, Action& action) const;
		bool MapButtonToState(FzlKey button, State& state) const;
		void MapAndEatButton(FzlKey button);

		// Internal tracking
	private:
		std::map<std::string, InputContext*> InputContexts;
		std::list<InputContext*> ActiveContexts;

		std::multimap<int, InputCallback> CallbackTable;

		MappedInput CurrentMappedInput;
	};

}

#endif