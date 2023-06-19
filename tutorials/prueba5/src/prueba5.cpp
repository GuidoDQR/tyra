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
#include "prueba5.hpp"
#include <screenshot.h>
#include <malloc.h>
#include <gs_psm.h>
#include <draw_buffers.h>

namespace Tyra {

Prueba5::Prueba5(Engine* t_engine) : engine(t_engine), pad(&t_engine->pad) {}

Prueba5::~Prueba5() {
  engine->renderer.getTextureRepository().freeBySprite(sprite);
  engine->renderer.getTextureRepository().freeBySprite(mask);
  engine->renderer.getTextureRepository().freeBySprite(punto);
  for(unsigned int i=0; i< spr_effects.size();i++){
    engine->renderer.getTextureRepository().freeBySprite(spr_effects[i]);
  }

}

void Prueba5::init() {
  engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

  /** Sprite contains rectangle information. */
  loadSprite();

  /** Texture contains png image. */
  loadTexture();
  //printf("count textures: %d\n",engine->renderer.getTextureRepository().getTexturesCount());

  //auto* tex = engine->renderer.getTextureRepository().getBySpriteId(sprite.id);
  engine->renderer.core.renderer2D.setTextureMappingType(TyraNearest);
  //printf("-1mb: %f\n",engine->renderer.core.gs.vram.getFreeSpaceInMB());
  //printf("mb sprite1: %f\n",tex->getSizeInMB());
  existFrameSprite(&sprite);
  renderMask(&sprite,&mask);
  //printf("count textures: %d\n",engine->renderer.getTextureRepository().getTexturesCount());
  //printf("mb sprite2: %f\n",tex->getSizeInMB());
  //printf("0mb: %f\n",engine->renderer.core.gs.vram.getFreeSpaceInMB()); 
}

void Prueba5::loop() {
  auto& renderer = engine->renderer;
  static bool pass = true;
  static bool pass2 = false;

  if(pad->getClicked().Cross){
    pass2 = !pass2;

    if(pass2 == true){
      printf("FR: TRUE\n");
    }else{printf("FR: false\n");}
    
  }else if(pad->getClicked().Circle){
    //printf("count textures: %d\n",renderer.getTextureRepository().getTexturesCount());
    pass = !pass;
  }else if(pad->getClicked().Square){
    
  }

  if(pad->getLeftJoyPad().v <= 100){
    mask.position -= Vec2(0,1); //up
        renderMask(&sprite,&mask); 
  }else if(pad->getLeftJoyPad().v >= 200){
    mask.position += Vec2(0,1); //down
        renderMask(&sprite,&mask); 
  }

   if (pad->getLeftJoyPad().h <= 100) {
      mask.position -= Vec2(1,0); // left
          renderMask(&sprite,&mask); 
   } else if (pad->getLeftJoyPad().h >= 200) {
      mask.position += Vec2(1,0); //right
          renderMask(&sprite,&mask); 
   }

  /** Begin frame will clear our screen. */
  renderer.beginFrame();

  /** Render sprite. */
  //engine->renderer.core.renderer2D.setTextureMappingType(TyraNearest);
  
  //   if(pass2 == false){
  // renderer.renderer2D.render(mask);
  // }
  //printf("1mb: %f\n",engine->renderer.core.gs.vram.getFreeSpaceInMB());
  if(pass == true){
    existFrameSprite(&sprite);

    renderer.renderer2D.render(sprite);  
     
  }
  //printf("2mb: %f\n",engine->renderer.core.gs.vram.getFreeSpaceInMB());
  //renderer.renderer2D.render(punto);

  if(pass2 == true){
    renderer.renderer2D.render(mask);
  }

  //printf("3mb: %f\n",engine->renderer.core.gs.vram.getFreeSpaceInMB());

  renderer.endFrame();
}

void Prueba5::loadSprite() {
  //const auto& screenSettings = engine->renderer.core.getSettings();

  sprite.mode = SpriteMode::MODE_STRETCH;

  /** Let's scale it down */
  sprite.size = Vec2(32.0F, 32.0F);
  sprite.scale = 4;
  perfectPixelSize(&sprite);
  /** Set it in screen center */
  sprite.position = Vec2(0.0F,0.0F);

  //sprite.offset = Vec2(0.5F,0.5F);
  mask.mode = sprite.mode;

  mask.size = Vec2(32.0F,32.0F);

  mask.position = Vec2(32.0F,32.0F);//sprite.position;
  perfectPixelSize(&mask);

  punto.mode = MODE_STRETCH;
  punto.position = Vec2(127.0F,127.0F);
  punto.size = Vec2(0,0);
  TYRA_LOG("Sprite created!");
}

void Prueba5::loadTexture() {
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

  //texture->addLink(punto.id);
  TYRA_LOG("Texture loaded!");
}

bool Prueba5::existFrameSprite(Sprite* sprite){
  bool exist = false;
  for(u32 i=0;i < id_spr.size() ;i++){
    if(sprite->id == id_spr[i]){ 
      //printf("se encontro la textura. id: %d\n",spr_effects[i].id);
      reScaleTexture(sprite,&spr_effects[i]);
      //printf("size:%f,%f\n",spr_effects[i].size.x,spr_effects[i].size.y);
      
      return true;
    }
  }
  // No existe, se crea uno
  printf("se creo una nueva textura\n");
  Sprite spr;
  u32 id = spr.id;
  spr = *sprite;
  spr.id = id;
  spr.size = Vec2(sprite->size.x*sprite->scale,sprite->size.y*sprite->scale);
  spr.scale = 1.0F;
  auto* texture = engine->renderer.core.texture.repository.getBySpriteId(sprite->id);
  auto* data = texture->getData(); // obtengo los datos del sprite
  //tex_effects.push_back(new Texture(data));
  Texture* newTex = new Texture(data);

  delete data;
  spr_effects.push_back(spr);
  id_spr.push_back(sprite->id);
  //tex_effects[tex_effects.size()-1]->addLink(spr.id);
  //engine->renderer.getTextureRepository().add(tex_effects[tex_effects.size()-1]);
  engine->renderer.getTextureRepository().add(newTex);
  newTex->addLink(spr.id);
  //printf("id de la nueva textura:%d\n",spr.id);
  reScaleTexture(sprite,&spr);
  exist = true;
  
  
  return exist;
}

void Prueba5::reScaleTexture(Sprite* sprite,Sprite* spr ){
  //Sprite* spr = &spr_effects[pos];

  // obtengo la textura del sprite
  // la primera vez sera 32x32 su textura
  auto* texture = engine->renderer.core.texture.repository.getBySpriteId(sprite->id);
  auto* texture_effects = engine->renderer.core.texture.repository.getBySpriteId(spr->id);

  int minTexX = texture->getWidth()/2; // 32/2=16
  int minTexY = texture->getHeight()/2; // 32/2=16

  int maxTexX = texture->getWidth();
  int maxTexY = texture->getHeight();
  // printf("spr size: %f,%f\n",spr->size.x,spr->size.y);
  // printf("sprite size: %f,%f\n",sprite->size.x*sprite->scale,sprite->size.y*sprite->scale);
  // printf("minTex: %d,%d\n",minTexX,minTexY);
  // printf("maxTex: %d,%d\n",maxTexX,maxTexX);

  if(spr->size.x != (sprite->size.x*sprite->scale) ||
     spr->size.y != (sprite->size.y*sprite->scale) ||
     // si 127 es mayor a 16 y 127 es menor a 32, es falso 
    (spr->size.x>minTexX && spr->size.x<maxTexX ) == false ||
    (spr->size.y>minTexY && spr->size.y<maxTexY ) == false 
    ){
      // las texturas tienen que ser 8,16,32,64,128,256 o 512
      // si se dibujo una vez y se cambio el tamaño de la textura
      // se tiene que borrar la textura conectada al sprite y darle uno nuevo
      if( ((spr->size.x>minTexX && spr->size.x<maxTexX ) == false ||
          (spr->size.y>minTexY && spr->size.y<maxTexY ) == false) &&
          texture != nullptr
          ){
        //    printf("pase\n");
        engine->renderer.getTextureRepository().free(texture);
        auto* data = texture_effects->getData(); // obtengo los datos del sprite
        texture = new Texture(data);
        delete data;
        engine->renderer.getTextureRepository().add(texture);
        texture->addLink(sprite->id);
        // printf("pase2\n");
      }

      // si el sprite modificado vuelve a su tamaño original solo se copian los datos
      if(sprite->size.x*sprite->scale == texture_effects->getWidth() &&
         sprite->size.y*sprite->scale == texture_effects->getHeight()){
          texture->core->width  = texture_effects->getWidth();
          texture->core->height = texture_effects->getHeight();
          texture->core->data   = texture_effects->core->data;
      }else{
        int sizeX=0;
        int sizeY=0;
        if(sprite->size.x*sprite->scale < 8 ){
          sizeX = 8;
        }else if(sprite->size.x*sprite->scale < 16){
          sizeX = 16;
        }else if(sprite->size.x*sprite->scale < 32){
          sizeX = 32;
        }else if(sprite->size.x*sprite->scale < 64){
          sizeX = 64;
        }else if(sprite->size.x*sprite->scale < 128){
          sizeX = 128;
        }else if(sprite->size.x*sprite->scale < 256){
          sizeX = 256;
        }

         if(sprite->size.y*sprite->scale < 8 ){
          sizeY = 8;
        }else if(sprite->size.y*sprite->scale < 16){
          sizeY = 16;
        }else if(sprite->size.y*sprite->scale < 32){
          sizeY = 32;
        }else if(sprite->size.y*sprite->scale < 64){
          sizeY = 64;
        }else if(sprite->size.y*sprite->scale < 128){
          sizeY = 128;
        }else if(sprite->size.y*sprite->scale < 256){
          sizeY = 256;
        }
        PngLoader loader;
        unsigned char* pixel_texture = static_cast<unsigned char*>(memalign(
          128, loader.getTextureSize(sizeX, sizeY, bpp32))); 

        struct PngPixel4* pngTexture = (struct PngPixel4*) pixel_texture;
        struct PngPixel4* pngOriginalTexture = (struct PngPixel4*) texture_effects->core->data;

        float texSizeX = texture_effects->getWidth();
        float texSizeY = texture_effects->getHeight();
        float unitX  = texSizeX/sizeX;
        float unitY  = texSizeY/sizeY;
        int pospng;
        int pospngTex = 0;
        //printf("unit x,y: %f,%f\n",unitX,unitY);
        //printf("size text x,y: %d,%d\n",tex_effects[pos]->getWidth(),tex_effects[pos]->getHeight());
        //printf("size x,y: %d,%d\n",sizeX,sizeY);
        //printf("pase3\n");
        for(float v=0; v < texture_effects->getHeight(); v+=unitY){
          for(float u=0; u < texture_effects->getWidth(); u+=unitX){
            pospng = (int)u+((int)v*texture_effects->getHeight());
            pngTexture[pospngTex] = pngOriginalTexture[pospng];
            //printf("v,u:%f,%f:%d\n",v,u,pospng);
            //if(pospngTex%2==0){
            //  pngTexture[pospngTex].r =255;
            //  pngTexture[pospngTex].g = 0;
            //  pngTexture[pospngTex].b = 0;
            //}
            //printf("%d:%d,%d,%d,%d\n",pospngTex,pixel_texture[(pospngTex*4)],pixel_texture[(pospngTex*4)+1],
            //pixel_texture[(pospngTex*4)+2],pixel_texture[(pospngTex*4)+3]);
            pospngTex++;
          }
        }
        texture->core->width  = sizeX;
        texture->core->height = sizeY;
        texture->core->data   = pixel_texture;

        unsigned char* new_tex_data = static_cast<unsigned char*>(memalign(
        128, loader.getTextureSize(sizeX, sizeY, bpp32))); 

        memcpy(new_tex_data,pixel_texture,loader.getTextureSize(sizeX, sizeY, bpp32));
        tex_data.push_back(new_tex_data);
        //printf("pase4\n");
      }
   // printf("pase5\n");
  }
}

bool Prueba5::existFrameTexture(Sprite* sprite,Sprite* spr){
  return false;
}

void Prueba5::perfectPixelSize(Sprite* sprite){
  
  float sizeRequiredX =  sprite->size.x * sprite->scale; // 32 *1 = 32 //32 * 4 = 128
  float sizeRequiredY =  sprite->size.y * sprite->scale;

  sizeRequiredX -= 1.0f; // 127
  sizeRequiredY -= 1.0f; // 127
  
  sprite->size.x = sizeRequiredX / sprite->scale; // 31/1 = 31 // 127/4 = 31.75
  sprite->size.y = sizeRequiredY / sprite->scale;
}

int Prueba5::renderMask(Sprite* sprite, Sprite* mask){
  Vec2 maskPos1 = mask->position; // 32,32
  Vec2 maskPos2 = Vec2((mask->size.x * mask->scale)+ maskPos1.x, (mask->size.y * mask->scale)+ maskPos1.y); // 63,63
  Vec2 sprPos1  = sprite->position; // 10,10
  Vec2 sprPos2  = Vec2((sprite->size.x * sprite->scale)+ sprPos1.x, (sprite->size.y * sprite->scale)+ sprPos1.y); // 137,137
  
  auto* tex = engine->renderer.getTextureRepository().getBySpriteId(sprite->id);

  const int pixelX = tex->getWidth();  // ancho de la textura del sprite
  const int pixelY = tex->getHeight(); // alto  de la textura del sprite

  int pixelMax = pixelX * pixelY;

  struct PngPixel4* pngSprite= (struct PngPixel4*) tex->core->data;
  auto buffer = engine->renderer.core.texture.useTexture(tex);

  // verifica que el mask este adentro del sprite
  if(!(maskPos1.x <= sprPos2.x && maskPos2.x >= sprPos1.x &&
     maskPos1.y <= sprPos2.y && maskPos2.y >= sprPos1.y)){

    //verifica que este un pixel al borde de la imagen para devolver sus datos normales
    if(maskPos1.x <= sprPos2.x+1 && maskPos2.x >= sprPos1.x-1 &&
     maskPos1.y <= sprPos2.y+1 && maskPos2.y >= sprPos1.y-1){
      //printf("esta en el limite\n");

      for(u32 i=0;i < id_spr.size() ;i++){
        if(sprite->id == id_spr[i]){ 
          //printf("se encontro la textura. id: %d\n",spr_effects[i].id);
          memcpy(tex->core->data,tex_data[i],128*128*4);
          // for(int j=0;j<pixelMax;j++){
          //   //if(!(pngSprite[i].r==0 && pngSprite[i].g==0 && pngSprite[i].g==0))
          //     *tex->core->data[j] =  *tex_data[i][j];
          // }
          
        }
      }

      Path3* path = engine->renderer.core.getPath3();
      path->sendTexture(tex,buffer);
    }
 
    return 1;
  }
  //printf("shader\n");
  auto* texMask = engine->renderer.getTextureRepository().getBySpriteId(mask->id);  

  struct PngPixel4* pngMask  = (struct PngPixel4*) texMask->core->data;

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

  if(maskInSprite2.y > pixelY-1){
    countPixel.y = pixelY - maskInSprite1.y;
  }

  // si la posicion inicial X del mask es menor a la del sprite
  
  if(maskInSprite1.x < 0){
    pixelMaskPos.x -= maskInSprite1.x; 
    //countPixel.x += maskInSprite1.x;  
  }
  
  // si la posicion inicial X del mask es mayor a la del sprite

  if(maskInSprite2.x > pixelX-1){
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

  if(maskInSprite2.y > pixelY-1){
    //maskInSprite2.y = sprPos2.y - maskInSprite1.y;
    maskInSprite2.y = pixelY-1;
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

  static int k=0;

  // printf("mask pos: %f,%f\n",maskPos1.x,maskPos1.y);
  // printf("mask1: %f,%f\n",maskInSprite1.x,maskInSprite1.y);
  // printf("mask2: %f,%f\n",maskInSprite2.x,maskInSprite2.y);
  // printf("initialPixel: %d\n",initialPixel);
  // printf("pixelmask: %f,%f\n",pixelMaskPos.x,pixelMaskPos.y);
  // printf("countPixel: %f,%f\n",countPixel.x,countPixel.y);
  // printf("\n");

  // for(int i=0;i<pixelMax;i++){
  //   if(!(pngSprite[i].r==0 && pngSprite[i].g==0 && pngSprite[i].g==0))
  //     pngSprite[i].a = 128;
  // }
  for(u32 i=0;i < id_spr.size() ;i++){
    if(sprite->id == id_spr[i]){ 
      //printf("se encontro la textura. id: %d\n",spr_effects[i].id);
      // se copian los datos originales
      // lo bueno de esta forma es que se puede cambiar el color de la textura
      memcpy(tex->core->data,tex_data[i],128*128*4);  
    }
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

    // if(k==0){
    //   printf("pixelMask[%d][%d]: %d=pixelSprite[%d]\n",counterY,counterX,counter,i);
    //   //printf("pixel[%d][%d]:%d: %d,%d,%d,%d\n",counterY,counterX,counter,pngMask[counter].r,pngMask[counter].g,pngMask[counter].b,pngMask[counter].a);
    // }
    if(pngMask[counter].r == 255 && pngMask[counter].g == 255 &&
       pngMask[counter].b == 255 ){
        // pngSprite[i].r = 0;
        // pngSprite[i].g = 0;
        // pngSprite[i].b = 255;
        pngSprite[i].a = 0;
    }

    counter++;
    counterX++;    
  }
  //printf("\n");
  k++;
  Path3* path = engine->renderer.core.getPath3();
  path->sendTexture(tex,buffer);
  return 0;
}
}  // namespace Tyra
