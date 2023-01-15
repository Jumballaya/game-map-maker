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
#include "../tilemap/TileMap.h"

class EditorGUI
{
public:
  EditorGUI() = default;
  ~EditorGUI() = default;
  void render(
      EditorState state,
      std::unique_ptr<Mouse> &mouse,
      std::unique_ptr<EventBus> &eventBus,
      std::unique_ptr<AssetStore> &assetStore,
      std::shared_ptr<TileMap> &tileMap);

  void renderSidebar(
      EditorState state,
      std::unique_ptr<Mouse> &mouse,
      std::unique_ptr<EventBus> &eventBus,
      std::unique_ptr<AssetStore> &assetStore,
      std::shared_ptr<TileMap> &tileMaps);

  void renderMainMenuBar(std::unique_ptr<EventBus> &eventBus, bool *openMapModal);

  // Modals
  void renderOpenMapModal(std::unique_ptr<EventBus> &eventBus, bool *openMapModal);

  // Components

  void renderScriptBox(std::unique_ptr<EventBus> &eventBus);
};

#endif