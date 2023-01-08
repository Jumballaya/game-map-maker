#ifndef MOUSE_H
#define MOUSE_H

#include "../../libs/glm/glm.hpp"

class Mouse
{
public:
  Mouse(int x, int y)
  {
    position = glm::vec2(x, y);
    lastPos = glm::vec2(x, y);
    zoom = 1;
  };
  ~Mouse() = default;

  bool isClicked(int mouseButton)
  {
    switch (mouseButton)
    {
    case 1:
      return _clickedLeft;
      break;
    case 2:
      return _clickedRight;
      break;
    case 3:
      return _clickedMiddle;
      break;
    }
    return false;
  };
  bool isDragging()
  {
    bool isClicked = _clickedMiddle;
    bool isMoving = position.x != lastPos.x || position.y != lastPos.y;
    if (isClicked && isMoving)
    {
      return true;
    }
    return false;
  };

  glm::vec2 getPosition()
  {
    return position;
  }

  glm::vec2 getDragOffset()
  {
    return glm::vec2(
        position.x - lastPos.x,
        position.y - lastPos.y);
  };

  void move(int x, int y)
  {
    lastPos.x = position.x;
    lastPos.y = position.y;
    position.x = x;
    position.y = y;
  }

  void click(int mouseButton)
  {
    switch (mouseButton)
    {
    case 1:
      _clickedLeft = true;
      break;
    case 2:
      _clickedRight = true;
      break;
    case 3:
      _clickedMiddle = true;
      break;
    }
  };

  void stopClick(int mouseButton)
  {
    switch (mouseButton)
    {
    case 1:
      _clickedLeft = false;
      break;
    case 2:
      _clickedRight = false;
      break;
    case 3:
      _clickedMiddle = false;
      break;
    }
  };

  void zoomPlus()
  {
    zoom *= 1.1;
  }
  void zoomMinus()
  {
    zoom /= 1.1;
  }
  double getZoom()
  {
    return zoom;
  }

private:
  bool _clickedLeft;
  bool _clickedRight;
  bool _clickedMiddle;

  double zoom;

  glm::vec2 position;
  glm::vec2 lastPos;
};

#endif