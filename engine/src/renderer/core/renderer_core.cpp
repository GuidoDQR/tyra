/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/renderer_core.hpp"
#include "thread/threading.hpp"

namespace Tyra {

/** Responsible for initializing GS. */
RendererCoreGS gs;

/** All logic responsible for 3D drawing. */
RendererCore3D renderer3D;

/** All logic responsible for 2D drawing. */
RendererCore2D renderer2D;

/** Texture transferring. */
RendererCoreTexture texture;

/** EE <-> VU1 synchronization */
RendererCoreSync sync;
Color bgColor;
RendererSettings settings;
Path3 path3;
Path1 path1;

RendererCore::RendererCore() { isFrameLimitOn = true; }
RendererCore::~RendererCore() {}

void RendererCore::init() {
  path3.init(&settings);
  sync.init(&path3, &path1);
  gs.init(&settings);
  texture.init(&gs, &path3);
  renderer3D.init(&settings, &path1);
  renderer2D.init(&settings, &texture.getClutBuffer());
}

RendererCoreGS& getCoreGS() { return gs; }

RendererCore3D& getCore3D() { return renderer3D; }

RendererCore2D& getCore2D() { return renderer2D; }

RendererCoreTexture& getCoreTexture() { return texture; }

RendererCoreSync& getCoreSync() { return sync; }

void RendererCore::setClearScreenColor(const Color& color) { bgColor = color; }

void RendererCore::beginFrame() {
  renderer3D.update();
  Threading::switchThread();
  path3.clearScreen(&gs.zBuffer, bgColor);
}

void RendererCore::beginFrame(const CameraInfo3D& cameraInfo) {
  renderer3D.update(cameraInfo);
  Threading::switchThread();
  path3.clearScreen(&gs.zBuffer, bgColor);
}

void RendererCore::endFrame() {
  Threading::switchThread();
  if (isFrameLimitOn) graph_wait_vsync();
  gs.flipBuffers();
}

const RendererSettings& RendererCore::getSettings() { return settings; }

Path1* RendererCore::getPath1() { return &path1; }

Path3* RendererCore::getPath3() { return &path3; }

}  // namespace Tyra
