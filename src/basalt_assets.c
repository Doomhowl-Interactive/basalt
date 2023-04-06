#ifndef BASALT_NO_ASSETS

#include "basalt.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#include "external/stb_image.h"

class(AssetEntry) {
    const char* filePath;
    const char* fileName;
    usize lastPollFrame;
};

static usize AssetEntryCount = 0;
static AssetEntry* AssetEntries = NULL;

static const char* AssetFolders[] = {
    "assets",
    "../assets",
    "~/dev/basalt/assets",
    "C:\\dev\\basalt\\assets",
    NULL,
};

func bool GetAssetEntry(const char* name, AssetEntry* result)
{
    assert(name);
    assert(AssetEntries);
    for (usize i = 0; i < AssetEntryCount; i++)
    {
        const char* fileName = AssetEntries[i].fileName;
        if (strcmp(fileName, name) == 0)
        {
            *result = AssetEntries[i];
            return true;
        }
    }
    return false;
}

pubfunc void HotReloadTexture(Texture texture)
{
    if (!Config.hasHotloading || texture.name == NULL)
        return;

    AssetEntry entry;
    if (GetAssetEntry(texture.name, &entry))
    {
        if (entry.lastPollFrame - GetFrameIndex() < 300)
            return; // polled to recently
        entry.lastPollFrame = GetFrameIndex();

        DEBUG("Polling %s", texture.name);
    }
    else if (GetFrameIndex() % 300 == 0)
    {
        WARN("Can't find original asset of %s, hotloading not supported!", texture.name);
    }
}

pubfunc void InitHotReloading()
{
    if (!Config.hasHotloading)
        return;

    const char* folder = GetFirstExistingFolder(AssetFolders);
    INFO("Found asset folder at %s", folder);
    FilePathList list = GetFolderFiles(folder, ".png");
    INFO("Found %d textures to be hot-reloaded",list.count);

    // Map to AssetEntries
    AssetEntryCount = list.count;
    AssetEntries = calloc(sizeof(AssetEntry), AssetEntryCount);

    for (usize i = 0; i < AssetEntryCount; i++)
    {
        AssetEntry* e = &AssetEntries[i];
        e->filePath = list.strings[i];

        char* fileStem = (char*) GetFileStem(list.strings[i]);
        ToUppercase(fileStem);
        e->fileName = fileStem;

        e->lastPollFrame = GetFrameIndex();

        DEBUG("--> %s %s %u",e->filePath, e->fileName, e->lastPollFrame);
    }

    UnloadFilePathList(list);
}

pubfunc Texture LoadTextureEx(const char* name, uchar* pixels) {

    int width, height;
    int channels = 0;

    uint* pixels32Bit = (uint*) pixels;

    int size = pixels32Bit[0];
    uchar* data = (uchar*) stbi_load_from_memory((stbi_uc*) &pixels32Bit[1], size,
                                                 &width, &height,
                                                 &channels, 4);

    DEBUG("Loaded texture %s of size %u with %d channels", name, size, channels);

    Texture texture = InitTexture(width, height);
    texture.name = name;

    if (data == NULL) {
        ERR("Failed to load texture %s from memory! ( likely data corruption :( )", name);
        return texture;
    }

    if (channels != 4 && channels != 3) {
        ERR("Unexpected amount of channels in image (%d)!", channels);
        return texture;
    }

    // HACK: Copy the texture into the correct color order
    // (by trial and error lol)
    uchar* comps = (uchar*) texture.pixels;
    for (int i = 0; i < width * height; i++) {
        ((uchar*)texture.pixels)[i*4+0] = data[i*4+3];
        ((uchar*)texture.pixels)[i*4+1] = data[i*4+2];
        ((uchar*)texture.pixels)[i*4+2] = data[i*4+1];
        ((uchar*)texture.pixels)[i*4+3] = data[i*4+0];
    }

    stbi_image_free(data);

    if (!texture.pixels) {
        ERR("Failed to parse texture!");
        return texture;
    }
    return texture;
}

#endif
