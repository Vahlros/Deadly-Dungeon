#include "audio.h"

void InitAudio(std::vector<sf::SoundBuffer>& sounds)
{
	//Sound
	sounds.resize(GC::NUM_SOUNDS);
	//Sound: Player
	sounds[GC::SOUND_PLAYER_FOOTSTEPS].loadFromFile("audio/Player_Footstep.ogg");
	sounds[GC::SOUND_PLAYER_HIT].loadFromFile("audio/Player_Hit.ogg");
	sounds[GC::SOUND_PLAYER_DEATH].loadFromFile("audio/Player_Death.ogg");
	//Sound: Imp
	sounds[GC::SOUND_IMP_FOOTSTEPS].loadFromFile("audio/Imp_Footstep.ogg");
	sounds[GC::SOUND_IMP_HIT].loadFromFile("audio/Imp_Hit.ogg");
	sounds[GC::SOUND_IMP_NOISE0].loadFromFile("audio/Imp_Noise0.ogg");
	//Sound: Lesser Demon
	sounds[GC::SOUND_L_DEMON_FOOTSTEPS].loadFromFile("audio/LDemon_Footstep.ogg");
	sounds[GC::SOUND_L_DEMON_HIT].loadFromFile("audio/LDemon_Hit.ogg");
	sounds[GC::SOUND_L_DEMON_NOISE0].loadFromFile("audio/LDemon_Noise0.ogg");
	sounds[GC::SOUND_L_DEMON_NOISE1].loadFromFile("audio/LDemon_Noise1.ogg");
	//Sound: Aberrant
	sounds[GC::SOUND_ABERRANT_FOOTSTEPS].loadFromFile("audio/Aberrant_Footstep.ogg");
	sounds[GC::SOUND_ABERRANT_HIT].loadFromFile("audio/Aberrant_Hit.ogg");
	sounds[GC::SOUND_ABERRANT_NOISE0].loadFromFile("audio/Aberrant_Noise0.ogg");
	sounds[GC::SOUND_ABERRANT_NOISE1].loadFromFile("audio/Aberrant_Noise1.ogg");
	//Sound: Greater Demon
	sounds[GC::SOUND_G_DEMON_FOOTSTEPS].loadFromFile("audio/GDemon_Footstep.ogg");
	sounds[GC::SOUND_G_DEMON_HIT].loadFromFile("audio/GDemon_Hit.ogg");
	sounds[GC::SOUND_G_DEMON_NOISE0].loadFromFile("audio/GDemon_Noise0.ogg");
	sounds[GC::SOUND_G_DEMON_NOISE1].loadFromFile("audio/GDemon_Noise1.ogg");
	//Sound: Attack
	sounds[GC::SOUND_ATTACK0].loadFromFile("audio/Attack_Noise0.ogg");
	sounds[GC::SOUND_ATTACK1].loadFromFile("audio/Attack_Noise1.ogg");
	sounds[GC::SOUND_ATTACK2].loadFromFile("audio/Attack_Noise2.ogg");
	//Sound: Projectile
	sounds[GC::SOUND_FIRE_PROJECTILE].loadFromFile("audio/Projectile_Fire.ogg");
	//Sound: Shop
	sounds[GC::SOUND_SHOP_BUY].loadFromFile("audio/Shop_Buy.ogg");
	sounds[GC::SOUND_SHOP_FAIL].loadFromFile("audio/Shop_Fail.ogg");
	//Sound: Menu
	sounds[GC::SOUND_MENU_HOVER].loadFromFile("audio/Menu_Hover.ogg");
	sounds[GC::SOUND_MENU_SELECT].loadFromFile("audio/Menu_Select.ogg");
}

void PlaySound(const std::vector<sf::SoundBuffer>& soundList, sf::Sound& sound, const int& ID, const bool& loop)
{
	sound.stop();
	sound.setBuffer(soundList[ID]);
	sound.setLoop(loop);
	sound.play();
}

void PlayMusic(sf::Music& music, const int& ID, const bool& loop)
{
	music.stop();

	switch (ID)
	{
	case GC::MUSIC_MENUS:
		music.openFromFile("audio/Music_Menu.wav");;
		break;

	case GC::MUSIC_GAME:
		music.openFromFile("audio/Music_Game.wav");;
		break;

	case GC::MUSIC_WIN:
		music.openFromFile("audio/Music_Win.wav");;
		break;

	case GC::MUSIC_LOSE:
		music.openFromFile("audio/Music_Lose.wav");;
		break;
	}

	music.setVolume(GC::VOLUME_MUSIC);
	music.play();
	music.setLoop(loop);
}