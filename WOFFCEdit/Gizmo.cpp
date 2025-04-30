#include "Gizmo.h"

using namespace DirectX::SimpleMath;

Gizmo::Gizmo() : m_position(Vector3(0,0,0)), m_mode(Mode::Translate), m_activeAxis(Axis::None), m_dragging(false)
{
    m_gizmoSpace = GizmoSpace::World;

    m_active = true;

    posMultiplier = 1;
    rotMultiplier = 0;
    scaMultiplier = 0;

    m_mode = Mode::Translate;
}

Gizmo::~Gizmo()
{

}


void Gizmo::Update(InputCommands* input, const Matrix& view, const Matrix& proj)
{
	if (input->mouse_LB_Down)
	{
		if (!m_dragging)
			PickAxis(input, view, proj);
		else
			Drag(input, view, proj);
	}
	else 
	{
		m_dragging = false;
		m_activeAxis = Axis::None;
	}

    if (translationNum != input->translationNum)
    {
        translationNum = input->translationNum;

        switch (translationNum)
        {
        case 1:
            m_mode = Mode::Translate;
            posMultiplier = 1;
            rotMultiplier = 0;
            scaMultiplier = 0;
            break;
        case 2:
            m_mode = Mode::Rotate;
            posMultiplier = 0;
            rotMultiplier = 1;
            scaMultiplier = 0;
            break;
        case 3:
            m_mode = Mode::Scale;
            posMultiplier = 0;
            rotMultiplier = 0;
            scaMultiplier = 1;
            break;
        default:
            m_mode = Mode::Translate;
            break;
        }
    }
}


void Gizmo::Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch)
{
    if (!batch || !m_active) return;

    const float axisLength = 2.0f;
    const float shaftWidth = 0.2f; 

    Vector3 position = m_position;

    // If we're in local space, you could transform the axis directions by an orientation matrix
    Matrix transform = Matrix::Identity;
    if (m_gizmoSpace == GizmoSpace::Local)
    {
        transform = Matrix::CreateFromYawPitchRoll(m_rotation.x, m_rotation.y, m_rotation.z);
    }

    // X Axis (Red)
    Vector3 xDir = Vector3::TransformNormal(Vector3::UnitX, transform);
    Vector3 xStart = m_position;
    Vector3 xEnd = m_position + (xDir * axisLength);

    DrawAxisLine(batch, xStart, xEnd, DirectX::Colors::Red, shaftWidth);

    // Y Axis (Green)
    Vector3 yDir = Vector3::TransformNormal(Vector3::UnitY, transform);
    Vector3 yStart = m_position;
    Vector3 yEnd = m_position + (yDir * axisLength);

    DrawAxisLine(batch, yStart, yEnd, DirectX::Colors::Green, shaftWidth);

    // Z Axis (Blue)
    Vector3 zDir = Vector3::TransformNormal(Vector3::UnitZ, transform);
    Vector3 zStart = m_position;
    Vector3 zEnd = m_position + (zDir * axisLength);

    DrawAxisLine(batch, zStart, zEnd, DirectX::Colors::Blue, shaftWidth);
}

void Gizmo::DrawAxisLine(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, const Vector3& start, const Vector3& end, const DirectX::XMVECTORF32& color, float shaftWidth)
{
    Vector3 direction = end - start;
    direction.Normalize();

    // Pick an up vector that isn't parallel to direction
    Vector3 up(0.0f, 1.0f, 0.0f);

    // Check if direction and up are too close to being parallel
    if (fabs(up.Dot(direction)) > 0.99f) {
        up = Vector3(1.0f, 0.0f, 0.0f); // fallback if too parallel
    }

    // Compute the right vector
    Vector3 right = direction.Cross(up);
    right.Normalize();
    right *= (shaftWidth * 0.5f);

    // Build the 4 corners of the shaft rectangle
    Vector3 v0 = start + right;
    Vector3 v1 = start - right;
    Vector3 v2 = end + right;
    Vector3 v3 = end - right;

    DirectX::VertexPositionColor vertices[] =
    {
        { v0, color },
        { v1, color },
        { v2, color },
        { v2, color },
        { v1, color },
        { v3, color }
    };

    batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, vertices, 6);
}

void Gizmo::PickAxis(InputCommands* input, const Matrix& view, const Matrix& proj)
{
    // Create ray from mouse
    const DirectX::XMVECTOR nearSource = DirectX::XMVectorSet(input->mouse_X, input->mouse_Y, 0.0f, 1.0f);
    const DirectX::XMVECTOR farSource = DirectX::XMVectorSet(input->mouse_X, input->mouse_Y, 1.0f, 1.0f);

    DirectX::XMVECTOR nearPoint = XMVector3Unproject(nearSource, 0.0f, 0.0f, m_screenDimensions.right, m_screenDimensions.bottom, 0.0f, 1.0f, proj, view, Matrix::Identity);
    DirectX::XMVECTOR farPoint = XMVector3Unproject(farSource, 0.0f, 0.0f, m_screenDimensions.right, m_screenDimensions.bottom, 0.0f, 1.0f, proj, view, Matrix::Identity);

    Vector3 rayOrigin = Vector3(nearPoint.m128_f32[0], nearPoint.m128_f32[1], nearPoint.m128_f32[2]);
    Vector3 rayDir = Vector3(farPoint.m128_f32[0], farPoint.m128_f32[1], farPoint.m128_f32[2]) - rayOrigin;
    rayDir.Normalize();

    // Setup gizmo axes
    Vector3 origin = m_position;
    Vector3 axisXEnd = origin + Vector3(2.0f, 0, 0);
    Vector3 axisYEnd = origin + Vector3(0, 2.0f, 0);
    Vector3 axisZEnd = origin + Vector3(0, 0, 2.0f);

    // Test picking
    float minDistance = 0.1f;
    float closestHit = FLT_MAX;
    m_activeAxis = Axis::None;

    auto TestAxis = [&](const Vector3& axisEnd, Axis axisType)
        {
            float rayT, segmentT;
            float distance = RayToSegmentDistance(rayOrigin, rayDir, origin, axisEnd, rayT, segmentT);

            if (distance < minDistance && rayT >= 0.0f)
            {
                if (rayT < closestHit)
                {
                    closestHit = rayT;
                    m_activeAxis = axisType;
                }
            }
        };

    TestAxis(axisXEnd, Axis::X);
    TestAxis(axisYEnd, Axis::Y);
    TestAxis(axisZEnd, Axis::Z);

    if (m_activeAxis != Axis::None)
    {
        m_dragging = true;
        m_startMousePos = Vector2(input->mouse_X, input->mouse_Y);
        m_startObjectPos = m_position;
        m_startObjectRot = m_rotation;
        m_startObjectSca = m_scale;
    }
}

float Gizmo::RayToSegmentDistance(const Vector3& rayOrigin, const Vector3& rayDir, const Vector3& p0, const Vector3& p1, float& outRayT, float& outSegmentT)
{
    Vector3 segDir = p1 - p0;
    Vector3 w0 = rayOrigin - p0;

    float a = rayDir.Dot(rayDir);
    float b = rayDir.Dot(segDir);
    float c = segDir.Dot(segDir);
    float d = rayDir.Dot(w0);
    float e = segDir.Dot(w0);

    float denom = a * c - b * b;

    float sc, tc;

    if (fabs(denom) < 1e-6f)
    {
        sc = 0.0f;
        tc = (b > c ? d / b : e / c);
    }
    else
    {
        sc = (b * e - c * d) / denom;
        tc = (a * e - b * d) / denom;
    }

    Vector3 ptOnRay = rayOrigin + sc * rayDir;
    Vector3 ptOnSeg = p0 + tc * segDir;
    Vector3 diff = ptOnRay - ptOnSeg;

    outRayT = sc;
    outSegmentT = tc;

    return diff.Length();
}

void Gizmo::SetActive(bool active)
{
    m_active = active;
    if (!active)
    {
        m_activeAxis = Axis::None;
        m_dragging = false;
        m_position = Vector3::Zero;
    }
}

Vector2 WorldToScreen(const Vector3& worldPos, const Matrix& view, const Matrix& proj, float screenWidth, float screenHeight)
{
    Matrix viewProj = view * proj;
    Vector3 projected = Vector3::Transform(worldPos, viewProj);

    // Perform perspective divide
    float x = (projected.x + 1.0f) * 0.5f * screenWidth;
    float y = (1.0f - projected.y) * 0.5f * screenHeight;

    return Vector2(x, y);
}

void Gizmo::Drag(InputCommands* input, const Matrix& view, const Matrix& proj)
{
    Vector3 axisWorldDir;

    switch (m_activeAxis)
    {
    case Axis::X: 
        axisWorldDir = Vector3::UnitX;
        break;
    case Axis::Y: 
        axisWorldDir = Vector3::UnitY;
        break;
    case Axis::Z:
        axisWorldDir = Vector3::UnitZ;
        break;
    }

    Vector3 worldAxisDir;

    if (m_gizmoSpace == GizmoSpace::Local)
    {
        Matrix localRot = Matrix::CreateFromYawPitchRoll(m_rotation.x, m_rotation.y, m_rotation.z);
        worldAxisDir = Vector3::TransformNormal(axisWorldDir, localRot);
    }
    else
    {
        // Transform axis direction by camera/view matrix to get correct world direction
        //Matrix invView = view.Invert();
        //worldAxisDir = Vector3::TransformNormal(axisWorldDir, invView);
        worldAxisDir = axisWorldDir;
    }

    

    // Convert world axis start and end to screen space
    Vector3 axisStartWorld = m_startObjectPos;
    Vector3 axisEndWorld = m_startObjectPos + worldAxisDir;
    Vector2 axisStartScreen = WorldToScreen(axisStartWorld, view, proj, m_screenDimensions.right, m_screenDimensions.bottom);
    Vector2 axisEndScreen = WorldToScreen(axisEndWorld, view, proj, m_screenDimensions.right, m_screenDimensions.bottom);

    // Get axis screen direction
    Vector2 axisScreenDir = axisEndScreen - axisStartScreen;
    axisScreenDir.Normalize();

    // Project mouse delta onto axis direction in screen space
    Vector2 mouseDelta = Vector2(input->mouse_X, input->mouse_Y) - m_startMousePos;

    // make it feel smoother? might change for submission
    float pixelsPerUnit = 75.0f;
    float dragAmount = mouseDelta.Dot(axisScreenDir) / pixelsPerUnit;


	/*if (m_activeAxis == Axis::X)
		m_position.x = m_startObjectPos.x + dragAmount * (mouseDelta.x < 0 ? 1 : -1);
	else if (m_activeAxis == Axis::Y)
		m_position.y = m_startObjectPos.y + dragAmount * (mouseDelta.y < 0 ? 1 : -1); // Y is inverted
	else if (m_activeAxis == Axis::Z)
		m_position.z = m_startObjectPos.z + dragAmount * (mouseDelta.x > 0 ? 1 : -1);*/

    if (m_activeAxis == Axis::X)
    {
        if (posMultiplier > 0) m_position.x = (m_startObjectPos.x + dragAmount) * posMultiplier;
        if (rotMultiplier > 0) m_rotation.x = (m_startObjectRot.x + dragAmount * 5.0f) * rotMultiplier;
        if (scaMultiplier > 0) m_scale.x = (m_startObjectSca.x + dragAmount) * scaMultiplier;
    }
    else if (m_activeAxis == Axis::Y)
    {
        if (posMultiplier > 0) m_position.y = (m_startObjectPos.y + dragAmount) * posMultiplier;
        if (rotMultiplier > 0) m_rotation.y = (m_startObjectRot.y + dragAmount * 5.0f) * rotMultiplier;
        if (scaMultiplier > 0) m_scale.y = (m_startObjectSca.y + dragAmount) * scaMultiplier;
    }
    else if (m_activeAxis == Axis::Z)
    {
        if (posMultiplier > 0) m_position.z = (m_startObjectPos.z + dragAmount) * posMultiplier;
        if (rotMultiplier > 0) m_rotation.z = (m_startObjectRot.z + dragAmount * 5.0f) * rotMultiplier;
        if (scaMultiplier > 0) m_scale.z = (m_startObjectSca.z + dragAmount) * scaMultiplier;
    }
}


/*
if (m_activeAxis == Axis::X)
    {
        m_position.x = posMultiplier > 0 ? (m_startObjectPos.x + dragAmount * (mouseDelta.x < 0 ? 1 : -1)) * posMultiplier : m_position.x;
        m_rotation.x = rotMultiplier > 0 ? (m_startObjectRot.x + (dragAmount * 5) * (mouseDelta.y < 0 ? 1 : -1)) * rotMultiplier : m_rotation.x;
        m_scale.x = scaMultiplier > 0 ? (m_startObjectSca.x + dragAmount * (mouseDelta.x < 0 ? 1 : -1)) * scaMultiplier : m_scale.x;
    }
    else if (m_activeAxis == Axis::Y)
    {
        m_position.y = posMultiplier > 0 ? (m_startObjectPos.y + dragAmount * (mouseDelta.y < 0 ? 1 : -1)) * posMultiplier : m_position.y;
        m_rotation.y = rotMultiplier > 0 ? (m_startObjectRot.y + (dragAmount * 5) * (mouseDelta.x > 0 ? 1 : -1)) * rotMultiplier : m_rotation.y;
        m_scale.y = scaMultiplier > 0 ? (m_startObjectSca.y + dragAmount * (mouseDelta.y < 0 ? 1 : -1)) * scaMultiplier : m_scale.y;
    }
    else if (m_activeAxis == Axis::Z)
    {
        m_position.z = posMultiplier > 0 ? (m_startObjectPos.z + dragAmount * (mouseDelta.x > 0 ? 1 : -1)) * posMultiplier : m_position.z;
        m_rotation.z = rotMultiplier > 0 ? (m_startObjectRot.z + (dragAmount * 5) * (mouseDelta.x < 0 ? 1 : -1)) * rotMultiplier : m_rotation.z;
        m_scale.z = scaMultiplier > 0 ? (m_startObjectSca.z + dragAmount * (mouseDelta.x > 0 ? 1 : -1)) * scaMultiplier : m_scale.z;
    }
*/