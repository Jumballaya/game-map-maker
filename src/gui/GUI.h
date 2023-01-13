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
#include "../editor/EditorState.h"

class EditorGUI
{
public:
  EditorGUI() = default;
  ~EditorGUI() = default;
  void render(
      EditorState state,
      std::unique_ptr<Mouse> &mouse,
      std::unique_ptr<EventBus> &eventBus);

  void renderSidebar(
      EditorState state,
      std::unique_ptr<Mouse> &mouse,
      std::unique_ptr<EventBus> &eventBus);

  void renderMainMenuBar(std::unique_ptr<EventBus> &eventBus);

  // Modals
  void renderOpenMapModal(std::unique_ptr<EventBus> &eventBus);

  // Components

  void renderScriptBox(std::unique_ptr<EventBus> &eventBus);
};

#endif