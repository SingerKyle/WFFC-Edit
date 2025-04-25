#pragma once
#include "InputCommands.h"

#include <d3d11.h>
#include "SimpleMath.h"
#include <Mouse.h>
#include <GeometricPrimitive.h>
#include <VertexTypes.h>
#include <PrimitiveBatch.h>


using namespace DirectX::SimpleMath;

class Gizmo
{
public:
	enum class Mode { 
		Translate, 
		Rotate, 
		Scale };

	enum class Axis { 
		None, 
		X, 
		Y, 
		Z };

	enum class GizmoSpace
	{
		Local,
		World };

	Gizmo();
	~Gizmo();

	inline void SetPosition(const Vector3& pos) { m_position = pos; };
	inline void SetRotation(const Vector3& rot) { m_rotation = rot; };
	inline void SetScale(const Vector3& sca) { m_scale = sca; };
	inline Vector3 GetPosition() { return m_position; };
	inline Vector3 GetRotation() { return m_rotation; };
	inline Vector3 GetScale() { return m_scale; };
	inline void SetMode(Mode m) { m_mode = m; };
	inline void SetScreenDimensions(int width, int height) { m_screenDimensions.right = width; m_screenDimensions.bottom = height; }
	inline void SetScreenDimensions(RECT Dimensions) { m_screenDimensions = Dimensions; }
	void Update(InputCommands* input, const Matrix& view, const Matrix& proj);
	void Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch);

	void DrawAxisLine(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, const Vector3& start, const Vector3& end, const DirectX::XMVECTORF32& color, float shaftWidth);

	float RayToSegmentDistance(const Vector3& rayOrigin, const Vector3& rayDir, const Vector3& p0, const Vector3& p1, float& outRayT, float& outSegmentT);
	bool IsActive() const { return m_activeAxis != Axis::None; }
	Axis GetActiveAxis() const { return m_activeAxis; }

	void SetActive(bool active);

	inline bool GetDragging() { return m_dragging; }

private:
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;
	Mode m_mode;
	Axis m_activeAxis;
	GizmoSpace m_gizmoSpace;
	bool m_dragging;
	bool m_active;
	Vector2 m_startMousePos;
	Vector3 m_startObjectPos;
	Vector3 m_startObjectRot;
	Vector3 m_startObjectSca;
	RECT	m_screenDimensions;
	int translationNum;

	// multipliers for movement
	int posMultiplier;
	int rotMultiplier;
	int scaMultiplier;

	void PickAxis(InputCommands* input, const Matrix& view, const Matrix& proj);
	void Drag(InputCommands* input, const Matrix& view, const Matrix& proj);
};

