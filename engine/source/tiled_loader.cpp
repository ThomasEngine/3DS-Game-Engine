#include "tiled_loader.h"
#include "cJSON.h"
// engine/source/tiled_loader.cpp
#include "tiled_loader.h"
#include "cJSON.h"

#include <cstdio>
#include <cstdlib>

// TILED MAP LOADER MADE WITH AI
bool loadTiledMap(const std::string& filePath, TiledMap& out) {
    // --- 1. read the whole file into memory ---
    FILE* f = fopen(filePath.c_str(), "rb");
    if (!f) {
        return false;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size <= 0) {
        fclose(f);
        return false;
    }

    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fclose(f);
        return false;
    }

    size_t read = fread(buffer, 1, size, f);
    buffer[read] = '\0';
    fclose(f);

    // --- 2. parse JSON ---
    cJSON* root = cJSON_Parse(buffer);
    free(buffer);   // cJSON copies what it needs, safe to free now
    if (!root) {
        return false;
    }

    // --- 3. map-level fields ---
    cJSON* tw = cJSON_GetObjectItem(root, "tilewidth");
    cJSON* th = cJSON_GetObjectItem(root, "tileheight");
    if (cJSON_IsNumber(tw)) out.tileWidth  = tw->valueint;
    if (cJSON_IsNumber(th)) out.tileHeight = th->valueint;

    cJSON* mw = cJSON_GetObjectItem(root, "width");
    cJSON* mh = cJSON_GetObjectItem(root, "height");
    if (cJSON_IsNumber(mw)) out.width = mw->valueint;
    if (cJSON_IsNumber(mh)) out.height = mh->valueint;

    // --- 4. iterate layers ---
    cJSON* layers = cJSON_GetObjectItem(root, "layers");
    if (!cJSON_IsArray(layers)) {
        cJSON_Delete(root);
        return false;
    }

    int layerCount = cJSON_GetArraySize(layers);
    for (int i = 0; i < layerCount; i++) {
        cJSON* layerJson = cJSON_GetArrayItem(layers, i);
        if (!layerJson) continue;

        // only handle tile layers
        cJSON* type = cJSON_GetObjectItem(layerJson, "type");
        if (!cJSON_IsString(type) || std::string(type->valuestring) != "tilelayer") {
            continue;
        }

        TiledLayer layer;

        cJSON* name = cJSON_GetObjectItem(layerJson, "name");
        if (cJSON_IsString(name)) layer.name = name->valuestring;

        cJSON* w = cJSON_GetObjectItem(layerJson, "width");
        cJSON* h = cJSON_GetObjectItem(layerJson, "height");
        if (cJSON_IsNumber(w)) layer.width  = w->valueint;
        if (cJSON_IsNumber(h)) layer.height = h->valueint;

        // parallax (Tiled uses parallaxx/parallaxy, default 1.0)
        cJSON* px = cJSON_GetObjectItem(layerJson, "parallaxx");
        if (cJSON_IsNumber(px)) layer.parallax = (float)px->valuedouble;
        else                    layer.parallax = 1.0f;

        // tile data — flat array of GIDs, row-major
        cJSON* data = cJSON_GetObjectItem(layerJson, "data");
        if (cJSON_IsArray(data)) {
            int count = cJSON_GetArraySize(data);
            layer.tiles.reserve(count);
            for (int t = 0; t < count; t++) {
                cJSON* val = cJSON_GetArrayItem(data, t);
                if (cJSON_IsNumber(val)) {
                    // valuedouble used because GIDs with flip bits can exceed int range
                    layer.tiles.push_back((uint32_t)val->valuedouble);
                } else {
                    layer.tiles.push_back(0);
                }
            }
        }

        out.layers.push_back(std::move(layer));
    }

    cJSON_Delete(root);
    return true;
}