#include "raylib.h"
#include "global.h"
#include "math.h"

Camera2D camera = {0};
Texture2D worldTexture = {0};
Texture2D heightMapTexture = {0};
Texture2D groundTileTexture = {0};
Texture2D biomeFeatureTexture = {0};

Player player = {0};

GridCell *cells;

#define VIEW_LOCAL 0
#define VIEW_WORLD 1

unsigned char *heights;

int view = VIEW_LOCAL;
int drawCount = 0;

void ResetCamera()
{
	camera.offset.x = WORLD_WIDTH / 2;
	camera.offset.y = WORLD_HEIGHT / 2;
	camera.rotation = 0.0f;
	camera.target.x = WORLD_WIDTH / 2;
	camera.target.y = WORLD_HEIGHT / 2;
	camera.zoom = 1.0f;
}

void GenerateWorld()
{
	// Height map generated using fastnoiselite with the same worldseed.
	Image worldHeightImage = GenerateWorldHeightMap(WORLD_WIDTH, WORLD_HEIGHT, 12345);

	heightMapTexture = LoadTextureFromImage(worldHeightImage);

	Image worldImg = {
		.data = (Color *)MemAlloc(sizeof(Color) * WORLD_WIDTH * WORLD_HEIGHT),
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
		.width = WORLD_WIDTH,
		.height = WORLD_HEIGHT,
		.mipmaps = 1};

	GenerateWorldImage(worldHeightImage, heights, &worldImg);
	worldTexture = LoadTextureFromImage(worldImg);

	UnloadImage(worldImg);
	UnloadImage(worldHeightImage);
}

void LoadLocal(int worldSeed)
{
}

void InitGame()
{
	heights = MemAlloc(sizeof(unsigned char) * WORLD_WIDTH * WORLD_HEIGHT);

	player.grid.x = 85;
	player.grid.y = WORLD_HEIGHT / 2;

	// each square of the grid is 256 x 256 tiles, each tile is 32 x 32.
	// start player in the middle for now.
	player.pos.x = 10;
	player.pos.y = 10;

	player.speed = 100.0f;

	ResetCamera();

	// initialize the randomizer with the world seed.
	SeedRandom(12345);

	GenerateWorld();

	Image groundTileImage = GenerateGroundTiles();
	groundTileTexture = LoadTextureFromImage(groundTileImage);
	UnloadImage(groundTileImage);

	Image biomeFeatureImage = GenerateBiomeTiles();
	biomeFeatureTexture = LoadTextureFromImage(biomeFeatureImage);
	UnloadImage(biomeFeatureImage);

	cells = (GridCell *)MemAlloc(sizeof(GridCell) * WORLD_WIDTH * WORLD_HEIGHT);
	FillLayers(player.grid, cells, heights);

	player.tex = GenerateHuman();
}

void UpdateLocal()
{
	Vector2 vel = {0};
	if (IsKeyDown(KEY_UP))
	{
		vel.y--;
	}

	if (IsKeyDown(KEY_DOWN))
	{
		vel.y++;
	}

	if (IsKeyDown(KEY_LEFT))
	{
		vel.x--;
	}

	if (IsKeyDown(KEY_RIGHT))
	{
		vel.x++;
	}

	if (fabs(vel.x) > 0 && fabs(vel.y) > 0)
	{
		vel.x = 0.7 * vel.x;
		vel.y = 0.7 * vel.y;
	}

	// int localX = player.pos.x / 32;
	// int localY = (player.pos.y + 16) / 32;

	float xInc = vel.x * GetFrameTime() * player.speed;
	float yInc = vel.y * GetFrameTime() * player.speed;

	// set the rectangle to where the player will be after position increments.
	Rectangle playerRec = {player.pos.x + xInc, player.pos.y + yInc, 16, 16};

	player.pos.x = playerRec.x;
	player.pos.y = playerRec.y;
}

void UpdateWorld()
{
}

int UpdateGame()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		if (view == VIEW_LOCAL)
		{
			view = VIEW_WORLD;
		}
		else
		{
			view = VIEW_LOCAL;
		}
	}

	switch (view)
	{
	case VIEW_LOCAL:
		camera.target.x = player.pos.x;
		camera.target.y = player.pos.y;
		UpdateLocal();
		break;
	case VIEW_WORLD:
		camera.target.x = player.grid.x;
		camera.target.y = player.grid.y;
		UpdateWorld();
		break;
	}
	return MODE_RUNNING;
}

void DrawLocal()
{
	int biome = GetBiomeAtWorldLocation(player.grid);
	drawCount = 0;

	for (int y = (camera.target.y - (WORLD_HEIGHT / 2)) / 32; y < WORLD_HEIGHT && y < (camera.target.y - (WORLD_HEIGHT / 2)) / 32 + 8; y++)
	{
		for (int x = (camera.target.x - (WORLD_WIDTH / 2)) / 32; x < WORLD_WIDTH && x < (camera.target.x - (WORLD_WIDTH / 2)) / 32 + 8; x++)
		{
			if (y < 0 || x < 0)
				continue;
			DrawTexturePro(groundTileTexture, (Rectangle){0, (biome * 8 * 32) + (cells[y * WORLD_WIDTH + x].tile * 32), 32, 32}, (Rectangle){x * 32, y * 32, 32, 32}, (Vector2){0, 0}, 0.0f, WHITE);
			drawCount++;
		}
	}

	//DrawRectangle(player.pos.x - 10, player.pos.y - 10, 20, 20, RED);
	DrawTexture(player.tex, player.pos.x, player.pos.y, WHITE);
}

void DrawWorld()
{
	DrawTexture(worldTexture, 0, 0, WHITE);
	DrawRectangleLines(player.grid.x - 1, player.grid.y - 1, 3, 3, RED);
}

void DrawGame()
{
	BeginMode2D(camera);
	{
		ClearBackground(GetBiomeColor(GetBiomeAtWorldLocation(player.grid)));
		switch (view)
		{
		case VIEW_LOCAL:
			DrawLocal();
			break;
		case VIEW_WORLD:
			DrawWorld();
			break;
		}
	}
	EndMode2D();

	DrawText(TextFormat("Draw:%i Biome:%i Grid:%ix%i", drawCount, GetBiomeAtWorldLocation(player.grid), player.grid.x, player.grid.y), 10, 10, GetFontDefault().baseSize, BLACK);
}

void UnloadGame()
{
	UnloadTexture(worldTexture);
	UnloadTexture(heightMapTexture);
	UnloadTexture(groundTileTexture);
	UnloadTexture(biomeFeatureTexture);
	UnloadTexture(groundTileTexture);
	UnloadTexture(biomeFeatureTexture);
	UnloadTexture(player.tex);

	MemFree(cells);
	MemFree(heights);
}