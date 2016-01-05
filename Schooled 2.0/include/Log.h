#ifndef LOG_H
#define LOG_H

#include <string>
#include <vector>
#include <Windows.h>
#include <map>

using std::map;
using std::string;
using std::vector;

class Buffer;

class Log
{
public:
	// Create an empty Log object
	Log();

	// Add a string to the log (colour optional)
	void push_back(string);
	void push_back(string, WORD);

	// Output the log
	void display(HANDLE);
	void display(Buffer&, int row=0);

	// Clear the log
	void clear();

private:
	vector<string> log;		// The log containing the phrases
	vector<WORD> colours;	// Contains the colours
	vector<bool> newLines;	// If the line is new or not
	const int TEXT_START=15;
	bool newLine;
};

//generic global messages to be reused
static map<string, string> messages =
{
	{ "Q_RANDOM", "Random: What do you want ?" },
	{ "GET_KEY", "It's a key! You picked it up." },
	{ "DOOR_LOCKED", "The door is locked, you need a key." },
	{ "P_DOOR_LOCKED", "You need the Janitor's key to get through here." },
	{ "USE_KEY", "You used a key!" },
	{ "USE_MASTERKEY", "You used the Janitor's Keys!" },
	{ "RECIEVE_KEY", "You got a key!" },
	{ "Q_NEXT_ROOM", "Go to next room?" },
	{ "NEW_ROOM", "New room." },
	{ "ENEMY_DEATH", "It died..." },
	{ "UNATTACKABLE", "You can't kill that, you silly!" },
	{ "ATTACKABLE", "You hit that thing with " },
	{ "ENEMY_ATTACK", "You got hit with " },
	{ "PLAYER_DEATH", "You died..." },
	{"ENEMY_INTERACT", "What d'you think this is? The Talk? Fight me!"}
};

// Used for idle attacks
static map<string, string> messagesLog =
{
	{ "1", "You hit air! You did 0 damage!" },
	{ "2", "You swung at nothing." },
	{ "3", "You stared angrily at the ground." },
	{ "4", "You danced in place. You really have to pee." },
	{ "5", "You missed. You are filled with determination." },
	{ "6", "You sighed dramatically. This is hard." },
	{ "7", "You decided not to attack." },
	{ "8", "You missed, maybe open your eyes next time." },
	{ "9", "There was a squirrel, you get distracted easily." },
	{ "10", "You swung at nothing. The voices lied to you." }
};

// Used for the jerks who like to hit the NPCs
static map<string, string> messagesNPCHit =
{
	{ "1", "Ow!" },
	{ "2", "Hey! What was that for?" },
	{ "3", "Do you mind?" },
	{ "4", "Dude, watch where you're swinging!" },
	{ "5", "Cut it out!" },
	{ "6", "Let's not get violent, shall we?" },
	{ "7", "Pft. You hit like a child. Wait..." },
	{ "8", "You're starting to make me angry, kid." },
	{ "9", "Why me?" },
	{ "10", "Ouch! That really hurt!" },
	{ "11", "Please stop. Please?" },
	{ "12", "What do you gain from this?" },
	{ "13", "If you're done?" },
	{ "14", "You know, I liked you once." },
	{ "15", "I am very much hurt!" },
	{ "16", "That's gonna leave a mark." },
	{ "17", "Thank you! I've always wanted a bruise!" },
	{ "18", "Leave me alone you jerk!" },
	{ "19", "I came to school to get away from this!" },
	{ "20", "Can I help you?" },
	{ "21", "You're gonna piss off the wrong people like this." },
	{ "22", "If I wasn't so lazy, I'd punch you back." },
	{ "23", "My father will hear about this!" },
	{ "24", "Really? The school's relying on a bully?" },
	{ "25", "We're doomed." }
};

#endif