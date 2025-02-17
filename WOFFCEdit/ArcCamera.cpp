#include "pch.h"
#include "ArcCamera.h"

ArcCamera::ArcCamera(float moveSpeed, float rotationSpeed, DirectX::SimpleMath::Vector3 initialPosition, DirectX::SimpleMath::Vector3 initialOrientation, 
	DirectX::SimpleMath::Vector3 lookAt, DirectX::SimpleMath::Vector3 initialLookDirection, DirectX::SimpleMath::Vector3 initialRightVector) : 
    m_moveSpeed(moveSpeed),
    m_camRotRate(rotationSpeed),
    m_position(initialPosition),
    m_orientation(initialOrientation),
    m_lookAt(lookAt),
    m_lookDirection(initialLookDirection),
    m_rightVector(initialRightVector)
{
    m_upVector = DirectX::SimpleMath::Vector3::UnitY;

    m_pitch = 0.f;
    m_yaw = 0.f;

    CalculateVectors();
}

ArcCamera::~ArcCamera()
{

}

void ArcCamera::Rotate(float yawDegrees, float pitchDegrees)
{
    m_yaw += yawDegrees;
    m_pitch += pitchDegrees;

    CalculateVectors();
}

void ArcCamera::Move(const DirectX::SimpleMath::Vector3& movement)
{
    CalculateVectors();

    // Move along right and look vectors
    m_position += m_rightVector * movement.x * m_moveSpeed;   // Strafe
    m_position += m_lookDirection * movement.z * m_moveSpeed; // Forward/Back
    m_position += m_upVector * movement.y * m_moveSpeed; // Up/Down
}

void ArcCamera::CalculateVectors()
{
    // Convert angles to radians
    float yawRad = DirectX::XMConvertToRadians(m_yaw);
    float pitchRad = DirectX::XMConvertToRadians(m_pitch);

    // Spherical to Cartesian conversion
    // x = r * cos(yaw) * cos(pitch)
    // y = r * sin(pitch)
    // z = r * sin(yaw) * cos(pitch)
    m_lookDirection.x = cos(yawRad) * cos(pitchRad);
    m_lookDirection.y = sin(pitchRad);
    m_lookDirection.z = sin(yawRad) * cos(pitchRad);

    // Normalize to ensure unit vector
    m_lookDirection.Normalize();

    // Calculate right vector (cross product of look and world up)
    m_rightVector = m_lookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY);
    m_rightVector.Normalize();

    // Calculate up vector (cross product of look and right)
    m_upVector = m_lookDirection.Cross(m_rightVector);
    m_upVector.Normalize();
}

DirectX::SimpleMath::Matrix ArcCamera::GetViewMatrix() const
{
    // Calculate look-at point by adding look direction to position
    DirectX::SimpleMath::Vector3 lookAt = m_position + m_lookDirection;

    // Create view matrix using DirectX's CreateLookAt
    return DirectX::SimpleMath::Matrix::CreateLookAt(m_position, lookAt, DirectX::SimpleMath::Vector3::UnitY);
}
