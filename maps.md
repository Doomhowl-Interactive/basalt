# Loading level layouts into code
## Making a map editor

Make editor where you use Texture names as your palette (+ the actual texture)

### Definition of in-game tile

A tile is a synonym for a textured entity that is aligned to a grid.
Tiles can have custom behaviour by assigning tags to the entities tag bitflag: for example TAG\_DOOR, TAG\_COIN, TAG\_PLAYER (game-specific things)
Giving entities tags is way easier than using systems like ECS. I will be using the 'Entity mega-struct' approach, termed by Ryan Fleury.

I might get tempted to use unions to compress the data, but I shouldn't bother with that during most of development. Just buy more RAM lol.

### Reading in the texture assets:
- ~~Map editor should parse assets.dat.c files~~
- **Map editor gets linked with the game!~~

How to deal with advanced 'tiles' like doors, keys, coins?
- Map editor collects all tags that can be given to a tile.

The map editor will also be written in C, as I hate myself.
