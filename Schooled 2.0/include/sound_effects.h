#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H
#include <windows.h>

// Contains a sound file and methods for manipulating it
class Sound
{
private:
	LPCTSTR fileLocation;
	bool loop;
	bool async;
public:
	// Create an empty Sound object
	Sound();

	// Create a Sound object containing a .wav file
	Sound(LPCTSTR, bool, bool=true);

	// Play the sound
	void play();

	// Stop the sound
	void stop();
};

// Pointer to Sound object
typedef Sound* SndPtr;

// Sound effects and music
namespace snd
{
	const SndPtr title = new Sound(L"schooled64.wav", true);
	const SndPtr startGame = new Sound(L"pewpew.wav", false, false);
	const SndPtr dungeonMusic = new Sound(L"background_1.wav", true);
	const SndPtr pause = new Sound(L"pause.wav", false);
	const SndPtr unpause = new Sound(L"unpause.wav", false);
	const SndPtr key = new Sound(L"key.wav", false);
	const SndPtr menuSelect = new Sound(L"select.wav", false);
	const SndPtr menuDeselect = new Sound(L"back.wav", false);
	const SndPtr lockedDoor = new Sound(L"locked_jiggle.wav", false);
	const SndPtr menuHighlight = new Sound(L"boop.wav", false);
	const SndPtr nextRoom = new Sound(L"door_sounds.wav", false);
	const SndPtr attack1 = new Sound(L"hit_1.wav", false);
	const SndPtr attack2 = new Sound(L"hit_2.wav", false);
	const SndPtr playerDeath = new Sound(L"death.wav", false);
}
#endif