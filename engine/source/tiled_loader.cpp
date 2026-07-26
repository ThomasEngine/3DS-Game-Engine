#include "tiled_loader.h"
#include "cJSON.h"
#include <cstdio>
#include <cstring>

// Mostly made with ai
void parseProperties(cJSON* propsArray, std::vector<TiledProperty>& out) {
    if (!cJSON_IsArray(propsArray)) return;

    cJSON* prop = nullptr;
    cJSON_ArrayForEach(prop, propsArray) {
        TiledProperty p;

        cJSON* pn = cJSON_GetObjectItem(prop, "name");
        cJSON* pt = cJSON_GetObjectItem(prop, "type");
        cJSON* pv = cJSON_GetObjectItem(prop, "value");

        if (cJSON_IsString(pn)) p.name = pn->valuestring;
        if (cJSON_IsString(pt)) p.type = pt->valuestring;

        if (pv) {
            if (cJSON_IsString(pv)) {
                p.value_s = pv->valuestring;
            } else if (cJSON_IsNumber(pv)) {
                p.value_f = (float)pv->valuedouble;
                p.value_i = pv->valueint;
            } else if (cJSON_IsBool(pv)) {
                p.value_b = cJSON_IsTrue(pv);
            }
        }

        out.push_back(std::move(p));
    }
}

void parseTileLayer(cJSON* layerJson, TiledMap& out) {
    TiledLayer layer;

    cJSON* name = cJSON_GetObjectItem(layerJson, "name");
    if (cJSON_IsString(name)) layer.name = name->valuestring;

    cJSON* w = cJSON_GetObjectItem(layerJson, "width");
    cJSON* h = cJSON_GetObjectItem(layerJson, "height");
    if (cJSON_IsNumber(w)) layer.width  = w->valueint;
    if (cJSON_IsNumber(h)) layer.height = h->valueint;

    cJSON* px = cJSON_GetObjectItem(layerJson, "parallaxx");
    if (cJSON_IsNumber(px)) layer.parallax = (float)px->valuedouble;
    else                    layer.parallax = 1.0f;

    cJSON* data = cJSON_GetObjectItem(layerJson, "data");
    if (cJSON_IsArray(data)) {
        layer.tiles.reserve(cJSON_GetArraySize(data));
        cJSON* val = nullptr;
        cJSON_ArrayForEach(val, data) {
            if (cJSON_IsNumber(val)) {
                // valuedouble handles GIDs with flip bits that exceed int range
                layer.tiles.push_back((uint32_t)val->valuedouble);
            } else {
                layer.tiles.push_back(0);
            }
        }
    }

    out.layers.push_back(std::move(layer));
}

void parseObjectLayer(cJSON* layerJson, TiledMap& out) {
    TiledObjectLayer objLayer;

    cJSON* name = cJSON_GetObjectItem(layerJson, "name");
    if (cJSON_IsString(name)) objLayer.name = name->valuestring;

    cJSON* objects = cJSON_GetObjectItem(layerJson, "objects");
    if (!cJSON_IsArray(objects)) {
        out.objectLayers.push_back(std::move(objLayer));
        return;
    }

    cJSON* objJson = nullptr;
    cJSON_ArrayForEach(objJson, objects) {
        TiledObject obj;

        cJSON* id = cJSON_GetObjectItem(objJson, "id");
        if (cJSON_IsNumber(id)) obj.id = id->valueint;

        cJSON* n = cJSON_GetObjectItem(objJson, "name");
        if (cJSON_IsString(n)) obj.name = n->valuestring;

        cJSON* t = cJSON_GetObjectItem(objJson, "type");
        if (cJSON_IsString(t)) obj.type = t->valuestring;

        cJSON* x = cJSON_GetObjectItem(objJson, "x");
        cJSON* y = cJSON_GetObjectItem(objJson, "y");
        if (cJSON_IsNumber(x)) obj.x = (float)x->valuedouble;
        if (cJSON_IsNumber(y)) obj.y = (float)y->valuedouble;

        cJSON* w = cJSON_GetObjectItem(objJson, "width");
        cJSON* h = cJSON_GetObjectItem(objJson, "height");
        if (cJSON_IsNumber(w)) obj.width  = (float)w->valuedouble;
        if (cJSON_IsNumber(h)) obj.height = (float)h->valuedouble;

        cJSON* pt = cJSON_GetObjectItem(objJson, "point");
        if (cJSON_IsBool(pt)) obj.point = cJSON_IsTrue(pt);

        cJSON* props = cJSON_GetObjectItem(objJson, "properties");
        parseProperties(props, obj.properties);

        objLayer.objects.push_back(std::move(obj));
    }

    out.objectLayers.push_back(std::move(objLayer));
}


bool loadTiledMap(const std::string& filePath, TiledMap& out) {
    // --- read the whole file into memory ---
    FILE* f = fopen(filePath.c_str(), "rb");
    if (!f) return false;

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

    // --- parse JSON ---
    cJSON* root = cJSON_Parse(buffer);
    free(buffer);
    if (!root) return false;

    // --- map-level fields ---
    cJSON* tw = cJSON_GetObjectItem(root, "tilewidth");
    cJSON* th = cJSON_GetObjectItem(root, "tileheight");
    cJSON* mw = cJSON_GetObjectItem(root, "width");
    cJSON* mh = cJSON_GetObjectItem(root, "height");
    if (cJSON_IsNumber(tw)) out.tileWidth  = tw->valueint;
    if (cJSON_IsNumber(th)) out.tileHeight = th->valueint;
    if (cJSON_IsNumber(mw)) out.width      = mw->valueint;
    if (cJSON_IsNumber(mh)) out.height     = mh->valueint;

    // --- iterate layers (tile layers + object layers) ---
    cJSON* layers = cJSON_GetObjectItem(root, "layers");
    if (!cJSON_IsArray(layers)) {
        cJSON_Delete(root);
        return false;
    }

    cJSON* layerJson = nullptr;
    cJSON_ArrayForEach(layerJson, layers) {
        cJSON* type = cJSON_GetObjectItem(layerJson, "type");
        if (!cJSON_IsString(type)) continue;

        const char* typeStr = type->valuestring;

        if (strcmp(typeStr, "tilelayer") == 0) {
            parseTileLayer(layerJson, out);
        } else if (strcmp(typeStr, "objectgroup") == 0) {
            parseObjectLayer(layerJson, out);
        }
        // silently skip other types (imagelayer, group, etc.)
    }

    cJSON_Delete(root);
    return true;
}

const TiledProperty* findObjectProperty(const TiledObject& obj, const std::string& name) {
    for (const auto& p : obj.properties) {
        if (p.name == name) return &p;
    }
    return nullptr;
}