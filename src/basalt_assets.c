#include <time.h>

#include "basalt.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#include "external/stb_image.h"

class(AssetEntry)
{
    char filePath[MAX_PATH_LENGTH];
    char assetName[MAX_PATH_LENGTH];
    double lastPollTime;
    ulong lastEditTime;
};

#define HOTLOAD_INTERVAL 2.0

static usize AssetEntryCount = 0;
static AssetEntry* AssetEntries = NULL;

static const char* AssetFolders[] = {
    "assets", "../assets", "~/dev/basalt/assets", "C:\\dev\\basalt\\assets", NULL,
};

func bool GetAssetEntry(const char* name, AssetEntry** result)
{
    assert(name);
    assert(AssetEntries);
    for (usize i = 0; i < AssetEntryCount; i++) {
        const char* assetName = AssetEntries[i].assetName;
        if (strcmp(assetName, name) == 0) {
            *result = &AssetEntries[i];
            return true;
        }
    }
    return false;
}

func void LoadTextureFromStbData(Texture texture, uchar* data, int channels)
{
    assert(data);
    assert(texture.width > 0 || texture.height > 0);

    if (channels == 4 || channels == 3) {
        // HACK: Copy the texture into the correct color order
        // (by trial and error lol)
        uchar* comps = (uchar*)texture.pixels;
        for (int i = 0; i < texture.width * texture.height; i++) {
            ((uchar*)texture.pixels)[i * 4 + 0] = data[i * 4 + 3];
            ((uchar*)texture.pixels)[i * 4 + 1] = data[i * 4 + 2];
            ((uchar*)texture.pixels)[i * 4 + 2] = data[i * 4 + 1];
            ((uchar*)texture.pixels)[i * 4 + 3] = data[i * 4 + 0];
        }
    } else {
        ERR("Unexpected amount of channels in image (%d)!", channels);
    }
}

pubfunc void HotReloadTexture(Texture texture)
{
    if (!Config.hasHotloading || texture.name == NULL || strcmp(texture.name, "SPR_PIXELFONT") == 0)
        return;

    AssetEntry* entry;
    if (GetAssetEntry(texture.name, &entry)) {
        if (GetTimeElapsed() - entry->lastPollTime < HOTLOAD_INTERVAL)
            return;  // polled to recently
        entry->lastPollTime = GetTimeElapsed();

        ulong newEditTime = GetFileModifiedTime(entry->filePath);
        if (newEditTime > entry->lastEditTime) {
            entry->lastEditTime = newEditTime;

            // replace texture pixels and dimensions
            int channels = 3;
            int width, height;
            uchar* data = (uchar*)stbi_load(entry->filePath, &width, &height, &channels, 4);

            if (data) {
                // Re-alloc pixel memory when different width and height
                if (width != texture.width || height != texture.height) {
                    texture.width = width;
                    texture.height = height;
                    free(texture.pixels);
                    texture.pixels = calloc(sizeof(Color), width * height);
                    INFO("Resizing texture %s to %d x %d...", entry->assetName, width, height);
                }

                LoadTextureFromStbData(texture, data, channels);

                stbi_image_free(data);
                INFO("Hot swapped texture %s", entry->assetName);
            } else {
                ERR("Failed to hot-swap texture %s", entry->assetName);
            }
        }
    } else if (GetFrameIndex() % 300 == 0) {
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
    INFO("Found %d textures to be hot-reloaded", list.count);

    // Map to AssetEntries
    AssetEntryCount = list.count;
    AssetEntries = calloc(sizeof(AssetEntry), AssetEntryCount);

    for (usize i = 0; i < AssetEntryCount; i++) {
        AssetEntry* e = &AssetEntries[i];
        strcpy(e->filePath, list.strings[i]);

        char* fileStem = (char*)GetFileStem(list.strings[i]);
        ToUppercase(fileStem);
        strcpy(e->assetName, fileStem);

        e->lastPollTime = GetTimeElapsed();
        e->lastEditTime = GetFileModifiedTime(e->filePath);

        char* formattedModTime = ctime(&e->lastEditTime);
        DEBUG("HOTLOAD --> Found %s at %s (last edit %s)", e->assetName, e->filePath, formattedModTime);
    }

    UnloadFilePathList(list);
}

static uint LoadedTextureCapacity = 20;
static Texture* LoadedTextures = NULL;

// TODO: Support loading grayscale textures
pubfunc Texture RequestTextureEx(const char* name, const uchar* pixels)
{
    // try to load cached texture
    if (LoadedTextures == NULL)
        LoadedTextures = calloc(LoadedTextureCapacity, sizeof(Texture));

    uint loadedCount = 0;
    for (Texture* texture = LoadedTextures; texture->name != NULL; texture++) {
        if (strcmp(texture->name, name) == 0)
            return *texture;
        loadedCount++;
    }

    int width, height;
    int channels = 0;

    uint* pixels32Bit = (uint*)pixels;

    int size = pixels32Bit[0];
    uchar* data = (uchar*)stbi_load_from_memory((stbi_uc*)&pixels32Bit[1], size, &width, &height, &channels, 4);

    DEBUG("Loaded texture %s of size %u with %d channels", name, size, channels);

    Texture texture = InitTexture(width, height);
    texture.name = name;

    if (data) {
        LoadTextureFromStbData(texture, data, channels);
        stbi_image_free(data);
    } else {
        ERR("Failed to load texture %s", name);
    }

    // store texture in cache
    if (LoadedTextureCapacity > loadedCount) {
        LoadedTextureCapacity *= 2;
        LoadedTextures = realloc(LoadedTextures, LoadedTextureCapacity * sizeof(Texture));
        DEBUG("Reallocated texture cache to %d", LoadedTextureCapacity);
    }
    LoadedTextures[loadedCount++] = texture;

    return texture;
}

pubfunc Texture* GetLoadedTextures()
{
    return LoadedTextures;
}

// baked in assets (yes we really put every byte of a PNG in here)
const uchar SPR_PIXELFONT[] = {
    0x4A, 0x03, 0x00, 0x00, 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x40, 0x08, 0x06, 0x00, 0x00, 0x00, 0xAA, 0x69, 0x71, 0xDE, 0x00, 0x00, 0x00, 0x04, 0x73, 0x42, 0x49, 0x54, 0x08, 0x08, 0x08,
    0x08, 0x7C, 0x08, 0x64, 0x88, 0x00, 0x00, 0x03, 0x01, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9C, 0xED, 0x5B, 0xD9, 0x6E, 0x03, 0x21, 0x0C, 0xEC, 0xA2,
    0xFD, 0xFF, 0x4F, 0x4E, 0xFB, 0x52, 0x22, 0x8B, 0xF5, 0x35, 0x3E, 0xB2, 0xB4, 0xC9, 0x48, 0x95, 0xB2, 0x18, 0x1B, 0x73, 0x19, 0xDB, 0xD0, 0xE3,
    0xEB, 0x17, 0x8F, 0xC7, 0xE3, 0xFB, 0x8B, 0x60, 0x8C, 0x71, 0xD0, 0x6F, 0x4A, 0x5F, 0x69, 0x1C, 0xFF, 0x5A, 0x4F, 0xE3, 0xE7, 0x78, 0x69, 0xBD,
    0x0A, 0x3A, 0xD7, 0xE6, 0xB3, 0x8C, 0x13, 0x40, 0xCB, 0x56, 0xBA, 0xF5, 0xAD, 0xC9, 0xAA, 0xE0, 0xAF, 0xA0, 0xCF, 0xB2, 0xA1, 0x31, 0x76, 0x61,
    0x8C, 0x71, 0x58, 0x4A, 0xBF, 0x0A, 0xA7, 0xB7, 0xA2, 0xB5, 0x05, 0xB2, 0x40, 0xB6, 0x90, 0xA6, 0x87, 0xA4, 0xA7, 0x34, 0xE0, 0xA7, 0x55, 0x41,
    0x12, 0x56, 0x3D, 0x08, 0x96, 0x3C, 0x6F, 0x7B, 0x92, 0x5D, 0x90, 0xEC, 0xCE, 0x69, 0x55, 0xF8, 0xEF, 0xB8, 0xC5, 0x06, 0x74, 0xAC, 0xA0, 0x28,
    0x4A, 0x6C, 0x80, 0x64, 0xD4, 0x66, 0xBD, 0x95, 0x1E, 0x39, 0x46, 0x11, 0xFD, 0xC2, 0x88, 0x1E, 0x37, 0xD9, 0x63, 0x6E, 0xAD, 0xC3, 0xF1, 0x7B,
    0xDA, 0xA0, 0x7F, 0x9C, 0x7C, 0x8D, 0x9E, 0x82, 0xA6, 0xBC, 0x54, 0xC6, 0xD1, 0xA9, 0x72, 0xAB, 0x4C, 0x6D, 0x00, 0x50, 0x3F, 0x86, 0x96, 0x0D,
    0x89, 0x49, 0x53, 0x54, 0xA3, 0x67, 0xF6, 0xF6, 0x18, 0xE3, 0x90, 0xF8, 0xBB, 0x7C, 0x87, 0x32, 0x23, 0xB8, 0x93, 0x61, 0x43, 0xE0, 0x36, 0x82,
    0x16, 0xE6, 0x0C, 0x75, 0x0E, 0x42, 0xA6, 0x0D, 0xC9, 0x2F, 0x28, 0x1B, 0x80, 0x29, 0x74, 0xD7, 0x95, 0x20, 0x6E, 0xAD, 0x8E, 0x86, 0x3A, 0x9D,
    0xA8, 0x6A, 0xF9, 0xAE, 0x15, 0x80, 0x36, 0xC8, 0xAD, 0x84, 0xEC, 0x39, 0xEF, 0x69, 0x2F, 0x22, 0xFB, 0x12, 0x0B, 0x20, 0x0E, 0x8B, 0x27, 0xD8,
    0xD0, 0x94, 0x46, 0xEA, 0xAF, 0xBC, 0xEB, 0x31, 0xA7, 0xC6, 0xFB, 0x0C, 0x26, 0xFD, 0xD4, 0x04, 0x68, 0x0A, 0x23, 0x23, 0xAE, 0x25, 0x26, 0xEE,
    0xC6, 0x2D, 0xB1, 0xC0, 0x4E, 0x08, 0xC5, 0x02, 0x5E, 0xFA, 0x2E, 0xB3, 0xBD, 0xDA, 0x24, 0xFA, 0xDB, 0x3D, 0x00, 0x5A, 0x3E, 0xC0, 0x1B, 0xDC,
    0x44, 0xA1, 0x75, 0x00, 0x95, 0x31, 0x7F, 0x3F, 0xCB, 0xAB, 0x94, 0xDC, 0x1D, 0x73, 0xD0, 0x2E, 0x91, 0xA9, 0x54, 0x59, 0x13, 0xB6, 0x53, 0x4E,
    0x8F, 0x42, 0x8B, 0x04, 0xD7, 0x15, 0x7B, 0xC9, 0x08, 0x45, 0xCF, 0x51, 0x8E, 0x17, 0x85, 0xE7, 0x1C, 0x97, 0x96, 0x30, 0xA5, 0x45, 0xB6, 0x08,
    0xCB, 0xE0, 0x71, 0x62, 0x38, 0x45, 0xFE, 0x34, 0x38, 0x47, 0xC8, 0x53, 0x1F, 0x95, 0x5F, 0x81, 0x16, 0x57, 0x18, 0x09, 0x64, 0x76, 0x74, 0x68,
    0x2C, 0x94, 0x45, 0x83, 0x52, 0xE7, 0x3D, 0x36, 0x44, 0x73, 0xB3, 0x51, 0xCF, 0x33, 0xD2, 0x3E, 0x57, 0x06, 0x0D, 0x80, 0x36, 0xF3, 0x88, 0xCD,
    0x88, 0xDA, 0x94, 0x2C, 0x3F, 0x37, 0x01, 0x90, 0x27, 0x68, 0x05, 0x17, 0x5E, 0x59, 0x5D, 0xB0, 0x02, 0xB9, 0xF0, 0x0A, 0xF0, 0xEE, 0x79, 0x6B,
    0x86, 0xBA, 0x11, 0xB1, 0x4B, 0xA9, 0x58, 0x20, 0xBB, 0x4F, 0x35, 0x7A, 0xC4, 0xBD, 0xB6, 0xF8, 0x3F, 0xF8, 0xC0, 0x80, 0x75, 0xB3, 0xE2, 0xE1,
    0xCF, 0xD0, 0x6F, 0x45, 0xF6, 0x76, 0x67, 0x87, 0xF6, 0x2D, 0x6F, 0x56, 0xBD, 0x16, 0xF3, 0x28, 0x60, 0xDD, 0xAB, 0x65, 0x56, 0x80, 0x47, 0x79,
    0x8F, 0x6C, 0x49, 0x8E, 0xD4, 0xBF, 0x53, 0xAA, 0xC4, 0xA1, 0x3B, 0xB7, 0x57, 0x21, 0x17, 0x4E, 0x94, 0x50, 0x46, 0xED, 0x6E, 0xEE, 0x15, 0xB8,
    0x23, 0xCF, 0xE0, 0x7E, 0x22, 0x93, 0x81, 0x14, 0xC7, 0x73, 0xD0, 0x82, 0xB2, 0xCF, 0x39, 0xFF, 0x41, 0x3D, 0xC2, 0x69, 0x2F, 0x74, 0x09, 0x66,
    0xF9, 0x5F, 0x02, 0xED, 0x98, 0xB3, 0x8E, 0xC9, 0x4A, 0xFE, 0xC8, 0x51, 0x88, 0xFA, 0x11, 0x6C, 0x52, 0x34, 0x73, 0xCC, 0x79, 0xF8, 0xAD, 0x0E,
    0xDC, 0x91, 0x8D, 0x82, 0xEE, 0x05, 0x2C, 0x47, 0xC8, 0x6C, 0x8C, 0xBC, 0x1A, 0xE3, 0x68, 0x1E, 0xF9, 0xE5, 0x0F, 0x34, 0xA1, 0xCA, 0x0E, 0x5F,
    0x21, 0x73, 0xA7, 0xC0, 0x5D, 0x5C, 0x54, 0x81, 0xCA, 0x15, 0x57, 0x51, 0x74, 0xFF, 0xA1, 0xFC, 0x19, 0x57, 0x3B, 0x43, 0x97, 0xDA, 0x7E, 0x6B,
    0x6C, 0x73, 0x14, 0x79, 0xBD, 0xBC, 0x68, 0x56, 0x5A, 0xA5, 0x47, 0x96, 0xAD, 0xF4, 0x1D, 0x59, 0x62, 0x96, 0x6C, 0x5A, 0xEE, 0x59, 0xE2, 0x50,
    0xEA, 0x0C, 0x51, 0x34, 0x03, 0xCF, 0x1E, 0xB7, 0xF8, 0x3B, 0x9C, 0xA7, 0x4B, 0x52, 0x54, 0x6A, 0x88, 0x06, 0x29, 0x5C, 0x1D, 0xEB, 0x66, 0x29,
    0x9B, 0x31, 0x46, 0x72, 0xFE, 0x08, 0x1D, 0xBA, 0x18, 0xC9, 0xBE, 0x03, 0xEC, 0x9A, 0xC5, 0x09, 0xCF, 0x3B, 0xA5, 0xA9, 0xC7, 0xB3, 0x1C, 0x69,
    0x80, 0xAE, 0x00, 0x54, 0xB9, 0xEE, 0xCE, 0x47, 0x71, 0x19, 0x15, 0xE9, 0x1E, 0x7E, 0xED, 0x00, 0xD7, 0x21, 0xAD, 0x93, 0x96, 0x95, 0xF7, 0x9C,
    0x02, 0x5A, 0x9D, 0xD4, 0x29, 0xF0, 0xCE, 0x60, 0x67, 0x54, 0x9A, 0x5D, 0xFA, 0x6D, 0x5D, 0x86, 0x56, 0xCF, 0x50, 0xC5, 0x0C, 0x72, 0xFD, 0x82,
    0x8D, 0x60, 0x84, 0xB6, 0xD2, 0xB5, 0x58, 0x20, 0x43, 0x8F, 0x00, 0xCA, 0x0A, 0x77, 0xE3, 0x0E, 0x3F, 0x9E, 0xFD, 0xB7, 0x39, 0x8F, 0x67, 0xD8,
    0xB1, 0x44, 0xB5, 0x19, 0xF6, 0xE8, 0x17, 0x41, 0xF8, 0xA1, 0x24, 0x4A, 0xDF, 0x01, 0xAC, 0x83, 0x77, 0x87, 0x22, 0x5D, 0x88, 0x6C, 0xA1, 0xD6,
    0xB7, 0xC2, 0x28, 0xB2, 0x32, 0x10, 0x43, 0x3C, 0xF1, 0x03, 0x77, 0xCF, 0xD9, 0xBF, 0x5B, 0x02, 0x38, 0x54, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45,
    0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82,
};
