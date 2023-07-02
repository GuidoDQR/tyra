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

#include <tyra>

namespace Tyra {

class AlphaBlending : public Game {
 public:
  AlphaBlending(Engine* engine);
  ~AlphaBlending();

  void init();
  void loop();

 private:
  void loadTexture();
  void loadSprite();

  Engine* engine;

  Sprite sprite;
};

}  // namespace Tyra
