#include "raylib.h"
#include "global.h"
#include "stdlib.h"

#define FNL_IMPL 1
#include "fastnoiselite.h"

Image GenerateWorldHeightMap(int width, int height, int worldSeed)
{
	unsigned char *data = (unsigned char *)MemAlloc(sizeof(unsigned char) * width * height);
	fnl_state noise = fnlCreateState();

	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
	noise.seed = worldSeed;

	int index = 0;
	int max = 0;

	float maxDistance = sqrtf(pow(width, 2) + pow(height, 2));

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int dx = abs(x - (width / 2));
			int dy = abs(y - (height / 2));
			float distance = sqrt((dx * dx) + (dy * dy));
			data[index++] = (fnlGetNoise2D(&noise, x, y) + 1) * 128 * pow((185 - distance) / maxDistance, 1.1);
			if (data[index - 1] > max)
				max = data[index - 1];
		}
	}

	TraceLog(LOG_INFO, TextFormat("max: %i", max));

	float rate = (max * 1.0f) / 255;
	for (int i = 0; i < 256 * 256; i++)
	{
		data[i] = data[i] / rate;
	}

	Image heightmap = {
		.data = data,
		.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE,
		.width = width,
		.height = height,
		.mipmaps = 1};

	return heightmap;
}

void GenerateWorldImage(Image heightMap, unsigned char *heights, Image *worldImage)
{
	Color *colorData = (Color *)(worldImage->data);

	unsigned char *pixels = heightMap.data;

	for (int i = 0; i < heightMap.width * heightMap.height; i++)
	{
		int biome = GetBiome(pixels[i]);
		heights[i] = pixels[i];
		colorData[i] = GetBiomeColor(biome);
	}
}

void FillLayers(Location worldLoc, GridCell *cells, unsigned char *h)
{
	// for now this is only the ground level.
	int elevation = h[worldLoc.y * WORLD_WIDTH + worldLoc.x];

	SeedRandom(worldLoc.y * WORLD_WIDTH + worldLoc.x);
	int biome = GetBiome(elevation);

	for (int i = 0; i < WORLD_WIDTH * WORLD_HEIGHT; i++)
	{
		cells[i].passable = biome != BIOME_WATER;
		cells[i].tile = GetRandom(0, 7);
		cells[i].item = GetRandom(0, 100);
		if (cells[i].item > 7)
		{
			cells[i].item = -1;
		}

		if (cells[i].item > -1 && cells[i].item < 4)
		{
			cells[i].passable = false;
		}
	}
}