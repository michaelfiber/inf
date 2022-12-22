#ifndef GLOBAL_H
#define GLOBAL_H

#include "raylib.h"

#define WORLD_WIDTH 256
#define WORLD_HEIGHT 256

#define MODE_RUNNING 0
#define MODE_DONE 1

#define BIOME_WATER 0
#define BIOME_SAND 1
#define BIOME_GRASS 2
#define BIOME_ROCKY 3
#define BIOME_SNOW 4
#define BIOME_PEAK 5

#define BIOME_COUNT 6

extern unsigned char *heights;

typedef struct
{
	int tile;
	bool passable;
	int item;
} GridCell;

typedef struct
{
	int x;
	int y;
} Location;

typedef struct
{
	Location grid;
	Vector2 pos;
	float speed;
	Texture2D tex;
} Player;

// random.c
void SeedRandom(int seed);
int GetRandom(int min, int max);

// drawimage.c
void DrawImageRec(Image *img, int x, int y, int width, int height, Color color);
void DrawImagePixel(Image *img, int x, int y, Color color);

// generator.c
Image GenerateWorldHeightMap(int width, int height, int worldSeed);
void GenerateWorldImage(Image heightMap, unsigned char *heights, Image *worldImage);
void FillLayers(Location worldLoc, GridCell *cells, unsigned char *heights);

// houses.c
Texture2D GenerateHouseTexture();

// biomes.c
Color GetBiomeColor(int biome);
int GetBiome(int elevation);
Image GenerateGroundTiles();
Image GenerateBiomeTiles();
int GetBiomeAtWorldLocation(Location worldLoc);
int GetElevationAtWorldLoc(Location worldLoc);

// human.c
Texture2D GenerateHuman();

// game.c
void InitGame();
int UpdateGame();
void DrawGame();
void UnloadGame();

#endif