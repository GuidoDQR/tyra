/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2024, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Guido Diego Quispe Robles
*/

#include <tyra>
#include "tutorial_12.hpp"

namespace Tyra {

Tutorial12::Tutorial12(Engine* t_engine) : engine(t_engine) {}

Tutorial12::~Tutorial12() {
  /** It is necessary to free up memory. */
  engine->font.unloadFontDataVRAM(&myFont);
  engine->font.unloadFontDataVRAM(&myEmojiFont);
}

void Tutorial12::init() {
  Tyra::Renderer& renderer = engine->renderer;

  renderer.setClearScreenColor(Tyra::Color(32.0F, 32.0F, 32.0F));

  /** How to load the texture of an individual character/s.*/

  unsigned int count;
  int codePoint = engine->font.getCodepoint("🥳", &count);
  std::vector<int> codePoints;
  codePoints.push_back(codePoint);

  engine->font.loadFontFromMemory(&myEmojiFont,
                                  FileUtils::fromCwd("seguiemj.ttf").c_str(),
                                  32, codePoints.data(), codePoints.size());

  /**
   * Loads chars with ASCII values 32-127 in one texture of 256x256.
   *
   * If you use a char with a value that is not between 32 and 127
   * or change the font size, the DrawText function will find the char anyway
   * and rewrite the texture.
   * (in case the char is inside on the font).
   *
   * If the new char can not be inside the texture,
   * the texture data is going to be erased;
   */

  engine->font.loadFont(&myFont, 32,
                        FileUtils::fromCwd("roboto-bold.ttf").c_str());

  /** Setting the font texture to a sprite */
  engine->renderer.getTextureRepository()
      .getByTextureId(myFont.textureID)
      ->addLink(fontSprite.id);

  const auto& screenSettings = engine->renderer.core.getSettings();

  fontSprite.position = Vec2(screenSettings.getWidth() / 2.0F - 256 / 2.0F,
                             screenSettings.getHeight() / 2.0F - 256 / 2.0F);

  fontSprite.mode = SpriteMode::MODE_REPEAT;
  fontSprite.size = Vec2(256, 256);

  /** Setting colors */

  white = Color(255.0f, 255.0f, 255.0f, 128.0f);
  skyBlue = Color(32.0f, 164.0f, 243.0f, 128.0f);
}

void Tutorial12::loop() {
  auto& renderer = engine->renderer;
  auto& font = engine->font;

  /** Begin frame will clear our screen. */
  renderer.beginFrame();

  changeFontSize();

  /** Render font. */

  /** It uses the freetype library,
   *  can support truetype and opentype fonts. */

  font.drawText(&myFont, "Hello world in", 20, 60, 32, white);
  font.drawText(&myFont, "TYRA", 222, 60, fontSize, skyBlue);
  font.drawText(&myEmojiFont, "🥳⭐", 320, 60, 32, white);

  /** Render Font Texture Atlas */
  renderer.renderer2D.render(fontSprite);

  /** End frame will perform vsync. */
  renderer.endFrame();
}

void Tutorial12::changeFontSize() {
  if (time == 10) {
    time = 0;
    fontSize += val;
    if (fontSize == 39) {
      val = -1;
    } else if (fontSize == 32) {
      val = 1;
    }
  }
  time++;
}
}  // namespace Tyra