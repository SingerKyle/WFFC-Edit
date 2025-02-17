#include "pch.h"
#include "ArcCamera.h"
#include "StepTimer.h"

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

void ArcCamera::Tick(InputCommands* Input)
{
    m_InputCommands = *Input;
}

void ArcCamera::Update(DX::StepTimer const& timer)
{
    // Update mouse positions 
    m_mouseYPrev = m_mouseYCurrent;
    m_mouseYCurrent = m_InputCommands.mouseDeltaY;

    m_mouseXPrev = m_mouseXCurrent;
    m_mouseXCurrent = m_InputCommands.mouseDeltaX;

    // Move camera
    MoveCam();
}

void ArcCamera::MoveCam()
{
    if (m_InputCommands.rightMousePressed)
    {
        int xDelta = m_mouseXPrev - m_mouseXCurrent;
        m_orientation.y += xDelta * m_camRotRate;

        int yDelta = m_mouseYPrev - m_mouseYCurrent;
        m_orientation.x += yDelta * m_camRotRate;
    }

    CalculateVectors();

    // get input
    if (m_InputCommands.forward)
    {
        m_position += m_lookDirection * m_moveSpeed;
    }

    if (m_InputCommands.back)
    {
        m_position -= m_lookDirection * m_moveSpeed;
    }

    if (m_InputCommands.right)
    {
        m_position += m_rightVector * m_moveSpeed;
    }

    if (m_InputCommands.left)
    {
        m_position -= m_rightVector * m_moveSpeed;
    }

    if (m_InputCommands.up)
    {
        m_position -= m_upVector * m_moveSpeed;
    }

    if (m_InputCommands.down)
    {
        m_position += m_upVector * m_moveSpeed;
    }





}

void ArcCamera::CalculateVectors()
{
    // Create look direction from orientation
    m_lookDirection.x = sin((m_orientation.y) * 3.1415 / 180) * cos((m_orientation.x) * 3.1415 / 180);
    m_lookDirection.y = sin((m_orientation.x) * 3.1415 / 180);
    m_lookDirection.z = cos((m_orientation.y) * 3.1415 / 180) * cos((m_orientation.x) * 3.1415 / 180);
    m_lookDirection.Normalize(); // Normalise values


    // Create right and up vectors from look Direction
    m_lookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_rightVector);
    m_lookDirection.Cross(m_rightVector, m_upVector);
}

DirectX::SimpleMath::Matrix ArcCamera::GetViewMatrix() const
{
    // Calculate look-at point by adding look direction to position
    DirectX::SimpleMath::Vector3 lookAt = m_position + m_lookDirection;

    // Create view matrix using DirectX's CreateLookAt
    return DirectX::SimpleMath::Matrix::CreateLookAt(m_position, lookAt, DirectX::SimpleMath::Vector3::UnitY);
}
