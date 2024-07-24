/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
# Guido Diego Quispe Robles
*/
#pragma once

#include "loaders/texture/base/texture_loader.hpp"
#include "renderer/core/texture/models/texture.hpp"

#ifdef INTELLISENSE
#else
#include <jpeglib.h>
#endif

namespace Tyra {
class JpgLoader : public TextureLoader {
 public:
  JpgLoader();
  ~JpgLoader();

  inline TextureBuilderData* load(const std::string& fullpath) {
    return load(fullpath.c_str());
  }

  TextureBuilderData* load(const char* fullpath);
};
}  // namespace Tyra
