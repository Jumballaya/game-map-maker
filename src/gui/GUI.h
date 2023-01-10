#ifndef EDITOR_GUI_H
#define EDITOR_GUI_H

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_sdl.h"
#include "../../libs/imgui/imgui_impl_sdl.h"
#include "../mouse/Mouse.h"
#include "../eventbus/EventBus.h"

class EditorGUI
{
public:
  EditorGUI() = default;
  ~EditorGUI() = default;
  void render(
      glm::vec2 imageSize,
      int tileSize,
      glm::vec2 selectedTileData,
      std::unique_ptr<Mouse> &mouse,
      std::unique_ptr<EventBus> &eventBus,
      SDL_Texture *selectedTileset);

  void renderSidebar(
      glm::vec2 imageSize,
      int tileSize,
      glm::vec2 selectedTileData,
      std::unique_ptr<Mouse> &mouse,
      std::unique_ptr<EventBus> &eventBus,
      SDL_Texture *selectedTileset);

  void renderMainMenuBar(bool &openMapModal);

  // Modals
  void renderOpenMapModal(std::unique_ptr<EventBus> &eventBus);

  // Components

  void renderScriptBox(std::unique_ptr<EventBus> &eventBus);
};

#endif