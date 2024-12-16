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

#include "./renderer/renderer.hpp"
#include "./pad/pad.hpp"
#include "./audio/audio.hpp"
#include "./irx/irx_loader.hpp"
#include "./info/info.hpp"
#include "./info/banner.hpp"
#include "./game.hpp"

namespace Tyra {

struct EngineOptions {
  /**
   * True -> logs will be written to file.
   * False -> logs will be displayed in console
   */
  bool writeLogsToFile = false;

  bool loadUsbDriver = false;
};

class Engine {
 public:
  Engine();
  Engine(const EngineOptions& options);
  ~Engine();

  // Renderer renderer;
  // Pad pad;
  // Audio audio;
  // Info info;

  void run(Game* t_game);

 private:
  // IrxLoader irx;

  Game* game;
  Banner banner;

  void realLoop();
  void initAll(const bool& loadUsbDriver);
};
class EngineCoreData {
 public:
  EngineCoreData()
      : width(512.0F),
        height(448.0F),
        interlacedHeightF(height / 2),
        near(0.1F),
        far(51200.0F),
        projectionScale(4096.0F),
        aspectRatio(width / height),
        interlacedHeightUI(static_cast<unsigned int>(interlacedHeightF)) {}

  void print() const;
  std::string getPrint() const;

  float width;
  float height;
  float interlacedHeightF;
  float near;
  float far;
  float projectionScale;
  float aspectRatio;
  unsigned int interlacedHeightUI;
};

void InitEngine(const EngineOptions& options);

void BeginDrawing(void);
void beginFrame();
void endFrame();
void render(const Sprite& sprite);
EngineCoreData getSettings();
TextureRepository& getTextureRepository();

void setClearScreenColor(const Color& color);

}  // namespace Tyra
