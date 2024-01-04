/*
# _____        ____   ___
#   |     \/   ____| |___|
#   |     |   |   \  |   |
#-----------------------------------------------------------------------
# Copyright 2022, tyra - https://github.com/h4570/tyra
# Licensed under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "renderer/core/texture/texture_repository.hpp"

namespace Tyra {

TextureRepository::TextureRepository() {}

TextureRepository::~TextureRepository() {
  TYRA_LOG("Texture repository destructed!");
  if (getTexturesCount() > 0) {
    for (u32 i = 0; i < getTexturesCount(); i++) delete textures[i];
    textures.clear();
  }
}

Texture* TextureRepository::getBySpriteId(const u32& t_id) const {
  for (u32 i = 0; i < textures.size(); i++) {
    if (textures[i]->isLinkedWith(t_id)) return textures[i];
  }
  return nullptr;
}

Texture* TextureRepository::getByMeshMaterialId(const u32& t_id) const {
  for (u32 i = 0; i < textures.size(); i++) {
    if (textures[i]->isLinkedWith(t_id)) return textures[i];
  }
  return nullptr;
}

Texture* TextureRepository::getByTextureId(const u32& t_id) const {
  for (u32 i = 0; i < textures.size(); i++)
    if (t_id == textures[i]->id) return textures[i];
  return nullptr;
}

const s32 TextureRepository::getIndexOf(const u32& t_texId) const {
  for (u32 i = 0; i < textures.size(); i++)
    if (textures[i]->id == t_texId) return i;
  return -1;
}

Texture* TextureRepository::add(Texture* texture) {
  textures.push_back(texture);
  return texture;
}

void TextureRepository::removeByIndex(const u32& t_index) {
  textures.erase(textures.begin() + t_index);
}

void TextureRepository::removeById(const u32& t_texId) {
  s32 index = getIndexOf(t_texId);
  TYRA_ASSERT(index != -1, "Cant remove texture, because it was not found!");
  removeByIndex(index);
}

void TextureRepository::freeByMesh(const Mesh& mesh) {
  for (auto* material : mesh.materials) {
    auto* texture = getByMeshMaterialId(material->id);

    if (texture != nullptr) {
      free(texture);
    }
  }
}

void TextureRepository::freeByMesh(const Mesh* mesh) { freeByMesh(*mesh); }

void TextureRepository::freeBySprite(const Sprite& sprite) {
  auto* texture = getBySpriteId(sprite.id);

  if (texture != nullptr) {
    free(texture);
  }
}

void TextureRepository::free(const Texture* t_tex) { free(t_tex->id); }

void TextureRepository::free(const Texture& t_tex) { free(t_tex.id); }

void TextureRepository::free(const u32& t_texId) {
  s32 index = getIndexOf(t_texId);
  auto* tex = textures[index];

  TYRA_ASSERT(index != -1, "Cant remove texture, because it was not found!");
  removeByIndex(index);

  delete tex;
}

Texture* TextureRepository::add(const char* fullpath){
  return add(fullpath, Vec2(0,0), Vec2(0,0), false);
}

Texture* TextureRepository::add(const char* fullpath, Vec2 pos, Vec2 size, const bool rect) {
  TextureLoader& loader = texLoaderSelector.getLoaderByFileName(fullpath);

  if(rect == true){
    TYRA_ASSERT(size.x <= 512 && size.y <= 512,
              "Tyra supports only 512x512 textures max!");

    TYRA_ASSERT(size.x == 8 || size.x == 16 ||
                    size.x == 32 || size.x == 64 ||
                    size.x == 128 || size.x == 256 ||
                    size.x == 512,
                "Texture width/height should be 8/16/32/64/128/256/512!");

    TYRA_ASSERT(size.y == 8 || size.y == 16 ||
                    size.y == 32 || size.y == 64 ||
                    size.y == 128 || size.y == 256 ||
                    size.y == 512,
                "Texture width/height should be 8/16/32/64/128/256/512!");
  }
  
  auto* data = loader.load(fullpath,pos.x,pos.y,size.x,size.y,rect);
  Texture* texture = new Texture(data);
  delete data;
  texture->print();
  textures.push_back(texture);
  return texture;
}

void TextureRepository::addByMesh(const Mesh* mesh, const char* directory,
                                  const char* extension) {
  auto& loader = texLoaderSelector.getLoaderByExtension(extension);

  std::string dirFixed = directory;
  if (dirFixed.back() != '/' && dirFixed.back() != ':') dirFixed += "/";

  for (u32 i = 0; i < mesh->materials.size(); i++) {
        if (!mesh->materials[i]->textureName.has_value()) {
      continue;
    }

    std::string fullPath =
        dirFixed + mesh->materials[i]->textureName.value() + "." + extension;

    auto* data = loader.load(fullPath.c_str());
    Texture* texture = new Texture(data);
    delete data;

    texture->addLink(mesh->materials[i]->id);
    textures.push_back(texture);
  }
}

}  // namespace Tyra
