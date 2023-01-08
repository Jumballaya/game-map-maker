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

  //////////////
  //  SIDEBAR
  ///////////////////
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration;
  int sideBarWidth = imgWidth * 2 + (tileSize / 2);
  ImGui::SetNextWindowSize(ImVec2(sideBarWidth, Editor::windowHeight - 20), 0);
  ImGui::SetNextWindowPos(ImVec2(Editor::windowWidth - sideBarWidth, 20));
  ImGui::Begin("Tiles and Textures", NULL, windowFlags);
  ImGui::Text("Tile Selection", 22);
  ImGui::Separator();
  ImGui::BeginChild("tiles", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

  auto scrollY = ImGui::GetScrollY();
  auto scrollX = ImGui::GetScrollX();

  int imageWidth = imgWidth * 2;
  int imageHeight = imgHeight * 2;

  ImGui::Image(selectedTileset, ImVec2(imageWidth, imageHeight));

  glm::vec2 mousePos = mouse->getPosition();
  int mousePosX = static_cast<int>(ImGui::GetMousePos().x - ImGui::GetWindowPos().x + scrollX);
  int mousePosY = static_cast<int>(ImGui::GetMousePos().y - ImGui::GetWindowPos().y + scrollY);

  int rows = imageHeight / (tileSize * 2);
  int cols = imageWidth / (tileSize * 2);

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

  ImGui::End();

  ///////////////////////
  //  MAIN MENU BAR
  /////////////////////
  bool openMapModal = false;
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
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  ///////////////
  //  OPEN FILE DIALOG
  //////////////

  if (openMapModal)
  {
    ImGui::OpenPopup("Open Map File");
  }
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

  ImGui::Render();
  ImGuiSDL::Render(ImGui::GetDrawData());
}