#pragma once
#include "SceneObject.h"
#include "ICommand.h"
#include <stack>

class CommandManager
{
public:
    void ExecuteCommand(CommandInterface* command)
    {
        // Execute the command
        command->Execute();
        // Add it to the undo stack
        m_undoStack.push(command);
        // Clear the redo stack since we're in a new operation
        while (!m_redoStack.empty()) m_redoStack.pop();
    }

    void Undo()
    {
        if (!m_undoStack.empty())
        {
            CommandInterface* command = m_undoStack.top();
            m_undoStack.pop();
            command->Undo();
            m_redoStack.push(command);
        }
    }

    void Redo()
    {
        if (!m_redoStack.empty())
        {
            CommandInterface* command = m_redoStack.top();
            m_redoStack.pop();
            command->Redo();
            m_undoStack.push(command);
        }
    }

private:
    std::stack<CommandInterface*> m_undoStack;
    std::stack<CommandInterface*> m_redoStack;
};

