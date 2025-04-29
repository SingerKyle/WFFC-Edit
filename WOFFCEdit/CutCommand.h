#pragma once
#include "SceneObject.h"
#include "ICommand.h"

class CutCommand : public CommandInterface
{
public:
	CutCommand(std::vector<SceneObject>* sceneGraph, int index) : m_sceneGraph(sceneGraph), m_index(index), m_cutObject(nullptr) {}

	void Execute() override
	{
		if (m_index >= 0 && m_index < m_sceneGraph->size())
		{
			// Copy the object
			m_cutObject = new SceneObject((*m_sceneGraph)[m_index]);

			// Remove it from the scene graph
			m_sceneGraph->erase(m_sceneGraph->begin() + m_index);
		}
	}

	void Undo() override
	{
		if (m_cutObject)
		{
			// add the cut object back into the scene
			m_sceneGraph->insert(m_sceneGraph->begin() + m_index, *m_cutObject);
		}
	}

	void Redo() override
	{
		if (m_index >= 0 && m_index < m_sceneGraph->size())
		{
			m_sceneGraph->erase(m_sceneGraph->begin() + m_index);
		}
	}

	SceneObject* GetCutObject() const { return m_cutObject; }

private:
	std::vector<SceneObject>* m_sceneGraph;
	int m_index;
	SceneObject* m_cutObject;
};

