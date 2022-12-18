#include "raylib.h"
#include "global.h"

void UpdateAndDraw();
void Update();
void Draw();

RenderTexture2D rt = {0};

int main(void)
{
	InitWindow(800, 800, "Inf");
	
	SetTargetFPS(60);

	rt = LoadRenderTexture(WORLD_WIDTH, WORLD_HEIGHT);

	InitGame();

	while (!WindowShouldClose())
	{
		UpdateAndDraw();
	}

	UnloadGame();

	UnloadRenderTexture(rt);

	CloseWindow();
}

void UpdateAndDraw()
{
	Update();
	Draw();
}

void Update()
{
	if (UpdateGame() == MODE_DONE)
	{
		// Move on to next stage here...
	}
}

void Draw()
{
	BeginDrawing();
	{
		BeginTextureMode(rt);
		{
			DrawGame();
		}
		EndTextureMode();
		DrawTexturePro(rt.texture, (Rectangle){0, 0, rt.texture.width, -rt.texture.height}, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, (Vector2){0, 0}, 0.0f, WHITE);
		DrawFPS(10, 10);
	}
	EndDrawing();
}
