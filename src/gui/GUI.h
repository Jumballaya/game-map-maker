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

struct UIState
{
  bool modal_map_new;
  bool modal_map_open;
  bool modal_map_save;

  bool modal_tileset_add;

  UIState()
  {
    modal_map_new = false;
    modal_map_open = false;
    modal_map_save = false;
    modal_tileset_add = false;
  }
};

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

  void renderMainMenuBar(std::unique_ptr<EventBus> &eventBus);

  // Modals
  void renderOpenMapModal(std::unique_ptr<EventBus> &eventBus);
  void renderNewMapModal(std::unique_ptr<EventBus> &eventBus);
  void renderSaveMapModal(std::unique_ptr<EventBus> &eventBus);
  void renderAddTilesetModal(std::unique_ptr<EventBus> &eventBus);

  // Components

  void renderScriptBox(std::unique_ptr<EventBus> &eventBus);

private:
  UIState state;
};

#endif