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
      int imgWidth,
      int imgHeight,
      int tileSize,
      int tileCol,
      int tileRow,
      std::unique_ptr<Mouse> &mouse,
      std::unique_ptr<EventBus> &eventBus,
      SDL_Texture *selectedTileset);

  void renderSidebar(
      int imgWidth,
      int imgHeight,
      int tileSize,
      int tileCol,
      int tileRow,
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