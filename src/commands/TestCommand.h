#ifndef TESTTILECOMMAND_H
#define TESTTILECOMMAND_H

#include <string>

#include "./CommandManager.h"
#include "../logger/Logger.h"

class TestCommand : public ICommand
{
public:
  TestCommand(int saved)
  {
    this->saved = saved;
  }

  void execute()
  {
    Logger::Log("Executed the command " + std::to_string(saved));
  }

  void undo()
  {
    Logger::Log("Undid that command " + std::to_string(saved));
  }

  void redo()
  {
    Logger::Log("Redid the original command " + std::to_string(saved));
  }

private:
  int saved;
};

#endif