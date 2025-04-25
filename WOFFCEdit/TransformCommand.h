#include "SceneObject.h"
#include "ICommand.h"

struct TransformData
{
    float posX, posY, posZ;
    float rotX, rotY, rotZ;
    float scaX, scaY, scaZ;
};

class TransformCommand : public CommandInterface 
{
public:
    TransformCommand(SceneObject* obj, const TransformData& oldData, const TransformData& newData)
        : m_obj(obj), m_oldData(oldData), m_newData(newData) {
    }

    void Execute() override {
        // Apply new data to object
        ApplyTransformData(m_newData);
    }

    void Undo() override {
        // Revert object to old data
        ApplyTransformData(m_oldData);
    }

    void Redo() override {
        // Reapply new data (same as Execute)
        ApplyTransformData(m_newData);
    }

private:
    void ApplyTransformData(const TransformData& data) {
        m_obj->posX = data.posX;
        m_obj->posY = data.posY;
        m_obj->posZ = data.posZ;
        m_obj->rotX = data.rotX;
        m_obj->rotY = data.rotY;
        m_obj->rotZ = data.rotZ;
        m_obj->scaX = data.scaX;
        m_obj->scaY = data.scaY;
        m_obj->scaZ = data.scaZ;

    }

    SceneObject* m_obj;
    TransformData m_oldData;
    TransformData m_newData;
};