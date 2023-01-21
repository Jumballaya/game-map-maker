#include <fstream>

#include <SDL2/SDL_image.h>
#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_sdl.h"
#include "../../libs/imgui/imgui_impl_sdl.h"
#include "../libs/pfd/pfd.h"

#include "Editor.h"
#include "../logger/Logger.h"
#include "../tilemap/TileMapLoader.h"

int Editor::windowWidth;
int Editor::windowHeight;

Editor::Editor()
{
  Logger::Log("[Editor] constructor called");
  commandManager = std::make_unique<CommandManager>();
  assetStore = std::make_unique<AssetStore>();
  eventBus = std::make_unique<EventBus>();
  tileMap = std::make_shared<TileMap>(glm::vec2(0), 0, 1.0);
  canvas = std::make_unique<Canvas>(0, 0, 0);
  mouse = std::make_unique<Mouse>(0, 0);
  gui = std::make_unique<EditorGUI>();

  canvas->setZoom(mouse->getZoom());
  tileMap->setZoom(mouse->getZoom());

  rootPath = "";
}

Editor::~Editor()
{
  Logger::Log("[Editor] destructor called");
}

void Editor::initialize(const std::string &rootPath)
{
  this->rootPath = rootPath;
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

  this->windowWidth = dm.w - 120;
  this->windowHeight = dm.h;
  state.windowHeight = dm.h;
  state.windowWidth = dm.w - 120;
  this->window = SDL_CreateWindow(
      "Tile map editor",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      windowWidth,
      windowHeight,
      SDL_WINDOW_RESIZABLE);

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
  state.running = true;
}

void Editor::setup()
{
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
  eventBus->subscribe<RunLUAEvent>(this, &Editor::onRunLua);
  eventBus->subscribe<TileToolSelectEvent>(this, &Editor::onTileToolSelect);
  eventBus->subscribe<TileMapLayerSelectEvent>(this, &Editor::onTileMapLayerSelect);
  eventBus->subscribe<TileMapLayerNameChangeEvent>(this, &Editor::onTileMapLayerNameChange);
  eventBus->subscribe<TileSetSelectEvent>(this, &Editor::onTileSetSelect);
  eventBus->subscribe<OpenTileMapEvent>(this, &Editor::onOpenTileMap);
  eventBus->subscribe<CreateNewTileMapEvent>(this, &Editor::onCreateNewTileMap);
  eventBus->subscribe<AddTileSetEvent>(this, &Editor::onAddTileSet);
  eventBus->subscribe<SaveTileMapEvent>(this, &Editor::onSaveTileMap);

  // Setup mouse
  int mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  mouse->move(mouseX, mouseY);
  mouse->move(mouseX, mouseY);

  state.hoveringCanvas = false;
  state.hoveringCoords.x = 0;
  state.hoveringCoords.y = 0;

  // Setup LUA
  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
}

void Editor::run()
{
  setup();
  while (state.running)
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
      state.running = false;
      break;

    case SDL_KEYDOWN:
      eventBus->emit<KeyPressEvent>(sdlEvent.key.keysym.sym);
      switch (sdlEvent.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        state.running = false;
        break;

      case SDLK_z:
        commandManager->undo();
        break;
      case SDLK_y:
        commandManager->redo();
        break;

      // TEMP
      case SDLK_q:
        state.selectedLayer = (state.selectedLayer + 1) % tileMap->layerCount();
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

      // Canvas and TileMap have a copy of the mouse zoom
      // It ended up being much easier than always passing
      // zoom into the canvas/tilemap methods
      // Maybe this means that mouse should be holding the
      // zoom value at all? Or maybe it should be a singleton
      // with static methods for getting pos, zoom ,etc
      canvas->setZoom(mouse->getZoom());
      tileMap->setZoom(mouse->getZoom());
      break;
    }
  }
}

void Editor::update()
{
  Uint32 ticks = SDL_GetTicks();
  int timeToWait = MILLISECS_PER_FRAME - (ticks - state.millisPreviousFrame);
  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
  {
    SDL_Delay(timeToWait);
  }
  double deltaTime = (ticks - state.millisPreviousFrame) / 1000.0f;
  ImGuiIO &io = ImGui::GetIO();
  io.DeltaTime = deltaTime;

  if (io.DeltaTime <= 0.0f)
    io.DeltaTime = 0.00001f;

  // Update Mouse Stuff
  canvas->setZoom(mouse->getZoom());
  tileMap->setZoom(mouse->getZoom());

  if (mouse->isDragging())
  {
    auto offset = mouse->getDragOffset();
    canvas->offset(offset.x, offset.y);
  }
  int mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  mouse->move(mouseX, mouseY);

  if (mouse->isClicked(1) && mouse->isHovering(canvas->getRect()) && ticks - state.lastClick > 15)
  {
    state.lastClick = SDL_GetTicks();
    switch (state.selectedTileTool)
    {
    case TileTool::PlaceTile:
      placeTile();
      break;
    case TileTool::EraseTile:
      eraseTile();
      break;
    case TileTool::FloodFill:
      floodFill();
      break;
    }
  }

  state.hoveringCanvas = mouse->isHovering(canvas->getRect());
  if (state.hoveringCanvas)
  {
    glm::vec2 coords = canvas->getTileCoords(mouse->getPosition());
    state.hoveringCoords.x = coords.x;
    state.hoveringCoords.y = coords.y;
  }

  // End Mouse Stuff
  state.millisPreviousFrame = ticks;
}

void Editor::renderCanvasCursor()
{
  auto selectedTileset = assetStore->getTileset(state.selectedTileset);
  if (selectedTileset == NULL)
  {
    return;
  }
  int tileSize = selectedTileset->tileSize;
  if (state.selectedTileTool == TileTool::EraseTile)
  {
    return;
  }
  glm::vec2 mouseCoords = mouse->getPosition();
  double mouseZoom = mouse->getZoom();
  // Render selected tile at the mouse
  glm::vec2 coords = canvas->getTileCoords(mouseCoords);
  if (coords.x >= 0 && coords.y >= 0)
  {
    int xPos = (canvas->getXPosition() + (coords.x * tileSize * mouseZoom));
    int xSrcRect = state.selectedTileData.x * tileSize;
    int yPos = (canvas->getYPosition() + (coords.y * tileSize * mouseZoom));
    int ySrcRect = state.selectedTileData.y * tileSize;
    SDL_Rect srcRect = {xSrcRect, ySrcRect, tileSize, tileSize};
    SDL_Rect dstrect = {xPos, yPos, static_cast<int>(tileSize * mouseZoom), static_cast<int>(tileSize * mouseZoom)};
    SDL_RenderCopy(renderer, selectedTileset->getTexture(), &srcRect, &dstrect);
  }
}

void Editor::render()
{
  if (state.dialogOpen)
  {
    return;
  }
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // Render phase
  // Background
  SDL_Rect rect = {0, 0, windowWidth, windowHeight};
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderFillRect(renderer, &rect);

  // Canvas
  canvas->draw(renderer);
  tileMap->render(renderer, assetStore, glm::vec2(canvas->getXPosition(), canvas->getYPosition()));

  // Render tile to place at the mouse cursor
  // Only render if hovering over the canvas
  if (mouse->isHovering(canvas->getRect()))
  {
    renderCanvasCursor();
  }

  // GUI
  gui->render(state, mouse, eventBus, assetStore, tileMap);

  SDL_RenderPresent(renderer);
}

void Editor::loadMap(std::string filePath)
{
  // Set Tilemap up
  tileMap = TileMapLoader::loadTileMap(filePath, assetStore, renderer);

  // Set Canvas up
  canvas->setTileSize(tileMap->tileSize);
  canvas->setWidth(tileMap->width);
  canvas->setHeight(tileMap->height);
  canvas->setPosition((windowWidth / 2) - (windowWidth / 10), windowHeight / 2);

  if (tileMap->tilesets.size() > 0)
  {
    state.selectedTileset = tileMap->tilesets[0];
    state.selectedTileData.x = 0;
    state.selectedTileData.y = 0;
  }
  else
  {
    state.selectedTileset = "";
    state.selectedTileData.x = -1;
    state.selectedTileData.y = -1;
  }
}

void Editor::placeTile()
{
  // 1. Get tile location from canvas
  glm::vec2 tileCoords = canvas->getTileCoords(mouse->getPosition());

  // 2. Insert the currently selected tile (col/row) into the tilemap
  if (tileCoords.x >= 0 && tileCoords.y >= 0)
  {
    Tile t = tileMap->getTile(state.selectedLayer, tileCoords);
    if (!(t.srcRow == state.selectedTileData.y && t.srcCol == state.selectedTileData.x))
    {
      commandManager->execute<PlaceTileCommand>(tileMap, state.selectedLayer, tileCoords, state.selectedTileData);
    }
  }
}

void Editor::eraseTile()
{
  glm::vec2 tileCoords = canvas->getTileCoords(mouse->getPosition());
  if (tileCoords.x >= 0 && tileCoords.y >= 0)
  {
    Tile t = tileMap->getTile(state.selectedLayer, tileCoords);
    if (!(t.srcRow == -1 && t.srcCol == -1))
    {
      commandManager->execute<PlaceTileCommand>(tileMap, state.selectedLayer, tileCoords, glm::vec2(-1, -1));
    }
  }
}

void Editor::floodFill()
{
  glm::vec2 tileCoords = canvas->getTileCoords(mouse->getPosition());
  if (tileCoords.x >= 0 && tileCoords.y >= 0)
  {
    tileMap->floodFill(state.selectedLayer, tileCoords, state.selectedTileData);
  }
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
  state.selectedTileData.x = event.col;
  state.selectedTileData.y = event.row;
};

void Editor::onTileMapLayerSelect(TileMapLayerSelectEvent &event)
{
  state.selectedLayer = event.layerId;
};

void Editor::onTileMapLayerNameChange(TileMapLayerNameChangeEvent &event)
{
  tileMap->renameLayer(event.layerId, event.newName);
};

void Editor::onRunLua(RunLUAEvent &event)
{
  lua.script(event.code);

  sol::optional<sol::function> hasFunc = lua["run"];
  if (hasFunc != sol::nullopt)
  {
    sol::function func = lua["run"];
    func();
  }
}

void Editor::onTileToolSelect(TileToolSelectEvent &event)
{
  state.selectedTileTool = event.selection;
}

void Editor::onTileSetSelect(TileSetSelectEvent &event)
{
  state.selectedTileset = event.tileset;
}

void Editor::onOpenTileMap(OpenTileMapEvent &event)
{
  state.dialogOpen = true;
  auto f = pfd::open_file("Choose files to read", rootPath,
                          {"Map File (.map.xml .xml)", "*.xml *.map.xml"});
  state.dialogOpen = false;
  auto res = f.result();

  if (res.size() > 0)
  {
    Logger::Log("[Editor] Opening map file: " + f.result()[0]);
    loadMap(f.result()[0]);
  }
}

void Editor::onCreateNewTileMap(CreateNewTileMapEvent &event)
{
  tileMap = std::make_shared<TileMap>(event.sizeInTiles, event.tileSize, mouse->getZoom());
  canvas->setTileSize(tileMap->tileSize);
  canvas->setWidth(tileMap->width);
  canvas->setHeight(tileMap->height);

  if (tileMap->tilesets.size() > 0)
  {
    state.selectedTileset = tileMap->tilesets[0];
    state.selectedTileData.x = 0;
    state.selectedTileData.y = 0;
  }
  else
  {
    state.selectedTileset = "";
    state.selectedTileData.x = -1;
    state.selectedTileData.y = -1;
  }
}

void Editor::onAddTileSet(AddTileSetEvent &event)
{
  glm::vec2 sizeInPixels = {event.sizeInTiles.x * event.tileSize, event.sizeInTiles.y * event.tileSize};
  assetStore->addTileset(renderer, event.assetId, event.filePath, event.sizeInTiles, sizeInPixels, event.tileSize);
  tileMap->tilesets.push_back(event.assetId);
  state.selectedTileset = event.assetId;
  state.selectedTileData = glm::vec2(0, 0);

  if (tileMap->layerCount() == 0)
  {
    tileMap->createLayer("layer1", event.assetId, event.sizeInTiles.x);
  }
}

void Editor::onSaveTileMap(SaveTileMapEvent &event)
{
  Logger::Log("[Editor] Saving tilemap to " + event.filepath);
  TileMapLoader::saveTileMap(event.filepath, tileMap, assetStore);
};
