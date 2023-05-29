/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include <tyra>
#include "tutorial_11.hpp"

namespace Tyra {

Tutorial11::Tutorial11(Engine* t_engine) : engine(t_engine), pad(&t_engine->pad) {}

Tutorial11::~Tutorial11() {
  engine->renderer.getTextureRepository().freeBySprite(sprite);
}

void Tutorial11::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

  /** Sprite contains rectangle information. */
  loadSprite();

  /** Texture contains png image. */
  loadTexture();

  engine->renderer.core.renderer2D.setTextureMappingType(TyraNearest);

  engine->renderer.core.renderer2D.UV.x = sprite.size.x;
  engine->renderer.core.renderer2D.UV.y = sprite.size.y;
}

void Tutorial11::loop() {
  auto& renderer = engine->renderer;

  if(pad->getClicked().Cross){
    engine->renderer.core.renderer2D.setTextureMappingType(TyraNearest);
  }else if(pad->getClicked().Circle){
    engine->renderer.core.renderer2D.setTextureMappingType(TyraLinear);
  }

  if(pad->getLeftJoyPad().v <= 100){
    mask.position -= Vec2(0,1); //up
  }else if(pad->getLeftJoyPad().v >= 200){
    mask.position += Vec2(0,1); //down
  }

   if (pad->getLeftJoyPad().h <= 100) {
      mask.position -= Vec2(1,0); // left
   } else if (pad->getLeftJoyPad().h >= 200) {
      mask.position += Vec2(1,0); //right
   }

  /** Begin frame will clear our screen. */
  renderer.beginFrame();

  /** Render sprite. */
  renderer.renderer2D.render(sprite);
  //renderer.renderer2D.render(mask);

  /** End frame will perform vsync. */
  renderer.endFrame();
}

void Tutorial11::loadSprite() {
  const auto& screenSettings = engine->renderer.core.getSettings();

  sprite.mode = SpriteMode::MODE_STRETCH;

  /** Let's scale it down */
  sprite.size = Vec2(31.0F, 31.0F);
  sprite.scale = 4;
  /** Set it in screen center */
  sprite.position = Vec2(0.0F,0.0F);
      /*Vec2(screenSettings.getWidth() / 2.0F - sprite.size.x / 2.0F,
           screenSettings.getHeight() / 2.0F - sprite.size.y / 2.0F);*/

  mask.mode = sprite.mode;

  mask.size = Vec2(32.0F,32.0F);

  mask.position = sprite.position;
  TYRA_LOG("Sprite created!");
}

void Tutorial11::loadTexture() {
  /**
   * Renderer has high layer functions,
   * which allows to render:
   * - Sprite (2D)
   * - Mesh (3D)
   *
   * It uses ONLY low layer functions which are in renderer.core
   */
  auto& renderer = engine->renderer;

  /**
   * TextureRepository is a repository of textures.
   * It is a singleton class, with all game textures.
   * We are linking these textures with sprite's (2D) and mesh (3D) materials.
   */
  auto& textureRepository = renderer.getTextureRepository();

  /**
   * Texture is stored in "res" directory.
   * Content of "res" directory is automatically copied into
   * "bin" directory, which contains our final game.
   *
   * File utils automatically add's device prefix to the path,
   * based on current working directory.
   *
   * In PS2 world:
   * - USB has a "mass:" prefix
   * - Our PC in PS2Link has a "host:" prefix
   * - Our PC in PCSX2 has a "host:" prefix
   */
  auto filepath = FileUtils::fromCwd("slot.png");

  /**
   * Tyra supports following PNG formats:
   * 32bpp (RGBA)
   * 24bpp (RGB)
   * 8bpp, palletized (RGBA)
   * 4bpp, palletized (RGBA)
   *
   * 8bpp and 4bpp are the fastest.
   * All of these formats can be easily exported via GIMP.
   */
  auto* texture = textureRepository.add(filepath);

  /** Let's assign this texture to sprite. */
  texture->addLink(sprite.id);
  
  //texture->setWrapSettings(Clamp,Clamp);

  filepath = FileUtils::fromCwd("mask.png");

  auto* texture2 = textureRepository.add(filepath);

  texture2->addLink(mask.id);

  TYRA_LOG("Texture loaded!");
}

}  // namespace Tyra
