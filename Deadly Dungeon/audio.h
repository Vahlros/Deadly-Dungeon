#pragma once
#include "SFML/Audio.hpp"

//Initializes the sound and music vectors
void InitAudio(std::vector<sf::SoundBuffer>& sounds);

//Plays a sound, either once or on a loop
void PlaySound(const std::vector<sf::SoundBuffer>& soundList, sf::Sound& sound, const int& ID, const bool& loop);

//Streams music on a loop, but once for win/loss, only one music will be playing at a time
void PlayMusic(sf::Music& music, const int& ID, const bool& loop);

namespace GC //Game constants
{
	//Enums
	enum SOUND_BUFFERS { SOUND_PLAYER_FOOTSTEPS, SOUND_PLAYER_HIT, SOUND_PLAYER_DEATH, SOUND_IMP_FOOTSTEPS, SOUND_IMP_HIT, SOUND_IMP_NOISE0,
		SOUND_L_DEMON_FOOTSTEPS, SOUND_L_DEMON_HIT, SOUND_L_DEMON_NOISE0, SOUND_L_DEMON_NOISE1, SOUND_ABERRANT_FOOTSTEPS, SOUND_ABERRANT_HIT,
		SOUND_ABERRANT_NOISE0, SOUND_ABERRANT_NOISE1, SOUND_G_DEMON_FOOTSTEPS, SOUND_G_DEMON_HIT, SOUND_G_DEMON_NOISE0, SOUND_G_DEMON_NOISE1,
		SOUND_ATTACK0, SOUND_ATTACK1, SOUND_ATTACK2, SOUND_FIRE_PROJECTILE, SOUND_SHOP_BUY, SOUND_SHOP_FAIL, SOUND_MENU_HOVER, SOUND_MENU_SELECT };
	enum MUSIC_FILES { MUSIC_MENUS, MUSIC_GAME, MUSIC_WIN, MUSIC_LOSE };

	//Audio
	const int NUM_SOUNDS = 26;
	const int NUM_MUSIC = 4;
	const float VOLUME_MUSIC = 10.f; //Maximum volume of music
	const float VOLUME_FOOTSTEPS = 0.6f; //Maximum volume of footsteps (60.f)
	const float VOLUME_NOISES = 0.6f; //Maximum volume of noises (60.f)
	//Audio: Enemy
	const float MAX_AUDIO_RANGE = 160.f; //Volume will be mute when more than this distance from the player
	const float MIN_AUDIO_RANGE = 32.f; //Volume will be maximum when less than this distance from the player
	const int RANDOM_NOISE_CHANCE = 30; //Percentage chance that an enemy will play a noise
	const float RANDOM_NOISE_TIMER = 1.f; //How frequent the enemy will attempt to play a noise
}