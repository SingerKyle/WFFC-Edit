#pragma once
#include "SceneObject.h"
#include "ICommand.h"
#include "TransformCommand.h"
#include <vector>


class DeleteCommand :public CommandInterface
{
public:
	DeleteCommand(std::vector<SceneObject>* sceneGraph, int index) : m_sceneGraph(sceneGraph), m_index(index), m_deletedObject(nullptr) {}

	void Execute() override
	{
		if (m_index >= 0 && m_index < m_sceneGraph->size())
		{
			m_deletedObject = new SceneObject((*m_sceneGraph)[m_index]);
			m_sceneGraph->erase(m_sceneGraph->begin() + m_index);
		}
	}

	void Undo() override
	{
		if (m_deletedObject)
		{
			// Insert the object back at the same index if possible
			if (m_index >= 0 && m_index <= m_sceneGraph->size())
			{
				m_sceneGraph->insert(m_sceneGraph->begin() + m_index, *m_deletedObject);
			}
			else
			{
				// If index is out of range, add it at the end
				m_sceneGraph->push_back(*m_deletedObject);
			}
		}
	}

	void Redo() override
	{
		if (m_index >= 0 && m_index < m_sceneGraph->size())
		{
			m_sceneGraph->erase(m_sceneGraph->begin() + m_index);
		}
	}

private:
	std::vector<SceneObject>* m_sceneGraph;
	int m_index;
	SceneObject* m_deletedObject;
};

