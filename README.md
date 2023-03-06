# basalt

Slowly solidifying [Magma Game Engine](https://github.com/bramtechs/RaylibMagmaEngine) and [dream-emu](https://github.com/bramtechs/dream-emu) while focusing on [Suckless](suckless.org)/[Handmade Hero](https://handmadehero.org/)-minimalism and performance.

## New goals
- C over C++
- Engine agnostic
- Low level SDL over Raylib
- Embed game assets into executable (instead of seperate file)
- Highly backwards-compatible (Windows XP support and higher)

- No CMake (use plain old .bat script)
- No Entity Component Systems (too complicated)
- No graphics cards! -> write graphics straight into memory (as seen in [olive.c library](https://github.com/tsoding/olive.c))

## Long-term goals
- WASM-browser support
- Actually make a game I guess

## Dumb goals
- Arduino Uno support (communicate graphics over serial connection)

