# INF
Procedurally generated 2d RPG game.

## Background
This started as part of the raylib 9 year gamejam but I ran out of time to finish it. So after the jam ended I started refactoring and cleaning up what I had been making into this.

## Goals
Procedurally generate every aspect of the game on the fly. Very little world info is saved anywhere with most of it being regenerated on the fly as you move about.

The generated parts include:
- The world map - starts as opensimplex2 generated height map that gets color coded into "biomes"
- Ground tiles - Each tile is randomly generated when the game loads
- Humans - humans, for now, are all the same shape but skin, hair, shirt and pant color are randomly generated.
- Obstacles - trees, rocks, etc, - parts of the terrain
- Houses - a handful will be generated when the game loads
- Village layouts - A city center will be randomly picked and then a village randomly built around it.