#pragma once
#include "SceneObject.h"
#include "ICommand.h"
#include "TransformCommand.h"
#include <vector>

class PasteCommand : public CommandInterface
{
public:
	PasteCommand(std::vector<SceneObject>* sceneGraph, const SceneObject& object) : m_sceneGraph(sceneGraph), m_obj(nullptr), m_data(object) 
	{
		m_newID = static_cast<int>(sceneGraph->size());
	}

	void Execute() override
	{
		m_obj = new SceneObject();
		*m_obj = m_data;
		m_obj->ID = m_newID;
		m_obj->posX = m_data.posX;
		m_obj->posY = m_data.posY;
		m_obj->posZ = m_data.posZ;
		m_obj->rotX = m_data.rotX;
		m_obj->rotY = m_data.rotY;
		m_obj->rotZ = m_data.rotZ;
		m_obj->scaX = m_data.scaX;
		m_obj->scaY = m_data.scaY;
		m_obj->scaZ = m_data.scaZ;

		m_sceneGraph->push_back(*m_obj);
	}

	void Undo() override
	{
		// just pop off scenegraph?
		if(!m_sceneGraph->empty())
		m_sceneGraph->pop_back();
	}

	void Redo() override
	{
		if (m_obj)
		{
			m_sceneGraph->push_back(*m_obj);
		}
	}


private:
	std::vector<SceneObject>* m_sceneGraph;
	SceneObject* m_obj;
	SceneObject m_data;
	int m_newID;
};

