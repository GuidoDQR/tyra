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
#include "prueba.hpp"
#include <screenshot.h>
#include <malloc.h>
#include <gs_psm.h>

namespace Tyra {

Prueba::Prueba(Engine* t_engine) : engine(t_engine), pad(&t_engine->pad) {}

Prueba::~Prueba() {
  engine->renderer.getTextureRepository().freeBySprite(sprite);
  engine->renderer.getTextureRepository().freeBySprite(mask);
  engine->renderer.getTextureRepository().freeBySprite(spr_frameBuffer);
  engine->renderer.getTextureRepository().freeBySprite(spr_frameBufferBefore);
  delete pixel_char;
  delete pixel_frameBuffer_before;
}

void Prueba::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

  PngLoader loader;

  // memoria para poder guardar el frame buffer
  u32 size = loader.getTextureSize(512, 448, bpp32);

  unsigned char* pixels_ali = static_cast<unsigned char*>(memalign(
    128, size)); 

  //unsigned char* pixels_2 = static_cast<unsigned char*>(memalign(
  //  128, size));      

  pixel_char = new unsigned char[size]();
  pixel_char = pixels_ali;

  //pixel_frameBuffer_before = new unsigned char[size]();
  pixel_frameBuffer_before = pixels_ali;
  //printf("count textures: %d\n",engine->renderer.getTextureRepository().getTexturesCount());
  /** Sprite contains rectangle information. */
  loadSprite();

  /** Texture contains png image. */
  loadTexture();
  printf("count textures: %d\n",engine->renderer.getTextureRepository().getTexturesCount());

  //engine->renderer.core.renderer2D.setTextureMappingType(TyraNearest);

}

void Prueba::loop() {
  auto& renderer = engine->renderer;
  static bool pass = true;
  static bool pass2 = false;
  if(pad->getClicked().Cross){
    pass2 = !pass2;

    auto* texture = renderer.getTextureRepository().getBySpriteId(spr_frameBuffer.id);
    if(texture != nullptr){
      printf("la textura existe. Texture id: %d\n",texture->id);
      renderer.getTextureRepository().freeBySprite(spr_frameBuffer);
      printf("count textures: %d\n",renderer.getTextureRepository().getTexturesCount());
    }else{ printf("la textura NO existe\n"); }
    //if(pass2 == true){
    //  printf("FR: TRUE\n");
    //}else{printf("FR: false\n");}
    
  }else if(pad->getClicked().Circle){
    printf("count textures: %d\n",renderer.getTextureRepository().getTexturesCount());
    pass = !pass;
  }else if(pad->getClicked().Square){
    auto* texture = renderer.getTextureRepository().getBySpriteId(spr_frameBuffer.id);
    if(texture == nullptr){
      auto* texture2 = renderer.getTextureRepository().getBySpriteId(sprite.id);

      //auto filepath = FileUtils::fromCwd("slot.png");
      //auto* texture = renderer.getTextureRepository().add(filepath);
      //auto* texture3 = new Texture(*texture2);
      //Texture texture = *texture3;
      texture2->print();
      auto* data = texture2->getData();
      Texture* texture = new Texture(data); //&texture;
      delete data;
      renderer.getTextureRepository().add(texture);
      texture->addLink(spr_frameBuffer.id);
      printf("pase\n");
    }else{ printf("existe 1 textura borrala\n");}
    
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
  engine->renderer.core.renderer2D.setTextureMappingType(TyraNearest);
  createTexture();
  //renderer.renderer2D.render(mask);
  
  //if(pass == true){
    renderer.renderer2D.render(sprite);
  //}
  
  //renderer.renderer2D.render(punto);
  //renderMask(&sprite,&mask);
  static int i=0;
  renderer.renderer2D.render(spr_frameBuffer);
  //engine->renderer.core.renderer2D.setTextureMappingType(TyraLinear);
  //if(i>=1 && pass2 == true){renderer.renderer2D.render(spr_frameBuffer);}
  i++;

  renderer.endFrame();
}

void Prueba::loadSprite() {
  const auto& screenSettings = engine->renderer.core.getSettings();

  sprite.mode = SpriteMode::MODE_STRETCH;

  /** Let's scale it down */
  sprite.size = Vec2(32.0F, 32.0F);
  sprite.scale = 4;
  perfectPixelSize(&sprite);
  /** Set it in screen center */
  sprite.position = Vec2(0.0F,0.0F);

  //sprite.offset = Vec2(0.5F,0.5F);
  mask.mode = sprite.mode;

  mask.size = Vec2(0.0F,0.0F);

  mask.position = Vec2(32.0F,32.0F);//sprite.position;
  perfectPixelSize(&mask);

  punto.mode = MODE_STRETCH;
  punto.position = Vec2(127.0F,127.0F);
  punto.size = Vec2(0,0);
  TYRA_LOG("Sprite created!");
}

void Prueba::loadTexture() {
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

  //texture2->addLink(mask.id);
  texture2->addLink(mask.id);

  Texture* texFrameBuffer = textureRepository.add("slot.png");
  texFrameBuffer->addLink(spr_frameBuffer.id);

  //filepath = FileUtils::fromCwd("dot.png");
//
  //texture = textureRepository.add(filepath);
//
  //texture->addLink(punto.id);
  TYRA_LOG("Texture loaded!");
}

void Prueba::perfectPixelSize(Sprite* sprite){
  
  float sizeRequiredX =  sprite->size.x * sprite->scale; // 32 *1 = 32 //32 * 4 = 128
  float sizeRequiredY =  sprite->size.y * sprite->scale;

  sizeRequiredX -= 1.0f; // 127
  sizeRequiredY -= 1.0f; // 127
  
  sprite->size.x = sizeRequiredX / sprite->scale; // 31/1 = 31 // 127/4 = 31.75
  sprite->size.y = sizeRequiredY / sprite->scale;
}

void Prueba::createTexture(){
  auto& renderer = engine->renderer;
  auto* texture = renderer.getTextureRepository().getBySpriteId(spr_frameBuffer.id);
  
  if(texture != nullptr){
    printf("la textura existe. Texture id: %d\n",texture->id);

    auto buffer = renderer.core.texture.useTexture(texture);
    renderer.core.gs.vram.free(buffer.core->address);
    renderer.core.texture.unregisterAllocation(texture->id);
    renderer.getTextureRepository().free(texture); // borro la textura del framebuffer
    renderer.core.sync.waitAndClear();
    auto* textureSprite = renderer.getTextureRepository().getBySpriteId(sprite.id);

    //textureSprite->print();

    auto* data = textureSprite->getData();
    texture = new Texture(data);
    printf("texture bpp: %d\n",texture->core->bpp);
    delete data;
    renderer.getTextureRepository().add(texture);
    texture->addLink(spr_frameBuffer.id);
    //auto filepath = FileUtils::fromCwd("slot.png");
    //auto* texture2 = renderer.getTextureRepository().add(filepath);
    //texture2->addLink(spr_frameBuffer.id);
  }else{ printf("la textura NO existe\n"); }
  
  printf("count textures: %d\n",renderer.getTextureRepository().getTexturesCount());
}
}  // namespace Tyra
