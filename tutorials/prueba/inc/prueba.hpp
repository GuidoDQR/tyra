/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

// voy a copiar una imagen ya procesada 
// y que se vea igual
#pragma once

#include <tyra>

namespace Tyra {

class Prueba : public Game {
 public:
  Prueba(Engine* engine);
  ~Prueba();

  void init();
  void loop();

 private:
  void loadTexture();
  void loadSprite();
  void perfectPixelSize(Sprite* sprite);
  void renderMask(Sprite* sprite, Sprite* mask);
  void getFrameBuffer(unsigned char* pixelBuffer);
  void createFrameTexture(unsigned char* data, Vec2 size, float scale);
  bool existFrameTexture(Sprite* framebuffer);
  void createTexture();
  Engine* engine;
  Pad* pad;
  
  //RendererCoreTextureSender sender;
  Sprite sprite;
  Sprite spr_frameBufferBefore;
  Sprite spr_frameBuffer;
  Sprite mask;
  Sprite punto;
  unsigned char* pixel_char;
  //alignas(128) unsigned char pixel_char[512*448*4];
  unsigned char* pixel_frameBuffer_before; // pixeles antes del mask
};

}  // namespace Tyra
