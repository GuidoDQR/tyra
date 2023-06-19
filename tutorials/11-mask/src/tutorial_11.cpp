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
#include <screenshot.h>
#include <malloc.h>
#include <gs_psm.h>

namespace Tyra {

Tutorial11::Tutorial11(Engine* t_engine) : engine(t_engine), pad(&t_engine->pad) {}

Tutorial11::~Tutorial11() {
  engine->renderer.getTextureRepository().freeBySprite(sprite);
  engine->renderer.getTextureRepository().freeBySprite(mask);
  engine->renderer.getTextureRepository().freeBySprite(spr_frameBuffer);
  engine->renderer.getTextureRepository().freeBySprite(spr_frameBufferBefore);
  delete pixel_char;
  delete pixel_frameBuffer_before;
}

void Tutorial11::init() {
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

void Tutorial11::loop() {
  auto& renderer = engine->renderer;
  static bool pass = true;
  static bool pass2 = false;
  if(pad->getClicked().Cross){
    pass2 = !pass2;
    if(pass2 == true){
      printf("FR: TRUE\n");
    }else{printf("FR: false\n");}
    
    //engine->renderer.core.renderer2D.setTextureMappingType(TyraNearest);
  }else if(pad->getClicked().Circle){
    pass = !pass;
    //engine->renderer.core.renderer2D.setTextureMappingType(TyraLinear);
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

  //renderer.renderer2D.render(mask);
  
  //if(pass == true){
  //  renderer.renderer2D.render(sprite);
  //}
  
  //renderer.renderer2D.render(punto);
  renderMask(&sprite,&mask);
  static int i=0;
  
  //engine->renderer.core.renderer2D.setTextureMappingType(TyraLinear);
  if(i>=1 && pass2 == true){renderer.renderer2D.render(spr_frameBuffer);}
  i++;

  // if(i==1){
    
  //   PngLoader loader;
    

  //   int counterX=0;
  //   int counterY=0;
  //   int counter =0;
  //   int sizeX = ((sprite.size.x)*sprite.scale)+1; // 128
  //   int sizeY = ((sprite.size.y)*sprite.scale)+1; // 128

  //   // data of new texture
  //   unsigned char* new_pixel = static_cast<unsigned char*>(memalign(
  //     128, loader.getTextureSize(sizeX, sizeY, bpp32)));    //guarda 128 pixeles. [0][0] hasta [127][127]

  //   Texture* texture = renderer.getTextureRepository().getBySpriteId(sprite.id);
  //   auto buffer = renderer.core.texture.useTexture(texture);
  //   int width = engine->renderer.core.getSettings().getWidth();
  //   int height = engine->renderer.core.getSettings().getHeight();
    
  //   //auto* data = loader.load("slot.png");
  //   //TextureBuilderData* data = new TextureBuilderData();
  //   // data->name = texture->name;
  //   // data->width = texture->getWidth();
  //   // data->height = texture->getHeight();
  //   // data->bpp = texture->core->bpp;
  //   // data->gsComponents = texture->core->components;

  //   // data->clutWidth = texture->clut->width;
  //   // data->clutHeight = texture->clut->height;
  //   // data->clut = texture->clut->data;
  //   // data->clutBpp = texture->clut->bpp;
  //   // data->clutGsComponents = texture->clut->components;
  //   //Texture* texFrameBuffer = new Texture(data);
  //   //delete data;
  //   auto& textureRepository = renderer.getTextureRepository();
  //   auto* texFrameBuffer = textureRepository.add("slot.png");
  //   texFrameBuffer->addLink(spr_frameBuffer.id);
  //   printf("tex id: %d\n",texture->id);
  //   printf("texFB id: %d\n",texFrameBuffer->id);
  //   //texFrameBuffer->id = rand() % 1000000;
  //   printf("tex id: %d\n",texture->id);
  //   printf("texFB id: %d\n",texFrameBuffer->id);
    
  //   texFrameBuffer->core->width = sizeX;
  //   texFrameBuffer->core->height = sizeY;

    
  //   //sprite frame buffer
  //   spr_frameBuffer.mode = SpriteMode::MODE_STRETCH;

  //   spr_frameBuffer.position = Vec2(0.0F,0.0F);
  //   spr_frameBuffer.size = Vec2(127.0F,127.0F); // NO CAMBIAR

  //   // Texture Buffer
  //   //ps2_screenshot(pixel_char,buffer.core->address/64,0,0,512,448,GS_PSM_32);
  //   //ps2_screenshot_file("datos.txt",0,512,448,GS_PSM_32);
  //   // Frame Buffer
  //   ps2_screenshot(pixel_char,0,0,0,512,488,GS_PSM_32);

  //   struct PngPixel4* pixel = (struct PngPixel4*) pixel_char;
    
  //   //printf("width: %d,%d\n",texture->core->width,texture->core->height);
    
  //   struct PngPixel4* new_pixelpng = (struct PngPixel4*) new_pixel;

  //   //printf("spr_buffer size: %f,%f\n",spr_frameBuffer.size.x,spr_frameBuffer.size.y);

  //   //printf("texture size: %d,%d\n",texture->core->width,texture->core->height);
  //   //printf("texture frameBuffer size: %d,%d\n",texFrameBuffer->core->width,texFrameBuffer->core->height);
    
  //   for(int j=0;j<512*448;j++){
  //     if(counterX == sizeX){ j+=(512-(sizeX)); counterX = 0;counterY++;}
  //     if(counterY == sizeY){ break;}
      
  //     //if(counterX == sizeX-1){
  //     //  //printf("pase[%d][%d]: %d\n",counterY,counterX,j);
  //     //  if(counterY % 2 == 0){
  //     //    //pixel[j].r = 255;
  //     //    //pixel[j].g = 0;
  //     //    //pixel[j].b = 0;
  //     //    //pixel[j].a = 0;
  //     //  }
  //     // 
  //     //} 

  //     //if(counterX > sizeX-2 && counterX <= sizeX-1){
  //       //pixel[j].r = 255;
  //       //pixel[j].g = 0;
  //       //pixel[j].b = 0;
  //       //printf("size-1:%d\n",sizeX-1);
  //       //printf("    pixel[%d][%d]:%d: %d,%d,%d,%d\n",counterY,counterX,j,pixel[j].r,pixel[j].g,pixel[j].b,pixel[j].a);
  //     //}

  //     new_pixelpng[counter] = pixel[j];
     
  //     counter++;
  //     counterX++;
  //   }
  //   texFrameBuffer->core->data = new_pixel;
  // }
  /** End frame will perform vsync. */
  renderer.endFrame();
}

void Tutorial11::loadSprite() {
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

void Tutorial11::perfectPixelSize(Sprite* sprite){
  
  float sizeRequiredX =  sprite->size.x * sprite->scale; // 32 *1 = 32 //32 * 4 = 128
  float sizeRequiredY =  sprite->size.y * sprite->scale;

  sizeRequiredX -= 1.0f; // 127
  sizeRequiredY -= 1.0f; // 127
  
  sprite->size.x = sizeRequiredX / sprite->scale; // 31/1 = 31 // 127/4 = 31.75
  sprite->size.y = sizeRequiredY / sprite->scale;
}

void Tutorial11::renderMask(Sprite* sprite, Sprite* mask){
  unsigned char* new_pixel = nullptr;
  createFrameTexture(new_pixel,sprite->size,sprite->scale);
  //printf("sizeof: %d\n",sizeof(new_pixel));
  engine->renderer.renderer2D.render(mask);
  //printf("pase 1\n");
  // Obtengo los pixeles antes de que se dibuje el sprite
  getFrameBuffer(pixel_frameBuffer_before);
  //printf("pase 2\n");
  // Dibujo el sprite
  engine->renderer.renderer2D.render(sprite);
  //printf("pase 3\n");
  // Obtengo los pixeles cuando se dibujo el sprite
  getFrameBuffer(pixel_char);
  //printf("pase 4\n");
  // Verifico si existe una textura para el framebuffer
  //existFrameTexture(&spr_frameBuffer);
  
  //printf("pase 5\n");
  
  //printf("pase 6\n");
  // obtengo la textura del sprite  
  auto& textureRepository = engine->renderer.getTextureRepository();

  Texture* texture = textureRepository.getBySpriteId(sprite->id);
  //printf("pase 7\n");
  // obtengo el buffer texture del sprite
  //auto buffer = engine->renderer.core.texture.useTexture(texture);
  //int width = engine->renderer.core.getSettings().getWidth();
  //int height = engine->renderer.core.getSettings().getHeight();
  
  // creo una textura para el framebuffer
  //Texture* texFrameBuffer = textureRepository.add("slot.png");
  //texFrameBuffer->addLink(spr_frameBuffer.id);

  // Obtengo la textura del sprite del framebuffer
  Texture* texFrameBuffer = textureRepository.getBySpriteId(spr_frameBuffer.id);
  //printf("pase 8\n");
  // Obtengo el texture buffer del sprite del framebuffer
  RendererCoreTextureBuffers buffer =  engine->renderer.core.texture.useTexture(texFrameBuffer);
  //printf("pase 9\n");
  // printf("count textures: %d\n",textureRepository.getTexturesCount());
  // std::vector<Texture*>* all = textureRepository.getAll();
  // for(int i=0; i< textureRepository.getTexturesCount();i++){
    // printf("texture[%d] id: %d | %s\n",i,all->at(i)->id,all->at(i)->name.c_str());
    // printf("linked with: %d\n", all->at(i)->isLinkedWith(spr_frameBuffer.id));
  // }
  // printf("\n");
  //printf("tex id: %d\n",texture->id);
  //printf("texFB id: %d\n",texFrameBuffer->id);
  //texFrameBuffer->id = rand() % 1000000;
  //printf("tex id: %d\n",texture->id);
  //printf("texFB id: %d\n",texFrameBuffer->id);
  
  int sizeX = ((sprite->size.x)*sprite->scale)+1;;
  int sizeY = ((sprite->size.y)*sprite->scale)+1;

  texFrameBuffer->core->width = sizeX;
  texFrameBuffer->core->height = sizeY;
  
  //sprite frame buffer
  spr_frameBuffer.mode = SpriteMode::MODE_STRETCH;
  spr_frameBuffer.position = Vec2(sprite->position.x,sprite->position.y);
  spr_frameBuffer.size = Vec2(sizeX-1,sizeY-1); // NO CAMBIAR

  // Texture Buffer
  //ps2_screenshot(pixel_char,buffer.core->address/64,0,0,512,448,GS_PSM_32);
  // Frame Buffer
  //ps2_screenshot(pixel_char,0,0,0,512,488,GS_PSM_32);

  // pixeles del frame buffer con el sprite dibujado
  struct PngPixel4* pixel_before_png = (struct PngPixel4*) pixel_frameBuffer_before;

  // pixeles del frame buffer con el sprite dibujado
  struct PngPixel4* pixel = (struct PngPixel4*) pixel_char;
  
  //printf("width: %d,%d\n",texture->core->width,texture->core->height);
  
  struct PngPixel4* new_pixelpng = (struct PngPixel4*) new_pixel;
  
  //printf("spr_buffer size: %f,%f\n",spr_frameBuffer.size.x,spr_frameBuffer.size.y);
  //printf("texture size: %d,%d\n",texture->core->width,texture->core->height);
  //printf("texture frameBuffer size: %d,%d\n",texFrameBuffer->core->width,texFrameBuffer->core->height);
  
  int counterX = 0;
  int counterY = 0;
  int counter  = 0;
  //printf("pase 10\n");
  for(int j=0;j<512*448;j++){
    if(counterX == sizeX){ j+=(512-(sizeX)); counterX = 0;counterY++;}
    if(counterY == sizeY){ break;}

    //printf("pixelbefore[%d]: %d,%d,%d,%d\n",j,pixel_before_png[j].r,pixel_before_png[j].g,pixel_before_png[j].b,pixel_before_png[j].a);
    
    // si el pixel es blanco por el color del mask 
    // entonces guarda el color del frame buffer anterior
    if(pixel_before_png[j].r == 255 &&
       pixel_before_png[j].g == 255 &&
       pixel_before_png[j].b == 255
       ){
        //printf("pase 11\n");
      pixel[j] = pixel_before_png[j];
      //printf("pase 12\n");
    }
    //printf("      pixel[%d]: %d,%d,%d,%d\n",j,pixel[j].r,pixel[j].g,pixel[j].b,pixel[j].a);
    new_pixelpng[counter] = pixel[j];
   
    counter++;
    counterX++;
  }
  //printf("pase 13\n");
  texFrameBuffer->core->data = new_pixel;
  //printf("pase 14\n");
  buffer.core->width = texFrameBuffer->getWidth();
  buffer.core->address = engine->renderer.core.gs.vram.allocate(*texFrameBuffer->core);
  buffer.core->info.width = draw_log2(texFrameBuffer->getWidth());
  buffer.core->info.height = draw_log2(texFrameBuffer->getHeight());
  
  // result->width = t_texture->getWidth();
  // result->psm = core->psm;
  // result->info.components = core->components;

  // auto address = gs->vram.allocate(*core);
  // TYRA_ASSERT(address > 0, "Texture buffer allocation error, no memory!");
  // result->address = address;

  // result->info.width = draw_log2(t_texture->getWidth());
  // result->info.height = draw_log2(t_texture->getHeight());
  // result->info.function = TEXTURE_FUNCTION_MODULATE;
  
  //Path3* path = engine->renderer.core.getPath3();
  //path->sendTexture(texFrameBuffer,buffer);
  
  //engine->renderer.getTextureRepository().freeBySprite(spr_frameBuffer);
}

void Tutorial11::getFrameBuffer(unsigned char* pixelBuffer){
  ps2_screenshot(pixelBuffer,0,0,0,512,488,GS_PSM_32);
}

void Tutorial11::createFrameTexture(unsigned char* data, Vec2 size, float scale){
  PngLoader loader;  

  int sizeX = ((size.x)*scale)+1; // 128
  int sizeY = ((size.y)*scale)+1; // 128
  // data of new texture
  // se puede mejorar
  data = static_cast<unsigned char*>(memalign(
    128, loader.getTextureSize(sizeX, sizeY, bpp32)));    //guarda 128 pixeles. [0][0] hasta [127][127]
}

bool Tutorial11::existFrameTexture(Sprite* framebuffer){
  //std::vector<Texture*>* all = engine->renderer.getTextureRepository().getAll();
  //for(int i=0; i< engine->renderer.getTextureRepository().getTexturesCount();i++){
  //  if(all->at(i)->isLinkedWith(spr_frameBuffer.id) == 1){
  //    engine->renderer.getTextureRepository().free(all->at(i));
  //    printf("si existe, se borrara\n");
  //  }
  //}

  //engine->renderer.getTextureRepository().freeBySprite(*framebuffer);
  Texture* texture = engine->renderer.getTextureRepository().getBySpriteId(framebuffer->id);
  
  // si existe una textura para el framebuffer lo borro
  //if(texture != nullptr){
  //engine->renderer.getTextureRepository().freeBySprite(*framebuffer);
  
    RendererCoreTextureSender sender;
    sender.init(engine->renderer.core.getPath3(),&engine->renderer.core.gs);
    auto buffer = engine->renderer.core.texture.useTexture(texture);
    sender.deallocate(buffer);
    //engine->renderer.getTextureRepository().free(texture);
    printf("si existe, se borrara\n");
    return true;
  //}
  //printf("no existe la textura del framebuffer\n");
  return false;
}

}  // namespace Tyra
