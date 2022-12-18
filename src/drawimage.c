#include "raylib.h"
#include "global.h"

void DrawImageRec(Image *img, int x, int y, int width, int height, Color color)
{
	Color *data = img->data;

	for (int xf = x; xf < x + width; xf++)
	{
		for (int yf = y; yf < y + height; yf++)
		{
			data[yf * img->width + xf] = color;
		}
	}
}

void DrawImagePixel(Image *img, int x, int y, Color color)
{
	Color *data = img->data;
	data[y * img->width + x] = color;
}