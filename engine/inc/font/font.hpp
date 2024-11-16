/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2024, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Guido Diego Quispe Robles
*/

#pragma once

#include <renderer/core/2d/sprite/sprite.hpp>
#include <renderer/renderer.hpp>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Tyra {

class TextureAtlas {
 public:
  unsigned int x = 0;
  unsigned int y = 0;
  unsigned int maxHeight = 0;
};

struct GlyphID {
  int fontSize;
  int glyphCodepoint;
};

struct Glyph {
  Vec2 posAtlas;
  Vec2 size;
  Vec2 bearing;
  int advanceX;
};

class FontData {
 public:
  FontData();
  u32 dataID;
  u32 textureID;  // texture of 256x256
  FT_Face face;
  std::vector<GlyphID> glyphID;
  std::vector<Glyph> glyph;
  TextureAtlas textureAtlas;
};

class Font {
 public:
  ~Font();
  void init(Renderer* renderer);

  void loadFont(FontData* font, int fontSize, const char* filePath);
  void loadFontFromMemory(FontData* font, const char* filePath, int fontSize,
                          int* codePoints, const int codePointCount);

  void unloadGlyphs(FontData* font);

  /** Unload the RAM and invalidate the fontID*/
  void unloadFontDataRAM(FontData* font);

  /** Unload the RAM and texture of the font also invalidate the IDs*/
  void unloadFontDataVRAM(FontData* font);

  bool fontIsValid(u32 fontID, u32 textureID);

  /**
   * Minimum value is 256.
   * @param maxID This value is used when the fontID is not valid*/
  void setMaxIDFont(u32 maxID);

  int getCodepoint(const char* text, unsigned int* bytes);

  void drawText(FontData* font, const char* text, float x, float y,
                int fontSize, Color color);

 private:
  bool getGlyphIndex(FontData* font, unsigned int* glyph, const int fontSize,
                     const int codepoint);
  void createGlyphTexture(FontData* font, const int fontSize,
                          const int codepoint);
};
}  // namespace Tyra