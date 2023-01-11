#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <stack>
#include <memory>
#include <functional>

class ICommand
{
public:
  virtual void execute() = 0;
  virtual void undo() = 0;
  virtual void redo() = 0;
};

typedef std::stack<std::shared_ptr<ICommand>> CommandStack;

class CommandManager
{
public:
  CommandManager() {}
  ~CommandManager()
  {
  }

  template <typename C, typename... CArgs, typename std::enable_if<std::is_base_of<ICommand, C>::value>::type * = nullptr>
  void execute(CArgs &&...args)
  {
    flushRedo();
    std::shared_ptr<ICommand> command(new C(std::forward<CArgs>(args)...));
    command->execute();
    undoStack.push(command);
  }

  void undo()
  {
    if (undoStack.size() <= 0)
    {
      return;
    }

    undoStack.top()->undo();
    redoStack.push(undoStack.top());
    undoStack.pop();
  }

  void redo()
  {
    if (redoStack.size() <= 0)
    {
      return;
    }
    redoStack.top()->redo();
    undoStack.push(redoStack.top());
    redoStack.pop();
  }

private:
  CommandStack undoStack;
  CommandStack redoStack;

  void flushRedo()
  {
    int len = redoStack.size();
    for (int i = 0; i < len; i++)
    {
      redoStack.pop();
    }
  }

  void flushUndo()
  {
    int len = undoStack.size();
    for (int i = 0; i < len; i++)
    {
      undoStack.pop();
    }
  }
};

#endif