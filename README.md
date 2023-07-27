> NOTE: Basalt has gone full circle and will be rewritten back into glorious C!

![basalt](misc/logo_small.png)

Software rendered game engine based on SDL3 for C++ cavemen

Solidified version of [Magma Game Engine](https://github.com/bramtechs/RaylibMagmaEngine) and [dream-emu](https://github.com/bramtechs/dream-emu).

Inspired by [Handmade Hero](https://handmadehero.org/).

## Bindings

- C (planned)
- Go (planned)
- Javacript (planned, maybe)

## Compilation

### Windows 8/10/11

- Visual Studio 22 or VS Build Tools 2019

1. Clone this repo **recursively** to download all required files.

```bash
git clone https://github.com/Doomhowl-Interactive/basalt.git --recursive
# or
gh repo clone Doomhowl-Interactive/basalt -- --recursive
```

2. Build with Visual Studio _(open basalt.sln)_ or compile via MSBuild.

```bash
msbuild -m -property:Configuration=Release
```

> SDL3 (master branch of SDL) is statically compiled with basalt,
> so the first compilation may take a while...

## Screenshots

![Preview](screenshots/windows7_2.PNG)

```

```
