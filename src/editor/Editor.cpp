#include <fstream>

#include <SDL2/SDL_image.h>
#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_sdl.h"
#include "../../libs/imgui/imgui_impl_sdl.h"

#include "Editor.h"
#include "../logger/Logger.h"
#include "../events/Events.h"

int Editor::windowWidth;
int Editor::windowHeight;

void Editor::renderGui()
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

  int mousePosX = static_cast<int>(ImGui::GetMousePos().x - ImGui::GetWindowPos().x + scrollX);
  int mousePosY = static_cast<int>(ImGui::GetMousePos().y - ImGui::GetWindowPos().y + scrollY);

  mouseX = ImGui::GetMousePos().x;
  mouseY = ImGui::GetMousePos().y;

  int rows = imageHeight / (tileSize * 2);
  int cols = imageWidth / (tileSize * 2);

  for (int i = 0; i < cols; i++)
  {
    for (int j = 0; j < rows; j++)
    {
      // auto drawList = ImGui::GetWindowDrawList();

      // Check to see if we are in the area of the desired 2D tile
      if ((mousePosX >= (imageWidth / cols) * i && mousePosX <= (imageWidth / cols) + ((imageWidth / cols) * i)) && (mousePosY >= (imageHeight / rows) * j && mousePosY <= (imageHeight / rows) + ((imageHeight / rows) * j)))
      {
        if (ImGui::IsItemHovered())
        {

          if (ImGui::IsMouseClicked(0))
          {

            tileCol = i;
            tileRow = j;
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
    }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  ImGui::ShowDemoWindow();

  ImGui::Render();
  ImGuiSDL::Render(ImGui::GetDrawData());
}

Editor::Editor()
{
  Logger::Log("[Editor] constructor called");
  running = false;
  assetStore = std::make_unique<AssetStore>();
  eventBus = std::make_unique<EventBus>();
  canvas = std::make_unique<Canvas>(0, 0, 0);
}

Editor::~Editor()
{
  Logger::Log("[Editor] destructor called");
}

void Editor::initialize()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    Logger::Error("Error initializing SDL");
    return;
  };

  if (TTF_Init() != 0)
  {
    Logger::Error("Error initializing SDL TTF");
    return;
  }
  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);

  this->windowWidth = dm.w;
  this->windowHeight = dm.h;
  this->window = SDL_CreateWindow(
      "Tile map editor",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      windowWidth,
      windowHeight,
      SDL_WINDOW_MAXIMIZED);

  if (!window)
  {
    Logger::Error("Error creating SDL window");
    return;
  }

  this->renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer)
  {
    Logger::Error("Error creating SDL renderer");
    return;
  }

  ImGui::CreateContext();
  ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);
  running = true;
}

void Editor::setup()
{
  assetStore->addTexture(renderer, "image", "./assets/f22.png");
  assetStore->addTexture(renderer, "tilemap", "./assets/tiles_packed.png");
  ImGuiIO &io = ImGui::GetIO();
  io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
  io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
  io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
  io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
  io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
  io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
  io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
  io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;

  loadMap("");
}

void Editor::run()
{
  setup();
  while (running)
  {
    processInput();
    update();
    render();
  }
}

void Editor::processInput()
{
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent))
  {
    // IMGUI SDL input
    ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
    ImGuiIO &io = ImGui::GetIO();

    int mouseX, mouseY;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

    io.MousePos = ImVec2(mouseX, mouseY);
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

    // core SDL events
    switch (sdlEvent.type)
    {

    case SDL_WINDOWEVENT:
      switch (sdlEvent.window.event)
      {
      case SDL_WINDOWEVENT_RESIZED:
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        Editor::windowHeight = h;
        Editor::windowWidth = w;
        break;
      }
      break;

    case SDL_QUIT:
      running = false;
      break;

    case SDL_KEYDOWN:
      eventBus->emit<KeyPressEvent>(sdlEvent.key.keysym.sym);
      switch (sdlEvent.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        running = false;
        break;
      }
      break;

    case SDL_MOUSEWHEEL:
      if (sdlEvent.wheel.y > 0)
      {
        zoom *= 1.075;
      }
      if (sdlEvent.wheel.y < 0)
      {
        zoom *= 0.925;
      }
      break;
    }
  }
}

void Editor::update()
{
  Uint32 ticks = SDL_GetTicks();
  int timeToWait = MILLISECS_PER_FRAME - (ticks - millisPreviousFrame);
  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
  {
    SDL_Delay(timeToWait);
  }
  double deltaTime = (ticks - millisPreviousFrame) / 1000.0f;
  ImGuiIO &io = ImGui::GetIO();
  io.DeltaTime = deltaTime;
  this->millisPreviousFrame = ticks;
}

void Editor::render()
{
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // Render phase
  // Background
  SDL_Rect rect = {0, 0, windowWidth, windowHeight};
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderFillRect(renderer, &rect);

  // Canvas
  canvas->draw(renderer, zoom);

  // Render selected tile at the mouse
  int yPos = mouseY - ((tileSize * zoom) / 2);
  int ysrcRect = tileCol * tileSize;
  int xPos = mouseX - ((tileSize * zoom) / 2);
  int xSrcRect = tileRow * tileSize;
  SDL_Rect srcRect = {ysrcRect, xSrcRect, tileSize, tileSize};
  SDL_Rect dstrect = {xPos, yPos, tileSize * zoom, tileSize * zoom};
  SDL_RenderCopy(renderer, selectedTileset, &srcRect, &dstrect);

  // GUI
  renderGui();

  SDL_RenderPresent(renderer);
}

void Editor::loadMap(std::string filePath)
{
  // Parse map file

  // Set values from file
  selectedTileset = assetStore->getTexture("tilemap");
  mapWidth = 32;
  mapHeight = 32;
  tileRow = 0;
  tileCol = 0;
  tileSize = 16;
  imgWidth = 12 * 16;
  imgHeight = 10 * 16;
  zoom = 1;

  // Set Canvas up
  canvas->setTileSize(tileSize);
  canvas->setWidth(mapWidth * tileSize);
  canvas->setHeight(mapHeight * tileSize);
  canvas->setPosition(30, 30);

  // Set Tilemap up
}

void Editor::destroy()
{
  ImGuiSDL::Deinitialize();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}