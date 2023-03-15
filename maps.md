# Loading level layouts into code

## Option A: Make asset format + map editor
+ Highly customizable
- very long development time

.blvl format

Make editor where you type-in Texture static variable names in as your palette (+ the actual texture)

What defines a game 'tile'?
: A tile is a synonym for a textured entity that is aligned to a grid.
: Tiles can have custom behaviour by assigning tags to the entities tag bitflag: for example TAG\_DOOR, TAG\_COIN, TAG\_PLAYER (game-specific)
: Giving entities tags is way easier than using systems like ECS. I will be using an 'Entity mega-struct', termed by Ryan Fleury. I might get tempted to use unions to compress the data, but I shouldn't be bother with that during most of development.

Ways to read in texture assets:
- Game engine should have a --dump-assets command that dumps each placeable texture asset into a temporary binary file
- Editor parses C file for byte arrays (messy)

How to deal with advanced 'tiles' like doors, keys, coins?
- Game engine should have a --dump-tags command that returns all available tags that can be given to a tile.

The map editor is a seperate Python-program that doesn't depend on any Basalt source-code.

## Option B: Use map editor like Tiled
+ Easy graphical editor
- xml parsing (hard in C)
- too many unneeded features
- Tiled saves have differences between versions

## Option C: Type layout into code
+ highly based
+ easiest solution
- unpractical
- can't store advanced properties about objects

## Option D: In-game map editor
+ live-editing is powerful
- bloats engine source code
