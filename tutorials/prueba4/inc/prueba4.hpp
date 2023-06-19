/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

// dibujo el sprite 32x32(x4) en el framebuffer
// copio los pixeles del framebuffer
// creo un sprite 128x128(x1)
// creo una textura para el sprite
// la textura sera 128x128
// la textura copiara el id del sprite
// con ese id obtengo el texturebuffer
// modifico ese texture buffer
// dibujo la textura 
// el texture buffer lo dejo igual que antes

#pragma once

#include <tyra>

// funciona pero no se dibuja bien por alguna razon
namespace Tyra {

class Prueba4 : public Game {
 public:
  Prueba4(Engine* engine);
  ~Prueba4();

  void init();
  void loop();

 private:
  void loadTexture();
  void loadSprite();
  void perfectPixelSize(Sprite* sprite);
  int renderMask(Sprite* sprite, Sprite* mask);
  void getFrameBuffer(unsigned char* pixelBuffer);
  void createFrameTexture(unsigned char* data, Vec2 size, float scale);
  bool existFrameSprite(Sprite* sprite);
  bool existFrameTexture(Sprite* sprite,Sprite* sprite2);
  void createTexture();
  Engine* engine;
  Pad* pad;
  
  //RendererCoreTextureSender sender;
  Sprite sprite;
  Sprite spr_frameBufferBefore;
  Sprite spr_frameBuffer;
  Sprite mask;
  Sprite punto;
  std::vector<u32> id_spr;
  std::vector<Sprite> spr_effects; // sprites para guardar el tamaño verdadero de los sprites texturas 

  unsigned char* pixel_fb;
  //alignas(128) unsigned char pixel_char[512*448*4];
  unsigned char* pixel_frameBuffer_before; // pixeles antes del mask
  unsigned char* pixel_fb_after;
  bool drawFB;
};

}  // namespace Tyra
