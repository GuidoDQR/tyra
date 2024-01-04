/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#pragma once

#include "loaders/texture/base/texture_loader.hpp"
#include "renderer/core/texture/models/texture.hpp"

#ifdef INTELLISENSE
typedef void* png_structp;
typedef void* png_bytep;
typedef void* png_infop;
typedef void* png_colorp;
#else
#include <png.h>
#endif

namespace Tyra {

struct PngPixel3 {
  u8 r, g, b;
};

struct PngPixel4 {
  u8 r, g, b, a;
};

class PngLoader : public TextureLoader {
 public:
  PngLoader();
  ~PngLoader();

  inline TextureBuilderData* load(const std::string& fullpath, int t_rectX, int t_rectY, int t_rectWidth, int t_rectHeight, const bool t_rect) {
    return load(fullpath.c_str(),t_rectX,t_rectY,t_rectWidth,t_rectHeight, t_rect);
  }

  TextureBuilderData* load(const char* fullpath, int t_rectX, int t_rectY, int t_rectWidth, int t_rectHeight, const bool t_rect);

 private:
  int rectX, rectY; //rectWidth, rectHeight;
  png_uint_32 width, height;

  void handle32bpp(TextureBuilderData* result, png_structp pngPtr,
                   png_infop infoPtr, png_bytep* rowPointers);

  void handle24bpp(TextureBuilderData* result, png_structp pngPtr,
                   png_infop infoPtr, png_bytep* rowPointers);

  void handlePalletized(TextureBuilderData* result, png_structp pngPtr,
                        png_infop infoPtr, png_bytep* rowPointers,
                        const int& bitDepth);

  void handle8bppPalletized(TextureBuilderData* result, png_structp pngPtr,
                            png_infop infoPtr, png_bytep* rowPointers,
                            png_colorp palette, png_bytep trans,
                            const int& numPallete, const int& numTrans);

  void handle4bppPalletized(TextureBuilderData* result, png_structp pngPtr,
                            png_infop infoPtr, png_bytep* rowPointers,
                            png_colorp palette, png_bytep trans,
                            const int& numPallete, const int& numTrans);
};

}  // namespace Tyra
