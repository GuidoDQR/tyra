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

#include <tamtypes.h>
#include "./2d/renderer_core_2d.hpp"
#include "./3d/renderer_core_3d.hpp"
#include "./gs/renderer_core_gs.hpp"
#include "./texture/renderer_core_texture.hpp"
#include "./paths/path3/path3.hpp"
#include "./paths/path1/path1.hpp"
#include "./renderer_core_sync.hpp"

namespace Tyra {

class RendererCore {
 public:
  RendererCore();
  ~RendererCore();

  // /** Responsible for initializing GS. */
  RendererCoreGS& getCoreGS();

  // /** All logic responsible for 3D drawing. */
  RendererCore3D& getCore3D();

  // /** All logic responsible for 2D drawing. */
  RendererCore2D& getCore2D();

  // /** Texture transferring. */
  RendererCoreTexture& getCoreTexture();

  // /** EE <-> VU1 synchronization */
  RendererCoreSync& getCoreSync();

  /** Called by renderer */
  void init();

  /** World background color */
  void setClearScreenColor(const Color& color);

  /** Clear screen and update view frustum for frustum culling. NO 3D support */
  void beginFrame();

  /** Clear screen and update view frustum for frustum culling. 3D support */
  void beginFrame(const CameraInfo3D& cameraInfo);

  /** VSync and swap frame double buffer. */
  void endFrame();

  void setFrameLimit(const bool& onoff) { isFrameLimitOn = onoff; }

  /** Get screen settings */
  const RendererSettings& getSettings();

  Path1* getPath1();

  Path3* getPath3();

 private:
  bool isFrameLimitOn;
};

}  // namespace Tyra
