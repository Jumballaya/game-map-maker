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

Editor::Editor()
{
  Logger::Log("[Editor] constructor called");
  running = false;
  assetStore = std::make_unique<AssetStore>();
  eventBus = std::make_unique<EventBus>();
  canvas = std::make_unique<Canvas>(0, 0, 0);
  mouse = std::make_unique<Mouse>(0, 0);
  gui = std::make_unique<EditorGUI>();
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

  eventBus->subscribe<TileSelectEvent>(this, &Editor::onTileSelect);

  loadMap("__init__");

  // Setup mouse
  int mouseX, mouseY;
  const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
  mouse->move(mouseX, mouseY);
  mouse->move(mouseX, mouseY);
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
    mouse->move(mouseX, mouseY);

    if (mouse->isDragging())
    {
      auto offset = mouse->getDragOffset();
      std::cout << "Drag Offset: " << offset.x << ", " << offset.y << std::endl;
      auto pos = mouse->getPosition();
      std::cout << "Mouse Pos: " << pos.x << ", " << pos.y << std::endl;
      canvas->offset(offset.x, offset.y);
    }

    if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
      mouse->click(1);
    }
    else
    {
      mouse->stopClick(1);
    }
    if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
      mouse->click(2);
    }
    else
    {
      mouse->stopClick(2);
    }
    if (buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE))
    {
      mouse->click(3);
    }
    else
    {
      mouse->stopClick(3);
    }

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
        mouse->zoomPlus();
      }
      if (sdlEvent.wheel.y < 0)
      {
        mouse->zoomMinus();
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
  glm::vec2 mouseCoords = mouse->getPosition();
  double mouseZoom = mouse->getZoom();

  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // Render phase
  // Background
  SDL_Rect rect = {0, 0, windowWidth, windowHeight};
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderFillRect(renderer, &rect);

  // Canvas
  canvas->draw(renderer, mouseZoom);

  // Render selected tile at the mouse
  int yPos = mouseCoords.y - ((tileSize * mouseZoom) / 2);
  int ysrcRect = tileCol * tileSize;
  int xPos = mouseCoords.x - ((tileSize * mouseZoom) / 2);
  int xSrcRect = tileRow * tileSize;
  SDL_Rect srcRect = {ysrcRect, xSrcRect, tileSize, tileSize};
  SDL_Rect dstrect = {xPos, yPos, tileSize * mouseZoom, tileSize * mouseZoom};
  SDL_RenderCopy(renderer, selectedTileset, &srcRect, &dstrect);

  // GUI
  gui->render(imgWidth, imgHeight, tileSize, tileCol, tileRow, mouse, eventBus, selectedTileset);

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

  // Set Canvas up
  canvas->setTileSize(tileSize);
  canvas->setWidth(mapWidth * tileSize);
  canvas->setHeight(mapHeight * tileSize);
  canvas->setPosition((windowWidth / 2) - (windowWidth / 10), windowHeight / 2);

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

void Editor::onTileSelect(TileSelectEvent &event)
{
  tileCol = event.col;
  tileRow = event.row;
};
