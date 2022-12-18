#include "global.h"
#include "raylib.h"

int GetBiome(int elevation)
{
	int biome = BIOME_WATER;
	if (elevation < 75)
	{
		biome = BIOME_WATER;
	}
	else if (elevation < 90)
	{
		biome = BIOME_SAND;
	}
	else if (elevation < 170)
	{
		biome = BIOME_GRASS;
	}
	else if (elevation < 225)
	{
		biome = BIOME_ROCKY;
	}
	else if (elevation < 250)
	{
		biome = BIOME_SNOW;
	}
	else
	{
		biome = BIOME_PEAK;
	}
	return biome;
}

Color GetBiomeColor(int biome)
{
	Color color = BLUE;

	switch (biome)
	{
	case BIOME_WATER:
		color = BLUE;
		break;
	case BIOME_PEAK:
		color = WHITE;
		break;
	case BIOME_ROCKY:
		color = BROWN;
		break;
	case BIOME_GRASS:
		color = GREEN;
		break;
	case BIOME_SNOW:
		color = LIGHTGRAY;
		break;
	case BIOME_SAND:
		color = YELLOW;
		break;
	}

	return color;
}

int GetElevationAtWorldLoc(Location worldLoc)
{
	return heights[worldLoc.y * 256 + worldLoc.x];
}

int GetBiomeAtWorldLocation(Location worldLoc)
{
	return GetBiome(GetElevationAtWorldLoc(worldLoc));
}

void DrawRock(Image *img, int x, int y, Color tint)
{
	int width = GetRandom(24, 31);
	int height = GetRandom(22, 29);
	DrawImageRec(img, (32 - width) / 2, (32 - height) / 2, width, height, tint);
}

void DrawBiomeRock(Image *img)
{
	Color tint = GetBiomeColor(BIOME_ROCKY);

	for (int i = 0; i < 8; i++)
	{
		if (i < 4)
		{
			DrawRock(img, 32 * BIOME_ROCKY, 32 * i, tint);
		}
	}
}

void DrawBiomeGrass(Image *img)
{
	Color berries[4] = {0};

	for (int i = 0; i < 4; i++)
	{
		berries[i] = ColorFromHSV(GetRandom(0, 359), GetRandom(60, 99) / 100.0f, GetRandom(60, 99) / 100.0f);
	}

	Color greens[3] = {0};

	for (int i = 0; i < 3; i++)
	{
		greens[i] = ColorFromHSV(125, 1.0f, GetRandom(10, 50) / 100.0f);
	}

	int biome = BIOME_GRASS;
	for (int i = 0; i < 8; i++)
	{
		// first 4 tiles are not passable.
		if (i < 4)
		{
			Color trunk = ColorFromHSV(22, GetRandom(50, 70) / 100.0f, GetRandom(20, 40) / 100.0f);
			DrawImageRec(img, 32 / 2 - 4 + (biome * 32), (32 * i) + 12, 8, 20, trunk);

			for (int j = 1; j < 4; j++)
			{
				for (int t = 0; t < 30; t++)
				{
					DrawImageRec(img, GetRandom(0, 27) + (biome * 32), GetRandom(1, 8) * j + (32 * i), 4, 4, greens[j - 1]);
				}
			}
		}
		else
		{
			int count = GetRandom(5, 15);
			for (int j = 0; j < count; j++)
			{
				DrawImageRec(img, GetRandom(0, 29) + (biome * 32), GetRandom(0, 29) + (32 * i), 2, 2, berries[i - 4]);
			}
		}
	}
}

Image GenerateGroundTiles()
{
	int width = 32;
	int height = 32 * BIOME_COUNT * 8;

	Color *pixels = (Color *)MemAlloc(sizeof(Color) * width * height);
	int currentPixel = 0;

	for (int biome = 0; biome < BIOME_COUNT; biome++)
	{
		Vector3 hsv = {0};
		for (int currentTile = 0; currentTile < 8; currentTile++)
		{
			hsv = ColorToHSV(GetBiomeColor(biome));
			hsv.x += GetRandom(-10, 10);

			for (int i = 0; i < 32 * 32; i++)
			{
				float z = hsv.z * GetRandom(80, 100) / 100.0;
				pixels[currentPixel] = ColorFromHSV(hsv.x, hsv.y, z);
				currentPixel++;
			}
		}
	}

	Image img = {
		.data = pixels,
		.width = width,
		.height = height,
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
		.mipmaps = 1};

	return img;
}

Image GenerateBiomeTiles()
{
	Color transparent = ColorAlpha(WHITE, 0.0f);
	Color *data = (Color *)MemAlloc(sizeof(Color) * 32 * 32 * 8 * BIOME_COUNT);

	for (int i = 0; i < 32 * 32 * 8 * BIOME_COUNT; i++)
	{
		data[i] = transparent;
	}

	Image img = {
		.data = data,
		.width = 32 * BIOME_COUNT,
		.height = 32 * 8,
		.mipmaps = 1,
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

	for (int biome = 0; biome < BIOME_COUNT; biome++)
	{
		switch (biome)
		{
		case BIOME_GRASS:
			TraceLog(LOG_INFO, "DrawBiomeGrass");
			DrawBiomeGrass(&img);
			break;
		case BIOME_ROCKY:
			TraceLog(LOG_INFO, "DrawBiomeRocky");
			DrawBiomeRock(&img);
			break;
		}
	}

	return img;
}