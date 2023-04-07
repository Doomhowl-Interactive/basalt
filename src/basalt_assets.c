#include "basalt.h"
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#include "external/stb_image.h"

class(AssetEntry) {
    char filePath[MAX_PATH_LENGTH];
    char assetName[MAX_PATH_LENGTH];
    double lastPollTime;
    ulong lastEditTime;
};

#define HOTLOAD_INTERVAL 2.0

static usize AssetEntryCount = 0;
static AssetEntry* AssetEntries = NULL;

static const char* AssetFolders[] = {
    "assets",
    "../assets",
    "~/dev/basalt/assets",
    "C:\\dev\\basalt\\assets",
    NULL,
};

func bool GetAssetEntry(const char* name, AssetEntry** result)
{
    assert(name);
    assert(AssetEntries);
    for (usize i = 0; i < AssetEntryCount; i++)
    {
        const char* assetName = AssetEntries[i].assetName;
        if (strcmp(assetName, name) == 0)
        {
            *result = &AssetEntries[i];
            return true;
        }
    }
    return false;
}

func void LoadTextureFromStbData(Texture texture, uchar* data, int channels) {
    assert(data);
    assert(texture.width > 0 || texture.height > 0);

    if (channels == 4 || channels == 3)
    {
        // HACK: Copy the texture into the correct color order
        // (by trial and error lol)
        uchar* comps = (uchar*) texture.pixels;
        for (int i = 0; i < texture.width * texture.height; i++) {
            ((uchar*)texture.pixels)[i*4+0] = data[i*4+3];
            ((uchar*)texture.pixels)[i*4+1] = data[i*4+2];
            ((uchar*)texture.pixels)[i*4+2] = data[i*4+1];
            ((uchar*)texture.pixels)[i*4+3] = data[i*4+0];
        }
    }
    else
    {
        ERR("Unexpected amount of channels in image (%d)!", channels);
    }
}

pubfunc void HotReloadTexture(Texture texture)
{
    if (!Config.hasHotloading || texture.name == NULL)
        return;

    AssetEntry* entry;
    if (GetAssetEntry(texture.name, &entry))
    {
        if (GetTimeElapsed() - entry->lastPollTime < HOTLOAD_INTERVAL)
            return; // polled to recently
        entry->lastPollTime = GetTimeElapsed();

        ulong newEditTime = GetFileModifiedTime(entry->filePath);
        if (newEditTime > entry->lastEditTime)
        {
            entry->lastEditTime = newEditTime;

            // replace texture pixels and dimensions
            int channels = 3;
            int width, height;
            uchar* data = (uchar*) stbi_load(entry->filePath, &width, &height,
                                             &channels, 4);

            if (data)
            {
                // Re-alloc pixel memory when different width and height
                if (width != texture.width || height != texture.height)
                {
                    texture.width = width;
                    texture.height = height;
                    free(texture.pixels);
                    texture.pixels = calloc(sizeof(Color), width*height);
                    INFO("Resizing texture %s to %d x %d...", entry->assetName, width, height);
                }

                LoadTextureFromStbData(texture, data, channels);

                stbi_image_free(data);
                INFO("Hot swapped texture %s", entry->assetName);
            }
            else 
            {
                ERR("Failed to hot-swap texture %s", entry->assetName);
            }
        }
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
        strcpy(e->filePath, list.strings[i]);

        char* fileStem = (char*) GetFileStem(list.strings[i]);
        ToUppercase(fileStem);
        strcpy(e->assetName, fileStem);

        e->lastPollTime = GetTimeElapsed();
        e->lastEditTime = GetFileModifiedTime(e->filePath);

        char* formattedModTime = ctime(&e->lastEditTime);
        DEBUG("HOTLOAD --> Found %s at %s (last edit %s)",e->assetName, e->filePath, formattedModTime);
    }

    UnloadFilePathList(list);
}

pubfunc Texture LoadTextureEx(const char* name, uchar* pixels)
{
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

    if (data)
    {
        LoadTextureFromStbData(texture, data, channels);
        stbi_image_free(data);
    }
    else 
    {
        ERR("Failed to load texture %s", name);
    }
    return texture;
}
