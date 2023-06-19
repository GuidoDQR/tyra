/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

/*advertencia puede que no funcione y sea peligroso*/
#include <tyra>
#include "prueba3.hpp"
#include <screenshot.h>
#include <malloc.h>
#include <gs_psm.h>
#include <draw_buffers.h>

namespace Tyra {

Prueba::Prueba(Engine* t_engine) : engine(t_engine), pad(&t_engine->pad) {}

Prueba::~Prueba() {
  engine->renderer.getTextureRepository().freeBySprite(sprite);
  engine->renderer.getTextureRepository().freeBySprite(mask);
  engine->renderer.getTextureRepository().freeBySprite(spr_frameBuffer);
  engine->renderer.getTextureRepository().freeBySprite(spr_frameBufferBefore);
  engine->renderer.getTextureRepository().freeBySprite(punto);
  delete pixel_fb;
  delete pixel_frameBuffer_before;
  delete pixel_fb_after;
}

void Prueba::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

  drawFB = false;
  PngLoader loader;

  // memoria para poder guardar el frame buffer
  u32 size = loader.getTextureSize(512, 448, bpp32);

  unsigned char* pixels_ali = static_cast<unsigned char*>(memalign(
    128, size)); 

  unsigned char* pixels_2 = static_cast<unsigned char*>(memalign(
    128, size));    

  unsigned char* pixels_3 = static_cast<unsigned char*>(memalign(
    128, size));       

  pixel_fb = new unsigned char[size]();
  pixel_fb = pixels_ali;

  pixel_frameBuffer_before = new unsigned char[size]();
  pixel_frameBuffer_before = pixels_2;
  
  pixel_fb_after = new unsigned char[size]();
  pixel_fb_after = pixels_3;
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

    //auto* texture = renderer.getTextureRepository().getBySpriteId(spr_frameBuffer.id);
    //if(texture != nullptr){
    //  printf("la textura existe. Texture id: %d\n",texture->id);
    //  renderer.getTextureRepository().freeBySprite(spr_frameBuffer);
    //  printf("count textures: %d\n",renderer.getTextureRepository().getTexturesCount());
    //}else{ printf("la textura NO existe\n"); }
    if(pass2 == true){
      printf("FR: TRUE\n");
    }else{printf("FR: false\n");}
    
  }else if(pad->getClicked().Circle){
    //printf("count textures: %d\n",renderer.getTextureRepository().getTexturesCount());
    pass = !pass;
  }else if(pad->getClicked().Square){
    // auto* texture = renderer.getTextureRepository().getBySpriteId(spr_frameBuffer.id);
    // if(texture == nullptr){
      // auto* texture2 = renderer.getTextureRepository().getBySpriteId(sprite.id);

      // //auto filepath = FileUtils::fromCwd("slot.png");
      // //auto* texture = renderer.getTextureRepository().add(filepath);
      // //auto* texture3 = new Texture(*texture2);
      // //Texture texture = *texture3;
      // texture2->print();
      // auto* data = texture2->getData();
      // Texture* texture = new Texture(data); //&texture;
      // delete data;
      // renderer.getTextureRepository().add(texture);
      // texture->addLink(spr_frameBuffer.id);
      // printf("pase\n");
    // }else{ printf("existe 1 textura borrala\n");}
    
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
  
  if(pass2 == false){
  renderer.renderer2D.render(mask);
  }

  
  if(pass == true){
    renderer.renderer2D.render(sprite);
  }
  //printf("gola\n");
  //createTexture();
  //renderer.renderer2D.render(punto);
  
  static int i=0;
  if(i==1){
    auto* texture = renderer.getTextureRepository().getBySpriteId(sprite.id);
    auto texbuffer = renderer.core.texture.useTexture(texture);
    printf("Ya paso\n");
    printf("tex Width: %d\n",texture->getWidth());
    printf("tex Height: %d\n",texture->getHeight());
    printf("tex info Width: %d\n",texbuffer.core->info.width);
    printf("tex info Height: %d\n\n",texbuffer.core->info.height);
  }
  if(i==0){
    auto* texture = renderer.getTextureRepository().getBySpriteId(sprite.id);
    printf("tex Width: %d\n",texture->getWidth());
    printf("tex Height: %d\n",texture->getHeight());
    texture->core->width = 128;
    texture->core->height = 128;
    printf("tex Width: %d\n",texture->getWidth());
    printf("tex Height: %d\n",texture->getHeight());

    auto texbuffer = renderer.core.texture.useTexture(texture);
    texbuffer.core->width = texture->getWidth();

    PngLoader loader;
    
    unsigned char* pixel_texture = static_cast<unsigned char*>(memalign(
    128, loader.getTextureSize(128, 128, bpp32))); 

    ps2_screenshot(pixel_fb,0,0,0,512,448,GS_PSM_32);

    struct PngPixel4* pngPixel= (struct PngPixel4*) pixel_fb;
    struct PngPixel4* pngPixelTexture  = (struct PngPixel4*) pixel_texture;

    //obtener los pixeles del framebuffer
    int counter = 0;
    int counterX = 0;
    int counterY = 0;
    int initialPos = (sprite.position.y*512)+sprite.position.x;
    for(int j=initialPos;j<512*448;j++){
      if(counterX == 128){ j+=(512-128); counterX=0; counterY++;}
      if(counterY == 128){ break;}
      //printf("pixel[%d][%d]: %d,%d,%d,%d\n",counterY,counterX,pngPixel[j].r,pngPixel[j].g,pngPixel[j].b,pngPixel[j].a);
      if(counterY == 31){
        pngPixel[j].r = 255;
        pngPixel[j].g = 0;
        pngPixel[j].b = 0;
      }
      pngPixelTexture[counter] = pngPixel[j];
      counter++;
      counterX++;
    }
    texture->core->data = pixel_texture;
    //renderer.core.gs.vram.free()
    //auto address = gs->vram.allocate(*core);
    //TYRA_LOG("address:",address);
    //TYRA_ASSERT(address > 0, "Texture buffer allocation error, no memory!");
    //result->address = address;
    printf("tex info Width: %d\n",texbuffer.core->info.width);
    printf("tex info Height: %d\n\n",texbuffer.core->info.height);
    texbuffer.core->info.width = draw_log2(texture->getWidth());
    texbuffer.core->info.height = draw_log2(texture->getHeight());
    printf("tex info Width: %d\n",texbuffer.core->info.width);
    printf("tex info Height: %d\n\n",texbuffer.core->info.height);
    sprite.size = Vec2(127,127);
    sprite.scale = 1;
    Path3* path = renderer.core.getPath3();
    path->sendTexture(texture,texbuffer);
  }
  renderMask(&sprite,&mask);
  //renderer.renderer2D.render(spr_frameBuffer);
  //engine->renderer.core.renderer2D.setTextureMappingType(TyraLinear);
  // if(/*i>=1 &&*/ pass2 == true){
  //   drawFB=true;  
  //   renderer.renderer2D.render(spr_frameBuffer);   
  // }
  i++;

  renderer.endFrame();
}

void Prueba::loadSprite() {
  //const auto& screenSettings = engine->renderer.core.getSettings();

  sprite.mode = SpriteMode::MODE_STRETCH;

  /** Let's scale it down */
  sprite.size = Vec2(32.0F, 32.0F);
  sprite.scale = 4;
  perfectPixelSize(&sprite);
  /** Set it in screen center */
  sprite.position = Vec2(10.0F,10.0F);

  //sprite.offset = Vec2(0.5F,0.5F);
  mask.mode = sprite.mode;

  mask.size = Vec2(32.0F,32.0F);

  mask.position = Vec2(120.0F,32.0F);//sprite.position;
  perfectPixelSize(&mask);
  spr_frameBuffer.mode = SpriteMode::MODE_STRETCH;
  spr_frameBuffer.position = Vec2(0,0);
  spr_frameBuffer.size = Vec2(511,511);//Vec2((sprite.size.x*sprite.scale),(sprite.size.y*sprite.scale));

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

  auto* data = texture->getData(); // obtengo los datos del sprite
  Texture* texFrameBuffer =  new Texture(data);
  //auto* texture2 = renderer.getTextureRepository().getBySpriteId(sprite.id);    
  delete data;
  textureRepository.add(texFrameBuffer);
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
  // obtengo la textura del framebuffer
  auto* texture = renderer.getTextureRepository().getBySpriteId(spr_frameBuffer.id);
  //if(texture == nullptr){
  //  printf("texture no existe\n");
  //}else{
  //  printf("texture existe\n");
  //}
  int sizeX = (sprite.size.x*sprite.scale);
  int sizeY = (sprite.size.y*sprite.scale);
  //printf("texture width: %d\n", texture->core->width);
  //printf("texture height: %d\n", texture->core->height);
  if(sizeX>32){
    sizeX = 512;
  }
  if(sizeY>32){
    sizeY = 512;
  }
  if(texture != nullptr && (sizeX != texture->core->width || sizeY != texture->core->height) && drawFB == true){
    drawFB = false;
    printf("la textura existe. Texture id: %d\n",texture->id);
    renderer.getTextureRepository().free(texture);

    auto* textureSprite = renderer.getTextureRepository().getBySpriteId(sprite.id);
    auto* data = textureSprite->getData(); // obtengo los datos del sprite
    texture =  new Texture(data); 
    delete data;
    renderer.getTextureRepository().add(texture);
    texture->addLink(spr_frameBuffer.id);
  }else if (texture != nullptr){
    auto buffer = renderer.core.texture.useTexture(texture);
    renderer.core.texture.unregisterAllocation(texture->id);   
    renderer.core.gs.vram.free(buffer.core->address);
    printf("pase\n");
  }
  //auto buffer = renderer.core.texture.useTexture(texture);
  //if()

  //cambio el tamaño por los pixeles del sprite mostrado en pantalla
  

  //printf("texture width: %d\n", texture->core->width);
  //printf("texture height: %d\n", texture->core->height);
  texture->core->width  = sizeX;
  texture->core->height = sizeY;

  PngLoader loader;

  // memoria para poder guardar el frame buffer

  //unsigned char* pixel_texture = static_cast<unsigned char*>(memalign(
  //  128, loader.getTextureSize(sizeX, sizeY, bpp32))); 

  /*unsigned char* pixel_fb = static_cast<unsigned char*>(memalign(
    128, loader.getTextureSize(512,448,bpp32))); */

  //pixel_fb = pixels_ali;  
  //delete pixels_ali;
  
  ps2_screenshot(pixel_frameBuffer_before,0,0,0,512,448,GS_PSM_32);
  renderer.renderer2D.render(mask);
  ps2_screenshot(pixel_fb,0,0,0,512,448,GS_PSM_32);
  renderer.renderer2D.render(sprite);
  ps2_screenshot(pixel_fb_after,0,0,0,512,448,GS_PSM_32);
  //printf("fps: %d\n",engine->info.getFps());
  int counterX=0;
  int counterY=0;
  int counter =0;

  
  struct PngPixel4* pngPixel_before= (struct PngPixel4*) pixel_frameBuffer_before;
  struct PngPixel4* pngPixel= (struct PngPixel4*) pixel_fb;
  struct PngPixel4* pngPixel_after= (struct PngPixel4*) pixel_fb_after;
  //printf("pixelBF[0]: %d,%d,%d,%d\n",pngPixel_before[0].r,pngPixel_before[0].g,pngPixel_before[0].b,pngPixel_before[0].a);
  //printf("pixel[0]: %d,%d,%d,%d\n",pngPixel[0].r,pngPixel[0].g,pngPixel[0].b,pngPixel[0].a);
  //struct PngPixel4* pngPixelTexture  = (struct PngPixel4*) pixel_texture;

  for(int i=0;i<512*448;i++){
   if(counterX==texture->core->width){ i+=(512-texture->core->width); counterX=0;counterY++;}
   if(counterY==texture->core->height){ break;}
   //if(counterX%2==0){
   //  pngPixelTexture[i].r=255;
   //  pngPixelTexture[i].g=0;
   //  pngPixelTexture[i].b=0;
   //}
   //pngPixelTexture[counter] = pngPixel[i];
   if(!(pngPixel[i].r == 255 && pngPixel[i].g == 255 && pngPixel[i].b == 255)){
    pngPixel_before[i] = pngPixel_after[i];
   }
   counterX++;
   counter++;
  }
  //printf("pixel[0]: %d,%d,%d,%d\n",pngPixel[0].r,pngPixel[0].g,pngPixel[0].b,pngPixel[0].a);
  texture->core->data = pixel_frameBuffer_before;
  //delete pixel_frameBuffer_before;
  //delete pixel_fb;
  // auto& renderer = engine->renderer;
  // auto* texture = renderer.getTextureRepository().getBySpriteId(spr_frameBuffer.id);
  
  // if(texture != nullptr){
    // printf("la textura existe. Texture id: %d\n",texture->id);

    // auto buffer = renderer.core.texture.useTexture(texture);
    // renderer.core.gs.vram.free(buffer.core->address);
    // renderer.core.texture.unregisterAllocation(texture->id);
    // renderer.getTextureRepository().free(texture); // borro la textura del framebuffer
    // renderer.core.sync.waitAndClear();
    // auto* textureSprite = renderer.getTextureRepository().getBySpriteId(sprite.id);

    // //textureSprite->print();

    // auto* data = textureSprite->getData();
    // texture = new Texture(data);
    // printf("texture bpp: %d\n",texture->core->bpp);
    // delete data;
    // renderer.getTextureRepository().add(texture);
    // texture->addLink(spr_frameBuffer.id);
    // //auto filepath = FileUtils::fromCwd("slot.png");
    // //auto* texture2 = renderer.getTextureRepository().add(filepath);
    // //texture2->addLink(spr_frameBuffer.id);
  // }else{ printf("la textura NO existe\n"); }
  
  // printf("count textures: %d\n",renderer.getTextureRepository().getTexturesCount());
}

int Prueba::renderMask(Sprite* sprite, Sprite* mask){
  Vec2 maskPos1 = mask->position; // 32,32
  Vec2 maskPos2 = Vec2((mask->size.x * mask->scale)+ maskPos1.x, (mask->size.y * mask->scale)+ maskPos1.y); // 63,63
  Vec2 sprPos1  = sprite->position; // 10,10
  Vec2 sprPos2  = Vec2((sprite->size.x * sprite->scale)+ sprPos1.x, (sprite->size.y * sprite->scale)+ sprPos1.y); // 137,137

  // verifica que el mask este adentro del sprite
  if(maskPos1.x < sprPos2.x && maskPos2.x > sprPos1.x &&
     maskPos1.y < sprPos2.y && maskPos2.y > sprPos1.y){
     //printf("colission\n");
  }else{ /*printf("no colission\n");*/ return 1;}

  auto* tex = engine->renderer.getTextureRepository().getBySpriteId(sprite->id);
  auto* texMask = engine->renderer.getTextureRepository().getBySpriteId(mask->id);  
  auto buffer = engine->renderer.core.texture.useTexture(tex);

  struct PngPixel4* pngSprite= (struct PngPixel4*) tex->core->data;
  struct PngPixel4* pngMask  = (struct PngPixel4*) texMask->core->data;

  const int pixelX = tex->getWidth();  // ancho de la textura del sprite
  const int pixelY = tex->getHeight(); // alto  de la textura del sprite

  Vec2 pixelSprPos;
  
  // coordenadas en base a la posicion del sprite 
  // Significa que el valor minimo debe ser 0,0 
  // Y el valor maximo 127,127 incluidos o menor 128,128 
  // realmente lo que importa es la posicion inicial

  Vec2 maskInSprite1; // Posicion inicial donde aparece el mask en el sprite
  Vec2 maskInSprite2; // Posicion final donde aparece el mask en el sprite

  maskInSprite1.x = maskPos1.x - sprPos1.x; // 32-10 = 22
  maskInSprite1.y = maskPos1.y - sprPos1.y; // 32-10 = 22

  maskInSprite2.x = maskPos2.x - sprPos1.x; // 63-10 = 53 // 152-10 = 142
  maskInSprite2.y = maskPos2.y - sprPos1.y; // 63-10 = 53 //

  // coordenadas de los pixeles del mask

  Vec2 pixelMaskPos;
  Vec2 countPixel;
  
  pixelMaskPos = Vec2(0,0);   
  countPixel = Vec2(texMask->getWidth(),texMask->getHeight()); 

  // esto son valores para el mask

  // si la posicion inicial Y del mask es menor a la del sprite 

  if(maskInSprite1.y < 0){
    pixelMaskPos.y -= maskInSprite1.y; 
    countPixel.y += maskInSprite1.y;  
  }

  // si la posicion inicial Y del mask es mayor a la del sprite

  if(maskInSprite2.y > pixelY){
    countPixel.y = pixelY - maskInSprite1.y;
  }

  // si la posicion inicial X del mask es menor a la del sprite
  
  if(maskInSprite1.x < 0){
    pixelMaskPos.x -= maskInSprite1.x; 
    //countPixel.x += maskInSprite1.x;  
  }
  
  // si la posicion inicial X del mask es mayor a la del sprite

  if(maskInSprite2.x > pixelX){
    //pixelMaskPos.x -= maskInSprite2.x; 
    countPixel.x = pixelX - maskInSprite1.x;
  }

  //printf("mask pos: %f,%f\n",maskPos1.x,maskPos1.y);
  //printf("pixelmask: %f,%f\n",pixelMaskPos.x,pixelMaskPos.y);
  //printf("countPixel: %f,%f\n",countPixel.x,countPixel.y);

  // printf("mask pos: %f,%f\n",maskPos1.x,maskPos1.y);
  // printf("mask1: %f,%f\n",maskInSprite1.x,maskInSprite1.y);
  // printf("mask2: %f,%f\n",maskInSprite2.x,maskInSprite2.y);

  // esto son valores para el mask en base a la posicion del sprite(0,0)

  // si la parte de arriba del mask no esta adentro del sprite
  // a la posicion final Y del mask se le resta los pixeles que no aparecen 
  // comienza desde el pixel 0 en el eje Y del sprite
 
  if(maskInSprite1.y < 0){
    //maskInSprite2.y = (texMask->getHeight()-1) + maskInSprite1.y; 
    //maskInSprite2.y -= maskInSprite1.y;
    maskInSprite1.y = 0; 
  }

  // si la parte de abajo del mask no esta adentro del sprite
  // la posicion final Y del mask 
  // es la resta de la posicion final Y del sprite con
  // la posicion inical Y del mask

  if(maskInSprite2.y > pixelY){
    //maskInSprite2.y = sprPos2.y - maskInSprite1.y;
    maskInSprite2.y = pixelY;
  }

  // si la parte de izquierda del mask no esta adentro del sprite
  // a la posicion final X del mask se le resta los pixeles que no aparecen 
  // comienza desde el pixel 0 en el eje X del sprite

  if(maskInSprite1.x < 0){
    //maskInSprite2.x = (texMask->getWidth()-1) + maskInSprite1.x;
    //maskInSprite2.x -= maskInSprite1.x;
    maskInSprite1.x = 0;
  }

  // si la parte de derecha del mask no esta adentro del sprite
  // la posicion final X del mask 
  // es la resta de la posicion final x del sprite con
  // la posicion inical x del mask

  if(maskInSprite2.x > pixelX){
    //maskInSprite2.x = sprPos2.x - maskInSprite1.x;
    maskInSprite2.x = pixelX-1;
  }
  // printf("mask pos: %f,%f\n",maskPos1.x,maskPos1.y);
  // printf("mask1: %f,%f\n",maskInSprite1.x,maskInSprite1.y);
  // printf("mask2: %f,%f\n",maskInSprite2.x,maskInSprite2.y);

  // // si la parte de arriba del mask no esta adentro del sprite
  // // a la posicion final Y del mask se le resta los pixeles que no aparecen 
  // // comienza desde el pixel 0 en el eje Y del sprite
 
  // if(maskInSprite1.y < 0){
  //   //maskInSprite2.y = (texMask->getHeight()-1) + maskInSprite1.y; 
  //   maskInSprite2.y -= maskInSprite1.y;
  //   maskInSprite1.y *= -1; 
  // }

  // // si la parte de abajo del mask no esta adentro del sprite
  // // la posicion final Y del mask 
  // // es la resta de la posicion final Y del sprite con
  // // la posicion inical Y del mask

  // if(maskInSprite2.y > sprPos2.y){
  //   //maskInSprite2.y = sprPos2.y - maskInSprite1.y;
  //   maskInSprite2.y = sprPos2.y;
  // }

  // // si la parte de izquierda del mask no esta adentro del sprite
  // // a la posicion final X del mask se le resta los pixeles que no aparecen 
  // // comienza desde el pixel 0 en el eje X del sprite

  // if(maskInSprite1.x < 0){
  //   //maskInSprite2.x = (texMask->getWidth()-1) + maskInSprite1.x;
  //   maskInSprite2.x -= maskInSprite1.x;
  //   maskInSprite1.x *= -1;
  // }

  // // si la parte de derecha del mask no esta adentro del sprite
  // // la posicion final X del mask 
  // // es la resta de la posicion final x del sprite con
  // // la posicion inical x del mask

  // if(maskInSprite2.x > sprPos2.x){
  //   //maskInSprite2.x = sprPos2.x - maskInSprite1.x;
  //   maskInSprite2.x = sprPos2.x;
  // }
  // // printf("mask pos: %f,%f\n",maskPos1.x,maskPos1.y);
  // // printf("mask1: %f,%f\n",maskInSprite1.x,maskInSprite1.y);
  // // printf("mask2: %f,%f\n",maskInSprite2.x,maskInSprite2.y);


  int pixelMax = pixelX * pixelY;

  // es la posicion del pixel del sprite
  // en donde estara en el bucle
  int initialPixel = (maskInSprite1.y * pixelX) + maskInSprite1.x;

  // es la posicion del pixel del mask
  // en donde estara en el bucle
  int counter  = (pixelMaskPos.y*texMask->getWidth()/*countPixel.x*/)+pixelMaskPos.x;

  // counterX y counter Y 
  // cuentan la posicion del pixel en ejes X,Y donde estan
  // son los pixeles maximos por los que va a pasar
  // en base al texture mask
  // el valor no es mayor al pixel del texture mask
  int counterX = pixelMaskPos.x;
  int counterY = 0;

  // i: va a pasar por los pixeles del sprite
  // counter: va a pasar por los 32x32 pixeles del mask
  // pixeles del sprite 128x128
  //   0,1,2,....,127
  // 128,129,130,....
  // pixeles del mask 32x32
  //   0,1,2,....,31
  // 32,33,34,....
  static int k=0;

  printf("mask pos: %f,%f\n",maskPos1.x,maskPos1.y);
  printf("mask1: %f,%f\n",maskInSprite1.x,maskInSprite1.y);
  //printf("mask2: %f,%f\n",maskInSprite2.x,maskInSprite2.y);
  printf("pixelmask: %f,%f\n",pixelMaskPos.x,pixelMaskPos.y);
  printf("countPixel: %f,%f\n",countPixel.x,countPixel.y);

  for(int i=0;i<pixelMax;i++){
    pngSprite[i].a = 128;
  }

  // tiene que pasar por el pixel del sprite y del mask
  for(int i=initialPixel; i < pixelMax; i++){
    if(counterX == countPixel.x){
      i += (pixelX-countPixel.x+pixelMaskPos.x);
      counterX = pixelMaskPos.x;//0;//= maskInSprite1.x;
      //counter += pixelMaskPos.x; //+ countPixel.x;
      counter += pixelMaskPos.x + (texMask->getWidth()-countPixel.x);
      counterY++;
    }
    
    if(counterY == countPixel.y){break;}

    if(k==0){
      printf("pixelMask[%d][%d]: %d=pixelSprite[%d]\n",counterY,counterX,counter,i);
      //printf("pixel[%d][%d]:%d: %d,%d,%d,%d\n",counterY,counterX,counter,pngMask[counter].r,pngMask[counter].g,pngMask[counter].b,pngMask[counter].a);
    }
    if(pngMask[counter].r == 255 && pngMask[counter].g == 255 &&
       pngMask[counter].b == 255 ){
        pngSprite[i].a = 0;
    }else{ pngSprite[i].a = 128; }

    counter++;
    counterX++;    
  }
  k++;
  Path3* path = engine->renderer.core.getPath3();
  path->sendTexture(tex,buffer);
  return 0;
}
}  // namespace Tyra
