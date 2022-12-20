#include "raylib.h"

#define TILE_HOUSE_ROOF 0
#define TILE_HOUSE_ROOFTOP 1
#define TILE_HOUSE_WALL 2
#define TILE_HOUSE_WINDOW 3
#define TILE_HOUSE_DOOR 4

#define TILE_COUNT 5

Image GenerateHouseTileImage()
{
	Color *data = (Color *)MemAlloc(sizeof(Color) * 32 * TILE_COUNT);
	Color transparent = ColorAlpha(WHITE, 0.0f);
	for (int i = 0; i < 32 * TILE_COUNT; i++)
	{
		data[i] = transparent;
	}

	for (int i = 0; i < TILE_COUNT; i++)
	{
		switch (i)
		{
		case TILE_HOUSE_ROOF:
			break;
		}
	}

	return (Image){
		.data = data,
		.width = 32,
		.height = 32 * TILE_COUNT,
		.mipmaps = 1,
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
}

Texture2D GenerateHouseTexture()
{
	Image img = GenerateHouseTileImage();
	Texture2D tex = LoadTextureFromImage(img);
	UnloadImage(img);
	return tex;
}