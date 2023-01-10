#include "GUI.h"
#include "../editor/Editor.h"

#include "../events/Events.h"

void EditorGUI::render(
    int imgWidth,
    int imgHeight,
    int tileSize,
    int tileCol,
    int tileRow,
    std::unique_ptr<Mouse> &mouse,
    std::unique_ptr<EventBus> &eventBus,
    SDL_Texture *selectedTileset)
{
  ImGui::NewFrame();

  ///////////////////////
  //  MAIN MENU BAR
  /////////////////////
  bool openMapModal = false;
  renderMainMenuBar(openMapModal);

  //////////////
  //  SIDEBAR
  ///////////////////
  renderSidebar(imgWidth, imgHeight, tileSize, tileCol, tileRow, mouse, eventBus, selectedTileset);

  ///////////////
  //  OPEN FILE DIALOG
  //////////////
  if (openMapModal)
  {
    renderOpenMapModal(eventBus);
  }

  ImGui::Render();
  ImGuiSDL::Render(ImGui::GetDrawData());
}

void EditorGUI::renderSidebar(
    int imageWidth,
    int imageHeight,
    int tileSize,
    int tileCol,
    int tileRow,
    std::unique_ptr<Mouse> &mouse,
    std::unique_ptr<EventBus> &eventBus,
    SDL_Texture *selectedTileset)
{
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration;
  int sideBarWidth = imageWidth + tileSize;
  ImGui::SetNextWindowSize(ImVec2(sideBarWidth, Editor::windowHeight - 20), 0);
  ImGui::SetNextWindowPos(ImVec2(Editor::windowWidth - sideBarWidth, 20));
  ImGui::Begin("Tiles and Textures", NULL, windowFlags);

  // START TILE PICKER
  ImGui::Text("Tile Selection", 22);
  ImGui::Separator();
  ImGui::BeginChild("tiles", ImVec2(sideBarWidth, imageHeight + 30), false, ImGuiWindowFlags_HorizontalScrollbar);

  auto scrollY = ImGui::GetScrollY();
  auto scrollX = ImGui::GetScrollX();

  ImGui::Image(selectedTileset, ImVec2(imageWidth, imageHeight));

  int mousePosX = static_cast<int>(ImGui::GetMousePos().x - ImGui::GetWindowPos().x + scrollX);
  int mousePosY = static_cast<int>(ImGui::GetMousePos().y - ImGui::GetWindowPos().y + scrollY);

  int rows = imageHeight / tileSize;
  int cols = imageWidth / tileSize;

  for (int i = 0; i < cols; i++)
  {
    for (int j = 0; j < rows; j++)
    {
      // Check to see if we are in the area of the desired 2D tile
      if ((mousePosX >= (imageWidth / cols) * i && mousePosX <= (imageWidth / cols) + ((imageWidth / cols) * i)) && (mousePosY >= (imageHeight / rows) * j && mousePosY <= (imageHeight / rows) + ((imageHeight / rows) * j)))
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
  ImGui::BeginChild("Selected Tile");
  ImGui::Text("Currently Selected Tile", 22);
  double smallTileSizeX = 1.0 / static_cast<double>(cols);
  double smallTileSizeY = 1.0 / static_cast<double>(rows);
  double smallX = smallTileSizeX * static_cast<double>(tileCol);
  double smallY = smallTileSizeY * static_cast<double>(tileRow);
  ImGui::Image(selectedTileset, ImVec2(tileSize * 2, tileSize * 2), ImVec2(smallX, smallY), ImVec2(smallX + smallTileSizeX, smallY + smallTileSizeY));
  ImGui::EndChild();
  // END CURRENT TILE

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

void EditorGUI::renderMainMenuBar(bool &openMapModal)
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Open Map"))
      {
        openMapModal = true;
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
      ImGui::RadioButton("Tile Placer", &toolInUse, 0);
      ImGui::Separator();
      ImGui::RadioButton("Tile Deleter", &toolInUse, 1);
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