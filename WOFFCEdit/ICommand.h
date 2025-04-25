#pragma once

class CommandInterface
{
public:
    CommandInterface();
    virtual ~CommandInterface();
    virtual void Execute() = 0;   // Apply the change (execute action)
    virtual void Undo() = 0;      // Revert the change (undo action)
    virtual void Redo() = 0;      // Reapply the change (redo action)
};