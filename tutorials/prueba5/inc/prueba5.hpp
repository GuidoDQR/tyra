/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

/*
  agarro el sprite
  agarro la textura del sprite
  verifico que el size del sprite sea igual al size de la textura
  si 
*/
/*
la forma mas facil es hacer que el sprite
que tenga la imagen original
obtenga la nueva textura con los nuevos datos
de pixeles si se cambio el tamaño del sprite
pero se tiene que cambiar su size
*/
#pragma once

#include <tyra>

// funciona pero no se dibuja bien por alguna razon
namespace Tyra {

class Prueba5 : public Game {
 public:
  Prueba5(Engine* engine);
  ~Prueba5();

  void init();
  void loop();

 private:
  void loadTexture();
  void loadSprite();
  void perfectPixelSize(Sprite* sprite);
  void reScaleTexture(Sprite* sprite,Sprite* spr);

  int renderMask(Sprite* sprite, Sprite* mask);
  void createFrameTexture(unsigned char* data, Vec2 size, float scale);
  bool existFrameSprite(Sprite* sprite);
  bool existFrameTexture(Sprite* sprite,Sprite* sprite2);
  void createTexture();
  Engine* engine;
  Pad* pad;
  
  Sprite sprite;
  Sprite mask;
  Sprite punto;
  std::vector<u32> id_spr;
  std::vector<Sprite> spr_effects; // sprites para guardar el tamaño actual de los sprites
  std::vector<unsigned char*> tex_data; // util para guardar los datos originales de la textura modificada
};

}  // namespace Tyra
