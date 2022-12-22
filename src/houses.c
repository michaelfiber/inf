#include "raylib.h"
#include "global.h"

#define TILE_HOUSE_ROOF 0
#define TILE_HOUSE_ROOFTOP 1
#define TILE_HOUSE_WALL 2
#define TILE_HOUSE_WINDOW 3
#define TILE_HOUSE_DOOR 4

#define TILE_COUNT 5

Image GenerateHouseTileImage()
{
	TraceLog(LOG_INFO, "Generating house tiles...");

	Color *data = (Color *)MemAlloc(sizeof(Color) * 32 * 32 * TILE_COUNT);
	Color transparent = ColorAlpha(WHITE, 0.0f);
	for (int i = 0; i < 32 * 32 * TILE_COUNT; i++)
	{
		data[i] = transparent;
	}

	for (int i = 0; i < TILE_COUNT; i++)
	{
		switch (i)
		{
		case TILE_HOUSE_ROOF:
			for (int x = 0; x < 32; x++)
			{
				for (int y = 32 * i; y < (32 * i) + 32; y++)
				{
					if (y > 32 * i && y % 8 == 0)
					{
						data[y * 32 + x] = BLACK;
					}
					else if (x % 8 == 0 || (y > 0 && (y + 1) % 8 == 0))
					{
						data[y * 32 + x] = WHITE;
					}
					else
					{
						data[y * 32 + x] = ColorFromHSV(0.0f, 0.0f, GetRandom(30, 49) / 100.0f);
					}
				}
			}
			break;
		case TILE_HOUSE_ROOFTOP:
			for (int x = 0; x < 32; x++)
			{
				for (int y = 32 * i; y < (32 * i) + 32; y++)
				{
					if (y > 32 * i && y % 8 == 0)
					{
						data[y * 32 + x] = BLACK;
					}
					else if (x % 8 == 0 || y == 32 * i || y == 32 * i + 2 || (y > 0 && (y + 1) % 8 == 0))
					{
						data[y * 32 + x] = WHITE;
					}
					else
					{
						data[y * 32 + x] = ColorFromHSV(0.0f, 0.0f, GetRandom(30, 49) / 100.0f);
					}
				}
			}
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
