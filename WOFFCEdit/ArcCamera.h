#pragma once

#include "SimpleMath.h"

class ArcCamera
{
public:
	ArcCamera(float, float, DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3);
	~ArcCamera();

	// Camera Controls
	void Rotate(float yawDegrees, float pitchDegrees);
	void Move(const DirectX::SimpleMath::Vector3& movement);
	// Matrices

	// Updates
	void CalculateVectors();

	// Getters & Setters
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }

	DirectX::SimpleMath::Vector3 GetOrientation() const { return m_orientation; }
	DirectX::SimpleMath::Vector3 GetLookAt() const { return m_lookAt; }
	DirectX::SimpleMath::Vector3 GetLookDirection() const { return m_lookDirection; }
	DirectX::SimpleMath::Vector3 GetRightVector() const { return m_rightVector; }

	// Generate view matrix for rendering
	DirectX::SimpleMath::Matrix GetViewMatrix() const;
private:
	//Camera Position Variable
	DirectX::SimpleMath::Vector3			m_position;

	// vectors
	DirectX::SimpleMath::Vector3			m_orientation;
	DirectX::SimpleMath::Vector3			m_lookAt;
	DirectX::SimpleMath::Vector3			m_lookDirection;
	DirectX::SimpleMath::Vector3			m_rightVector;
	DirectX::SimpleMath::Vector3			m_orientationAngles;
	DirectX::SimpleMath::Vector3			m_upVector;

	// Movement speed
	float									m_moveSpeed;
	// rotate speed
	float									m_camRotRate;

	// Rotation around Y-axis (longitude)
	float									m_yaw;    
	// Rotation around X-axis (latitude)
	float									m_pitch; 
protected:

};

