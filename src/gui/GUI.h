#ifndef EDITOR_GUI_H
#define EDITOR_GUI_H

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdl.h>
#include "../assetstore/AssetStore.h"
#include "../mouse/Mouse.h"
#include "../eventbus/EventBus.h"
#include "../editor/EditorState.h"

class EditorGUI
{
public:
  EditorGUI() = default;
  ~EditorGUI() = default;
  void render(
      EditorState state,
      std::unique_ptr<Mouse> &mouse,
      std::unique_ptr<EventBus> &eventBus,
      std::unique_ptr<AssetStore> &assetStore);

  void renderSidebar(
      EditorState state,
      std::unique_ptr<Mouse> &mouse,
      std::unique_ptr<EventBus> &eventBus,
      std::unique_ptr<AssetStore> &assetStore);

  void renderMainMenuBar(std::unique_ptr<EventBus> &eventBus);

  // Modals
  void renderOpenMapModal(std::unique_ptr<EventBus> &eventBus);

  // Components

  void renderScriptBox(std::unique_ptr<EventBus> &eventBus);
};

#endif