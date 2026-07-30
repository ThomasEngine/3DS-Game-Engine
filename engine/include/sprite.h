#pragma once
#include <3ds.h>
#include <citro2d.h>

#include "engine_settings.h"

struct Animation {
    size_t startFrame;
    size_t frameCount;
    float frameTime;
    bool loop;
};

class Sprite {
public:
    Sprite() = default;

    ~Sprite() = default;

    void init(C2D_SpriteSheet sheet, size_t spriteIndex, const EngineSettings &settings);

    void destroy();

    // Play an animation
    void playAnimation(const Animation *anim, bool restart = false);

    void stopAnimation();

    // Update every tick
    void update(float dt);

    bool isFinished() const { return finished; }

    // Transform
    void setPos(float x, float y);

    void setScale(float x, float y);

    void setRotation(float angle);

    void setOrigin(float x, float y);

    void setCurrentFrame(size_t frame);

    float getX() const { return sprite.params.pos.x; }
    float getY() const { return sprite.params.pos.y; }
    float getWidth() const { return sprite.params.pos.w; }
    float getHeight() const { return sprite.params.pos.h; }
    float getRotation() const { return sprite.params.angle; }

    void setFlipX(bool flip) {
        flipX = flip;
        applyTransform();
    };

    void setFlipY(bool flip) {
        flipY = flip;
        applyTransform();
    };

    void draw() const;

    uint32_t color = C2D_Color32(0, 255, 0, 255);

private:
    void updateFrame();

    void applyTransform();

    float scaleX = 1.0f;
    float scaleY = 1.0f;

    bool flipX = false;
    bool flipY = false;

    C2D_Sprite sprite;
    C2D_SpriteSheet sheet = nullptr;
    size_t currentFrame;
    const Animation *currentAnim = nullptr;
    float animTimer;
    size_t animFrameIndex;
    size_t totalSprites;
    bool finished;
};
