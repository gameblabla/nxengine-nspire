
// manages the tileset
#ifdef NSPIRE
#include "nspire_back.h"
#endif

#include "graphics.h"
#include "tileset.h"
#include "tileset.fdh"
using namespace Graphics;

extern const char *tileset_names[];		// from stagedata.cpp
extern const char *stage_dir;			// from main

static NXSurface *tileset;
static int current_tileset = -1;

bool Tileset::Init()
{
	tileset = NULL;
	current_tileset = -1;
	return 0;
}

void Tileset::Close()
{
	delete tileset;
}

/*
void c------------------------------() {}
*/

// load the given tileset into memory, replacing any other tileset.
bool Tileset::Load(int new_tileset)
{
	char fname[MAXPATHLEN];
		unsigned short *ff;

	if (new_tileset != current_tileset)
	{
		if (tileset)
		{
			delete tileset;
			current_tileset = -1;
		}
		
#ifdef NSPIRE

		if (new_tileset == 0) 	ff = image_Prt0;
		else if (new_tileset == 1) ff = image_PrtPens;
		else if (new_tileset == 2) ff = image_PrtEggs;
		else if (new_tileset == 3) ff = image_PrtEggX;
		else if (new_tileset == 4) ff = image_PrtEggIn;
		else if (new_tileset == 5) ff = image_PrtStore;
		else if (new_tileset == 6) ff = image_PrtWeed;
		else if (new_tileset == 7) ff = image_PrtBarr;
		else if (new_tileset == 8) ff = image_PrtMaze;
		else if (new_tileset == 9) ff = image_PrtSand;
		else if (new_tileset == 10) ff = image_PrtMimi;
		else if (new_tileset == 11) ff = image_PrtCave;
		else if (new_tileset == 12) ff = image_PrtRiver;
		else if (new_tileset == 13) ff = image_PrtGard;
		else if (new_tileset == 14) ff = image_PrtAlmond;
		else if (new_tileset == 15) ff = image_PrtOside;
		else if (new_tileset == 16) ff = image_PrtCent;
		else if (new_tileset == 17) ff = image_PrtJail;
		else if (new_tileset == 18) ff = image_PrtWhite;
		else if (new_tileset == 19) ff = image_PrtFall;
		else if (new_tileset == 20) ff = image_PrtHell;
		else if (new_tileset == 21) ff = image_PrtLabo;
		else ff = image_Prt0;
#else
		sprintf(fname, "%s/Prt%s.pbm", stage_dir, tileset_names[new_tileset]);
#endif
		
		// always use SDL_DisplayFormat on tilesets; they need to come out of 8-bit
		// so that we can replace the destroyable star tiles without them palletizing.
#ifdef NSPIRE
		if (new_tileset == 0) tileset = NXSurface::FromFile2(ff, false, true);
		else tileset = NXSurface::FromFile2(ff, true, true);
#else
		tileset = NXSurface::FromFile(fname, true, true);
#endif
		if (!tileset)
		{
			return 1;
		}
		
		current_tileset = new_tileset;
	}
	
	return 0;
}

// draw the given tile from the current tileset to the screen
void Tileset::draw_tile(int x, int y, int t)
{
	// 16 tiles per row on all tilesheet
	int srcx = (t % 16) * TILE_W;
	int srcy = (t / 16) * TILE_H;
	
	DrawSurface(tileset, x, y, srcx, srcy, TILE_W, TILE_H);
}

void Tileset::Reload()
{
	if (current_tileset != -1)
	{
		int tileset = current_tileset;
		current_tileset = -1;
		Load(tileset);
	}
}

/*
void c------------------------------() {}
*/

NXSurface *Tileset::GetSurface()
{
	return tileset;
}

