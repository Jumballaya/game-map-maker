#include "GUI.h"
#include "../events/Events.h"

void EditorGUI::render(
    EditorState state,
    std::unique_ptr<Mouse> &mouse,
    std::unique_ptr<EventBus> &eventBus,
    std::unique_ptr<AssetStore> &assetStore)
{
  ImGui::NewFrame();

  ///////////////////////
  //  MAIN MENU BAR
  /////////////////////
  static bool openMapModal = false; // check against EditorState
  renderMainMenuBar(eventBus);

  //////////////
  //  SIDEBAR
  ///////////////////
  renderSidebar(state, mouse, eventBus, assetStore);

  ///////////////
  //  OPEN FILE DIALOG
  //////////////
  if (openMapModal)
  {
    renderOpenMapModal(eventBus);
  }

  //////////////////
  /// Bottom Status Bar
  ///////////////////
  const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
  ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration;
  ImGui::SetNextWindowPos(ImVec2(0, main_viewport->Size.y - 90));
  ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x, 90));
  ImGui::Begin("Bottom Status Bar", NULL, winFlags);
  if (state.hoveringCanvas)
  {
    ImGui::Text("Mouse Position - [%d, %d]", static_cast<int>(state.hoveringCoords.x), static_cast<int>(state.hoveringCoords.y));
    ImGui::SameLine();
    ImGui::Spacing();
  }
  ImGui::SameLine();
  // ImGui::Text("Just placed a tile");

  ImGui::End();

  ImGui::Render();
  ImGuiSDL::Render(ImGui::GetDrawData());
}

void EditorGUI::renderSidebar(
    EditorState state,
    std::unique_ptr<Mouse> &mouse,
    std::unique_ptr<EventBus> &eventBus,
    std::unique_ptr<AssetStore> &assetStore)
{
  auto tileset = assetStore->getTileset(state.selectedTileset);
  auto texture = tileset->getTexture();
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration;
  const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
  int sideBarWidth = main_viewport->Size.x * .2;
  ImGui::SetNextWindowSize(ImVec2(sideBarWidth, state.windowHeight - 20), 0);
  ImGui::SetNextWindowPos(ImVec2(state.windowWidth - sideBarWidth, 20));
  ImGui::Begin("Tiles and Textures", NULL, windowFlags);

  // START TILE PICKER
  ImGui::Text("Tile Selection", 22);
  ImGui::Separator();
  ImGui::BeginChild("tiles", ImVec2(sideBarWidth, tileset->sizePixel.y + 30), false, ImGuiWindowFlags_HorizontalScrollbar);

  auto scrollY = ImGui::GetScrollY();
  auto scrollX = ImGui::GetScrollX();

  ImGui::Image(texture, ImVec2(tileset->sizePixel.x, tileset->sizePixel.y));

  int mousePosX = static_cast<int>(ImGui::GetMousePos().x - ImGui::GetWindowPos().x + scrollX);
  int mousePosY = static_cast<int>(ImGui::GetMousePos().y - ImGui::GetWindowPos().y + scrollY);

  int rows = tileset->sizePixel.y / tileset->tileSize;
  int cols = tileset->sizePixel.x / tileset->tileSize;

  for (int i = 0; i < cols; i++)
  {
    for (int j = 0; j < rows; j++)
    {
      // Check to see if we are in the area of the desired 2D tile
      bool mouseOverlapsX = mousePosX >= (tileset->sizePixel.x / cols) * i && mousePosX <= (tileset->sizePixel.x / cols) + ((tileset->sizePixel.x / cols) * i);
      bool mouseOverlapsY = mousePosY >= (tileset->sizePixel.y / rows) * j && mousePosY <= (tileset->sizePixel.y / rows) + ((tileset->sizePixel.y / rows) * j);
      if (mouseOverlapsX && mouseOverlapsY)
      {
        if (ImGui::IsItemHovered())
        {

          if (ImGui::IsMouseClicked(0))
          {
            eventBus->emit<TileSelectEvent>(i, j);
          }
        }
      }
    }
  }

  ImGui::EndChild();
  // END TILE PICKER

  // START CURRENT TILE
  ImGui::Separator();
  ImGui::Text("Currently Selected Tile", 22);
  double smallTileSizeX = 1.0 / static_cast<double>(cols);
  double smallTileSizeY = 1.0 / static_cast<double>(rows);
  double smallX = smallTileSizeX * static_cast<double>(state.selectedTileData.x);
  double smallY = smallTileSizeY * static_cast<double>(state.selectedTileData.y);

  ImGui::Image(texture, ImVec2(tileset->tileSize * 2, tileset->tileSize * 2), ImVec2(smallX, smallY), ImVec2(smallX + smallTileSizeX, smallY + smallTileSizeY));
  // END CURRENT TILE

  // START LAYERS
  ImGui::Separator();
  ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable;
  ImGui::Text("Tile Map Layers", 22);
  if (ImGui::BeginTable("layers-table", 1, tableFlags))
  {
    ImGui::TableSetupColumn("Layer Name");

    static const char *names[] = {"Layer 1", "Layer 2", "Layer 3", "Layer 4", "Layer 5"};
    static int selected = -1;
    for (int row = 0; row < IM_ARRAYSIZE(names); row++)
    {
      ImGui::TableNextRow();
      const char *item = names[row];
      for (int col = 0; col < 1; col++)
      {
        ImGui::TableSetColumnIndex(col);
        if (ImGui::Selectable(item, row == selected))
        {
          selected = row;
        };
        if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
        {
          int nextRow = row + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
          if (nextRow >= 0 && nextRow < IM_ARRAYSIZE(names))
          {
            selected = nextRow;
            names[row] = names[nextRow];
            names[nextRow] = item;
            ImGui::ResetMouseDragDelta();
          }
        }
      }
    }
    ImGui::EndTable();
  }
  // END LAYERS

  ImGui::ShowDemoWindow();

  ImGui::End();
}

void EditorGUI::renderScriptBox(std::unique_ptr<EventBus> &eventBus)
{
  /// Text Editor
  static char text[1024 * 16];
  ImGui::BeginChild("Lua Editor");
  ImGui::Text("Compile and run your LUA code here");
  ImGui::InputTextMultiline("Code", text, IM_ARRAYSIZE(text), ImVec2(0, 0), ImGuiInputTextFlags_AllowTabInput);

  if (ImGui::Button("Compile and Run"))
  {
    eventBus->emit<RunLUAEvent>(std::string(text));
  }

  ImGui::EndChild();
}

void EditorGUI::renderMainMenuBar(std::unique_ptr<EventBus> &eventBus)
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Open Map"))
      {
        // @TODO: Use events and set the state on the EditorState struct
        // openMapModal = true;
      }
      if (ImGui::MenuItem("Save Map"))
      {
      }
      if (ImGui::MenuItem("New Map"))
      {
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools"))
    {
      static int toolInUse = 0;
      if (ImGui::RadioButton("Place Tile", &toolInUse, 0))
      {
        eventBus->emit<TileToolSelectEvent>(TileTool::PlaceTile);
      };
      ImGui::Separator();
      if (ImGui::RadioButton("Erase Tile", &toolInUse, 1))
      {
        eventBus->emit<TileToolSelectEvent>(TileTool::EraseTile);
      };
      ImGui::Separator();
      if (ImGui::RadioButton("Flood Fill Tiles", &toolInUse, 2))
      {
        eventBus->emit<TileToolSelectEvent>(TileTool::FloodFill);
      };
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

void EditorGUI::renderOpenMapModal(std::unique_ptr<EventBus> &eventBus)
{
  ImGui::OpenPopup("Open Map File");
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

  if (ImGui::BeginPopupModal("Open Map File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Type in the map file's location below\nThen hit the button marked 'open' to start editing");
    ImGui::Separator();

    static char buf[256];
    ImGui::InputText("Filepath", buf, IM_ARRAYSIZE(buf));

    ImGui::Separator();
    if (ImGui::Button("Open", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup();
      // OPEN_MAP_EVENT
      // loadMap(std::string(buf));
    }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}