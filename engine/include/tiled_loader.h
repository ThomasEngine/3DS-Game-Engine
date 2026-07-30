#pragma once
#include <string>
#include <vector>
#include <stdint.h>

// namespace Engine {
struct TiledLayer {
    std::vector<uint32_t> tiles;
    float parallax = 1.0f;
    std::string name;
    int width = 0;
    int height = 0;
};

struct TiledProperty {
    std::string name;
    std::string type;
    std::string value_s;
    float value_f = 0.0f;
    int value_i = 0;
    bool value_b = false;
};

struct TiledObject {
    int id = 0;
    std::string name;
    std::string type;
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    bool point = false;
    std::vector<TiledProperty> properties;
};

struct TiledObjectLayer {
    std::string name;
    std::vector<TiledObject> objects;
};

struct TiledMap {
    std::vector<TiledLayer> layers;
    std::vector<TiledObjectLayer> objectLayers;
    int tileWidth = 0;
    int tileHeight = 0;
    int width = 0;
    int height = 0;
};

bool loadTiledMap(const std::string &filePath, TiledMap &outMap);

const TiledProperty *findObjectProperty(const TiledObject &obj, const std::string &name);
