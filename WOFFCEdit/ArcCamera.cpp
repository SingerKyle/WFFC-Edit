#include "pch.h"
#include "ArcCamera.h"
#include "StepTimer.h"
#include "SimpleMath.h"

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

    m_distance = 10.0f;

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
    float dt = timer.GetElapsedSeconds();

    // Update mouse positions 
    m_mouseYPrev = m_mouseYCurrent;
    m_mouseYCurrent = m_InputCommands.mouseDeltaY;

    m_mouseXPrev = m_mouseXCurrent;
    m_mouseXCurrent = m_InputCommands.mouseDeltaX;

    // Move camera
    MoveCam(dt);

    // Calculate look-at point by adding look direction to position
    DirectX::SimpleMath::Vector3 lookAt = m_position + m_lookDirection;

    // Create view matrix using DirectX's CreateLookAt
    m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, lookAt, DirectX::SimpleMath::Vector3::UnitY);
}

void ArcCamera::ArcCamUpdate(DirectX::SimpleMath::Vector3 objectPos, float scale, DX::StepTimer const& timer)
{
    float dt = timer.GetElapsedSeconds();

    m_mouseYPrev = m_mouseYCurrent;
    m_mouseYCurrent = m_InputCommands.mouseDeltaY;

    m_mouseXPrev = m_mouseXCurrent;
    m_mouseXCurrent = m_InputCommands.mouseDeltaX;

    int xDelta = m_mouseXCurrent - m_mouseXPrev;
    int yDelta = m_mouseYCurrent - m_mouseYPrev;


    m_yaw -= xDelta * m_camRotRate * dt;
    m_pitch += yDelta * m_camRotRate * dt;

    m_orientation.x = m_pitch;
    m_orientation.y = m_yaw;

    // Calculate the camera position based on spherical coordinates
    float x = m_distance * sinf(m_pitch) * cosf(m_yaw);
    float y = m_distance * cosf(m_pitch);
    float z = m_distance * sinf(m_pitch) * sinf(m_yaw);

    m_position = objectPos + DirectX::SimpleMath::Vector3(x, y, z);

    m_lookDirection = DirectX::SimpleMath::Vector3(objectPos - m_position);
    m_lookDirection.Normalize();

    // Make the camera look at the object
    m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, objectPos, DirectX::SimpleMath::Vector3::Up);

}

void ArcCamera::FocusCamOnObject(DirectX::SimpleMath::Vector3 objectPos, float scale, DX::StepTimer const& timer)
{
    float dt = timer.GetElapsedSeconds();

    if (scale <= 0.0f) {
        return;
    }


    //define necessary variables
    float desired_distance_from_object = scale * 5.f;
    DirectX::SimpleMath::Vector3 desired_camera_location = objectPos - m_lookDirection * desired_distance_from_object;
    DirectX::SimpleMath::Vector3 total_todo_movement_vector = (desired_camera_location - m_position);
    float current_distance_from_DCL = total_todo_movement_vector.Length();

    float focus_speed = current_distance_from_DCL + 2.0f;
    if (focus_speed < 1.0f) { focus_speed = 1.0f; }


    float immediate_set_threshold = 0.05f;

    focus_speed *= current_distance_from_DCL;

    total_todo_movement_vector.Normalize();
    if (current_distance_from_DCL > immediate_set_threshold) {
        m_position += dt * total_todo_movement_vector * 5.0f * focus_speed;
    }
    else {
        m_position = desired_camera_location;
    }
}

void ArcCamera::MoveCam(float dt)
{
    if (m_InputCommands.rightMousePressed)
    {
        int xDelta = m_mouseXPrev - m_mouseXCurrent;
        m_orientation.y += xDelta * m_camRotRate * dt;

        int yDelta = m_mouseYPrev - m_mouseYCurrent;
        m_orientation.x += yDelta * m_camRotRate * dt;
        m_orientation.x = std::min(m_orientation.x, 89.f);
        m_orientation.x = std::max(m_orientation.x, -89.f);
    }

    CalculateVectors();

    // get input
    if (m_InputCommands.forward)
    {
        m_position += m_lookDirection * m_moveSpeed * dt;
    }

    if (m_InputCommands.back)
    {
        m_position -= m_lookDirection * m_moveSpeed * dt;
    }

    if (m_InputCommands.right)
    {
        m_position += m_rightVector * m_moveSpeed * dt;
    }

    if (m_InputCommands.left)
    {
        m_position -= m_rightVector * m_moveSpeed * dt;
    }

    if (m_InputCommands.up)
    {
        m_position -= m_upVector * m_moveSpeed * dt;
    }

    if (m_InputCommands.down)
    {
        m_position += m_upVector * m_moveSpeed * dt;
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
    if (m_InputCommands.leftMouseHeld)
    {
        return m_viewMatrix;
    }
    else
    {
        return m_viewMatrix;
    }
    
}
