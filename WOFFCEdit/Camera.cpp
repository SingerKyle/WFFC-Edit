#include "pch.h"
#include "Camera.h"
#include "StepTimer.h"
#include "SimpleMath.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Update(DX::StepTimer const& timer, InputCommands* input)
{
    m_InputCommands = *input;

    float dt = timer.GetElapsedSeconds();

    // Update mouse positions 
    m_mouseYPrev = m_mouseYCurrent;
    m_mouseYCurrent = m_InputCommands.mouse_Y;

    m_mouseXPrev = m_mouseXCurrent;
    m_mouseXCurrent = m_InputCommands.mouse_X;

    // Move camera
    MoveCam(dt);

    // Calculate look-at point by adding look direction to position
    DirectX::SimpleMath::Vector3 lookAt = m_position + m_lookAt;

    // Create view matrix using DirectX's CreateLookAt
    m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, lookAt, DirectX::SimpleMath::Vector3::UnitY);
}

DirectX::SimpleMath::Vector3 Camera::GetPosition() const
{
    return m_position;
}

DirectX::SimpleMath::Matrix Camera::GetViewMatrix() const
{
    return m_view;
}

void Camera::MoveCam(float dt)
{
    if (m_InputCommands.mouse_RB_Down)
    {
        int xDelta = m_mouseXPrev - m_mouseXCurrent;
        int yDelta = m_mouseYPrev - m_mouseYCurrent;

        if (m_invertControls)
        {
            xDelta = -xDelta;
            yDelta = -yDelta;
        }

        m_orientation.y += xDelta * m_camRotRate * dt;

        m_orientation.x += yDelta * m_camRotRate * dt;
        m_orientation.x = std::min(m_orientation.x, 89.f);
        m_orientation.x = std::max(m_orientation.x, -89.f);


    }

    CalculateVectors();

    // get input
    if (m_InputCommands.forward)
    {
        m_position += m_lookAt * m_moveSpeed * dt;
    }

    if (m_InputCommands.back)
    {
        m_position -= m_lookAt * m_moveSpeed * dt;
    }

    if (m_InputCommands.right)
    {
        m_position += m_right * m_moveSpeed * dt;
    }

    if (m_InputCommands.left)
    {
        m_position -= m_right * m_moveSpeed * dt;
    }

    if (m_InputCommands.up)
    {
        m_position -= m_up * m_moveSpeed * dt;
    }

    if (m_InputCommands.down)
    {
        m_position += m_up * m_moveSpeed * dt;
    }
}

void Camera::CalculateVectors()
{
    // Create look direction from orientation
    m_lookAt.x = sin((m_orientation.y) * 3.1415 / 180) * cos((m_orientation.x) * 3.1415 / 180);
    m_lookAt.y = sin((m_orientation.x) * 3.1415 / 180);
    m_lookAt.z = cos((m_orientation.y) * 3.1415 / 180) * cos((m_orientation.x) * 3.1415 / 180);
    m_lookAt.Normalize(); // Normalise values


    // Create right and up vectors from look Direction
    m_lookAt.Cross(DirectX::SimpleMath::Vector3::UnitY, m_right);
    m_lookAt.Cross(m_right, m_up);
}

void Camera::UpdateCamValues(float moveSpeed, float camRotRate, bool invertControls)
{
    m_moveSpeed = moveSpeed;
    m_camRotRate = camRotRate;
    m_invertControls = invertControls;
}
