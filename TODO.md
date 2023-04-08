# Things to do
Automatically generated. Do not edit by hand!

## src/bullet_entities.c (3 items)
- [ ] TODO: Use regions for sprites instead of top-left pos
- [ ] TODO: Put in entity struct
- [ ] TODO: wrap in builder

## src/basalt.h (1 items)
- [ ] FIXME: This won't work on AZERTY keyboards!

## src/external/stb_image.h (5 items)
- [ ] @TODO: move stbi__convert_format to here
- [ ] @TODO: move stbi__convert_format16 to here
- [ ] @TODO: special case RGB-to-Y (and RGBA-to-YA) for 8-bit-to-16-bit case to keep more precision
- [ ] STBI_NOTUSED(tga_x_origin); // @TODO
- [ ] STBI_NOTUSED(tga_y_origin); // @TODO

## src/basalt_utils.c (2 items)
- [ ] TODO: This could be better
- [ ] FIXME: Untested

## src/bullet_common.h (1 items)
- [ ] TODO: Use regions for sprites

## src/win32_basalt.c (3 items)
- [ ] TODO: Implement
- [ ] TODO(casey): Logging
- [ ] TODO(casey): Logging

## src/basalt_plat.h (1 items)
- [ ] TODO: test on Windows

## src/xorg_basalt.c (10 items)
- [ ] TODO: this should be done dynamically
- [ ] FIXME: SLOW 25% performance hit
- [ ] FIXME: SLOW 50% performance hit
- [ ] HACK: ignore special characters (for now)
- [ ] DEBUG("%s %c", pressed ? "Pressed":"Released", key);
- [ ] HACK: make the window the size of the entire monitor so the DC is big enough
- [ ] HACK: resize window to game size
- [ ] FIXME: Prevent keys from getting stuck when pressing mutiple.
- [ ] HACK: might not work while pressing multiple mouse buttons
- [ ] HACK: Prevent delta spikes from breaking the game

## src/bullet_patterns.c (1 items)
- [ ] TODO: Add end condition instead of passing duration integer everywhere

## src/basalt_assets.c (1 items)
- [ ] HACK: Copy the texture into the correct color order

## src/basalt_extra_graphics.c (2 items)
- [ ] FIXME: SLOW AND TERRIBLE
- [ ] TODO: Refactor

## src/wasm_stdlib.h (1 items)
- [ ] TODO: free is not yet implemented, now pray you don't run out of RAM.

## src/basalt_graphics.c (8 items)
- [ ] TODO: Clean up required
- [ ] TODO: move boundary checks out side of the loops in olivec_draw_line
- [ ] TODO: move boundary checks out side of the loops in olivec_draw_line
- [ ] TODO: Add collective assert method, also write assert that prints values
- [ ] TODO: optimize
- [ ] TODO: Bounds checking!
- [ ] HACK: Accidental motion blur while messing around
- [ ] TODO: cleanup

## src/temple_game.c (1 items)
- [ ] TODO: use raylib's color constants

## basalt.js (1 items)
- [ ] const delta = 1.0 / 60; // TODO: implement prober deltatime

## temple_src/temple_game.c (2 items)
- [ ] TODO: not implemented
- [ ] TODO: Fix physics bugs at low FPS

