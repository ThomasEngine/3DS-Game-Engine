#include "sprite.h"


void Sprite::init(C2D_SpriteSheet spriteSheet, size_t spriteIndex, const EngineSettings& settings) {
    totalSprites = C2D_SpriteSheetCount(spriteSheet);
    sheet        = spriteSheet;
    currentFrame = spriteIndex;
    currentAnim  = nullptr;
    animTimer    = 0.0f;
    animFrameIndex = 0;
    finished     = false;

    C2D_SpriteFromSheet(&sprite, sheet, spriteIndex);
    C2D_SpriteSetCenter(&sprite, 0.5f, 0.5f);

    setScale(settings.entityScale, settings.entityScale);
}

void Sprite::playAnimation(const Animation* anim, bool restart) {
    if (currentAnim == anim && !restart) return;

    currentAnim    = anim;
    if (currentAnim->frameCount + currentAnim->frameCount > totalSprites) {
        currentAnim->frameCount = totalSprites;

    }
    animFrameIndex = 0;
    animTimer      = 0.0f;
    finished       = false;

    updateFrame();
}

void Sprite::stopAnimation() {
    currentAnim = nullptr;
    finished    = false;
}

void Sprite::update(float dt) {
    if (!currentAnim || finished) return;

    animTimer += dt;

    if (animTimer >= currentAnim->frameTime) {
        animTimer = 0.0f;
        animFrameIndex++;

        if (animFrameIndex >= currentAnim->frameCount) {
            if (currentAnim->loop) {
                animFrameIndex = 0;
            } else {
                animFrameIndex = currentAnim->frameCount - 1;
                finished = true;
            }
        }

        updateFrame();
    }
}

void Sprite::setPos(float x, float y) {
    C2D_SpriteSetPos(&sprite, x, y);
}

void Sprite::setScale(float x, float y) {
    C2D_SpriteSetScale(&sprite, x, y);
}

void Sprite::setRotation(float angle) {
    C2D_SpriteSetRotation(&sprite, angle);
}

void Sprite::setOrigin(float x, float y) {
    C2D_SpriteSetCenter(&sprite, x, y);
}

void Sprite::draw() const {
    // for debug purpeses draw red rect behind sprite
    // C2D_DrawParams param = sprite.params;
    // C2D_DrawRectSolid(param.pos.x,param.pos.y,0,param.pos.w,param.pos.h, color);
    //

    C2D_DrawSprite(&sprite);
}

void Sprite::updateFrame() {
    size_t index = currentAnim->startFrame + animFrameIndex;
    if (index >= totalSprites) return;
    C2D_DrawParams param = sprite.params;
    C2D_SpriteFromSheet(&sprite, sheet, index);
    sprite.params = param;
}