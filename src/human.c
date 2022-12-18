#include "raylib.h"
#include "global.h"

Image GenerateHumanImage()
{
	TraceLog(LOG_INFO, "Generating human...");

	Color transparent = ColorAlpha(WHITE, 0.0f);

	Color *data = (Color *)MemAlloc(sizeof(Color) * 16 * 32);

	Color hairColor = ColorFromHSV(GetRandom(0, 30), GetRandom(0, 9) / 10.0f, GetRandom(0, 9) / 10.0f);
	Color skinColor = ColorFromHSV(175 + GetRandom(0, 39) - 20, GetRandom(6, 9) / 10.0f, GetRandom(6, 9) / 10.0f);
	Color shirtColor = ColorFromHSV(GetRandom(0, 180) - 180, GetRandom(7, 9) / 10.0f, GetRandom(0, 9) / 10.0f);
	Color pantsColor = ColorFromHSV(GetRandom(0, 180) - 180, GetRandom(6, 8) / 10.0f, GetRandom(0, 7) / 10.0f);

	for (int i = 0; i < 16 * 32; i++)
	{
		data[i] = transparent;
	}

	Image img = {
		.data = data,
		.width = 16,
		.height = 32,
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
		.mipmaps = 1};

	DrawImageRec(&img, 4, 20, 8, 12, pantsColor);
	DrawImageRec(&img, 2, 8, 12, 12, shirtColor);
	DrawImageRec(&img, 4, 0, 8, 6, hairColor);
	DrawImageRec(&img, 6, 3, 4, 6, skinColor);

	return img;
}