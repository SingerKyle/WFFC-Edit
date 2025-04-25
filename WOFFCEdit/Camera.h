#pragma once

#include <d3d11.h>
#include "SimpleMath.h"
#include "InputCommands.h"
#include "StepTimer.h"
#include <SimpleMath.h>

namespace DX
{
	class StepTimer;
}

class Camera
{
public:
	Camera();
	~Camera();

	void Update(DX::StepTimer const& timer, InputCommands* input);

	DirectX::SimpleMath::Vector3 GetPosition() const;

	DirectX::SimpleMath::Matrix GetViewMatrix() const;

	inline float GetMoveSpeed() { return m_moveSpeed; }
	inline float GetRotRate() { return m_camRotRate; }
	inline bool GetIsInverted() { return m_invertControls; }

	void MoveCam(float dt);

	void CalculateVectors();

	void UpdateCamValues(float moveSpeed, float camRotRate, bool invertControls);

private:
	DirectX::SimpleMath::Vector3			m_scale;
	DirectX::SimpleMath::Vector3			m_rotation;
	DirectX::SimpleMath::Vector3			m_position{ 0.f, 3.7f, -3.5f };

	DirectX::SimpleMath::Vector3			m_forward;
	DirectX::SimpleMath::Vector3			m_lookAt{0,0,1};
	DirectX::SimpleMath::Vector3			m_orientation;
	DirectX::SimpleMath::Vector3			m_right;
	DirectX::SimpleMath::Vector3			m_up;
	DirectX::SimpleMath::Matrix				m_view;

	float									m_moveSpeed = 125.f;
	float									m_camRotRate = 10.f;
	bool									m_invertControls = false;

	// mouse positions
	int										m_mouseXCurrent;
	int										m_mouseXPrev;
	int										m_mouseYCurrent;
	int										m_mouseYPrev;

	// Input Commands
	InputCommands							m_InputCommands;
};

