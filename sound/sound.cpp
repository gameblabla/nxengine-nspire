
/* SOUND.C
  PXT/SS/Org sound interface
*/
#include <stdio.h>
#include <string.h>

#include "../nx.h"
#include "../settings.h"
#include "pxt.h"
#include "sound.h"
#include "sound.fdh"

#define MUSIC_OFF		0
#define MUSIC_ON		1
#define MUSIC_BOSS_ONLY	2
static int lastsong = 0;		// this holds the previous song, for <RMU
static int cursong = 0;

// there are more than this around 9b; those are drums and are loaded by the org module
#define NUM_SOUNDS		0x75
#define ORG_VOLUME		75

const char *org_names[] =
{
	NULL,
	"egg", "safety", "gameover", "gravity", "grasstown", "meltdown2", "eyesofflame",
	"gestation", "town", "fanfale1", "balrog", "cemetary", "plant", "pulse", "fanfale2",
	"fanfale3", "tyrant", "run", "jenka1", "labyrinth", "access", "oppression", "geothermal",
	"theme", "oside", "heroend", "scorching", "quiet", "lastcave", "balcony", "charge",
	"lastbattle", "credits", "zombie", "breakdown", "hell", "jenka2", "waterway", "seal",
	"toroko", "white", "azarashi", NULL
};

static const char bossmusic[] = { 4, 7, 10, 11, 15, 16, 17, 18, 21, 22, 31, 33, 35, 0 };

#ifdef NSPIRE
static const char *pxt_dir = "./pxt/";
static const char *org_dir = "./org/";
static const char *sndcache = "./sndcache.pcm.tns";
static const char *org_wavetable = "./wavetable.dat.tns";
#else
static const char *pxt_dir = "./pxt/";
static const char *org_dir = "./org/";
static const char *sndcache = "./sndcache.pcm";
static const char *org_wavetable = "./wavetable.dat";
#endif



bool sound_init(void)
{
	#ifdef SOUND
	if (SSInit()) return 1;
	if (pxt_init()) return 1;
	if (pxt_LoadSoundFX(pxt_dir, sndcache, NUM_SOUNDS)) return 1;
	
	if (org_init(org_wavetable, pxt_dir, ORG_VOLUME))
	{
		staterr("Music failed to initialize");
		return 1;
	}

	#endif
	return 0;
}

void sound_close(void)
{
	#ifdef SOUND
	pxt_freeSoundFX();
	SSClose();
	#endif
}

/*
void c------------------------------() {}
*/

void sound(int snd)
{
	#ifdef SOUND
	if (!settings->sound_enabled)
		return;
	
	pxt_Stop(snd);
	pxt_Play(-1, snd, 0);
	#endif
}

void sound_loop(int snd)
{
	#ifdef SOUND
	if (!settings->sound_enabled)
		return;
	
	pxt_Play(-1, snd, -1);
	#endif
}

void sound_stop(int snd)
{
	#ifdef SOUND
	pxt_Stop(snd);
	#endif
}

bool sound_is_playing(int snd)
{
	#ifdef SOUND
	return pxt_IsPlaying(snd);
	#endif
}


void StartStreamSound(int freq)
{
	#ifdef SOUND
	// pxt_ChangePitch(SND_STREAM1, some_formula);
	// pxt_ChangePitch(SND_STREAM2, some_other_formula);
	sound_loop(SND_STREAM1);
	sound_loop(SND_STREAM2);
	#endif
}

void StartPropSound(void)
{
	#ifdef SOUND
	sound_loop(SND_PROPELLOR);
	#endif
}

void StopLoopSounds(void)
{
	#ifdef SOUND
	sound_stop(SND_STREAM1);
	sound_stop(SND_STREAM2);
	sound_stop(SND_PROPELLOR);
	#endif
}

/*
void c------------------------------() {}
*/

void music(int songno)
{
	#ifdef SOUND
	if (songno == cursong)
		return;
	
	lastsong = cursong;
	cursong = songno;
	
	stat(" >> music(%d)", songno);
	
	if (songno != 0 && !should_music_play(songno, settings->music_enabled))
	{
		stat("Not playing track %d because music_enabled is %d", songno, settings->music_enabled);
		org_stop();
		return;
	}
	
	start_track(songno);
	#endif
}


bool should_music_play(int songno, int musicmode)
{
	#ifdef SOUND
	if (game.mode == GM_TITLE || game.mode == GM_CREDITS)
		return true;
	
	switch(musicmode)
	{
		case MUSIC_OFF: return false;
		case MUSIC_ON:  return true;
		case MUSIC_BOSS_ONLY:
			return music_is_boss(songno);
	}
	
	#endif
	
	return false;
}

bool music_is_boss(int songno)
{
	#ifdef SOUND
	if (strchr(bossmusic, songno))
		return true;
	else
	#endif
		return false;
}

void music_set_enabled(int newstate)
{
	#ifdef SOUND
	if (newstate != settings->music_enabled)
	{
		stat("music_set_enabled(%d)", newstate);
		
		settings->music_enabled = newstate;
		bool play = should_music_play(cursong, newstate);
		
		if (play != org_is_playing())
		{
			if (play)
				start_track(cursong);
			else
				org_stop();
		}
	}
	#endif
}

static void start_track(int songno)
{
	#ifdef SOUND
	char fname[MAXPATHLEN];

	if (songno == 0)
	{
		org_stop();
		return;
	}
	
	strcpy(fname, org_dir);
	strcat(fname, org_names[songno]);
	strcat(fname, ".org");
	
	if (!org_load(fname))
	{
		org_start(0);
	}
	#endif
}

int music_cursong()		{ return cursong; }
int music_lastsong() 	{ return lastsong; }


