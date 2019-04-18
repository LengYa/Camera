#define STRICT

#include "Camera.h"

Camera::Camera()
{
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Proj);
	D3DXMatrixIdentity(&m_ViewProj);

	m_Pos = D3DXVECTOR3(0.0F, 0.0F, 0.0F);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// Client should adjust to a value that makes sense for application's
	// unit scale, and the object the camera is attached to--e.g., car, jet,
	// human walking, etc.		   
	m_Speed = 50.0f;
}

Camera::~Camera()
{

}

const D3DXMATRIX& Camera::view() const
{
	return m_View;
}

const D3DXMATRIX& Camera::proj() const
{
	return m_Proj;
}

const D3DXMATRIX& Camera::viewproj() const
{
	return m_ViewProj;
}

const D3DXVECTOR3& Camera::right() const
{
	return m_Right;
}

const D3DXVECTOR3& Camera::up() const
{
	return m_Up;
}

const D3DXVECTOR3& Camera::look() const
{
	return m_Look;
}

D3DXVECTOR3& Camera::pos()
{
	return m_Pos;
}

D3DXMATRIX & Camera::SetProj(D3DXMATRIX &p)
{
	// TODO: 在此处插入 return 语句
	m_Proj = p;
	return m_Proj;
}

void Camera::LookAt(D3DXVECTOR3& pos, D3DXVECTOR3& target, D3DXVECTOR3& up)
{
	D3DXVECTOR3 L = target - pos;
	D3DXVec3Normalize(&L, &L);

	D3DXVECTOR3 R;
	D3DXVec3Cross(&R, &up, &L);
	D3DXVec3Normalize(&R, &R);

	D3DXVECTOR3 U;
	D3DXVec3Cross(&U, &L, &R);
	D3DXVec3Normalize(&U, &U);

	m_Pos = pos;
	m_Right = R;
	m_Up = U;
	m_Look = L;

	buildView();
	bulidWorldFrustumPlane();

	m_ViewProj = m_View * m_Proj;
}


void Camera::SetLens(float fov, float aspect, float nearz, float farz)
{
	D3DXMatrixPerspectiveFovLH(&m_Proj, fov, aspect, nearz, farz);
	bulidWorldFrustumPlane();
	m_ViewProj = m_View * m_Proj;
}

void Camera::SetSpeed(float s)
{
	m_Speed = s;
}

bool Camera::isVisible(const AABB& box) const
{
	// Test assumes frustum planes face inward.

	D3DXVECTOR3 P;
	D3DXVECTOR3 Q;

	for (int i = 0; i < 6; ++i)
	{
		// For each coordinate axis x, y, z...
		for (int j = 0; j < 3; ++j)
		{
			// Make PQ point in the same direction as the plane normal on this axis.
			if (m_FrustumPlanes[i][j] >= 0.0f)
			{
				P[j] = box.minPt[j];
				Q[j] = box.maxPt[j];
			}
			else
			{
				P[j] = box.maxPt[j];
				Q[j] = box.minPt[j];
			}
		}

		if (D3DXPlaneDotCoord(&m_FrustumPlanes[i], &Q) < 0.0f) // outside
			return false;
	}
	return true;
}

void Camera::update(float ftime, D3DXVECTOR3 dir, short state, float angle)
{
	D3DXVec3Normalize(&dir, &dir);
	m_Pos += dir * m_Speed*ftime;

	switch (state)
	{
	case 0:				//up vector rotation
	{
		this->RotationUpVec(angle);
	}
	break;
	case 1:				//right vector rotation
	{
		this->RotationRightVec(angle);
	}
	break;
	case 2:
	{
		this->RotationLookVec(angle);
	}
	break;

	default:
		break;
	}

	// Rebuild the view matrix to reflect changes.
	buildView();
	bulidWorldFrustumPlane();

	m_ViewProj = m_View * m_Proj;
}

void Camera::RotationRightVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_Right, fAngle);
	D3DXVec3TransformCoord(&m_Look, &m_Look, &R);
	D3DXVec3TransformCoord(&m_Up, &m_Up, &R);
}

void Camera::RotationUpVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_Up, fAngle);
	D3DXVec3TransformCoord(&m_Look, &m_Look, &R);
	D3DXVec3TransformCoord(&m_Right, &m_Right, &R);
}

void Camera::RotationLookVec(FLOAT fAngle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_Look, fAngle);
	D3DXVec3TransformCoord(&m_Right, &m_Right, &R);
	D3DXVec3TransformCoord(&m_Up, &m_Up, &R);
}


void Camera::buildView()
{
	// Keep camera's axes orthogonal to each other and of unit length.
	D3DXVec3Normalize(&m_Look, &m_Look);

	D3DXVec3Cross(&m_Up, &m_Look, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	D3DXVec3Cross(&m_Right, &m_Up, &m_Look);
	D3DXVec3Normalize(&m_Right, &m_Right);

	// Fill in the view matrix entries.

	float x = -D3DXVec3Dot(&m_Pos, &m_Right);
	float y = -D3DXVec3Dot(&m_Pos, &m_Up);
	float z = -D3DXVec3Dot(&m_Pos, &m_Look);

	m_View(0, 0) = m_Right.x;
	m_View(1, 0) = m_Right.y;
	m_View(2, 0) = m_Right.z;
	m_View(3, 0) = x;

	m_View(0, 1) = m_Up.x;
	m_View(1, 1) = m_Up.y;
	m_View(2, 1) = m_Up.z;
	m_View(3, 1) = y;

	m_View(0, 2) = m_Look.x;
	m_View(1, 2) = m_Look.y;
	m_View(2, 2) = m_Look.z;
	m_View(3, 2) = z;

	m_View(0, 3) = 0.0f;
	m_View(1, 3) = 0.0f;
	m_View(2, 3) = 0.0f;
	m_View(3, 3) = 1.0f;
}

void Camera::bulidWorldFrustumPlane()
{
	//Extra the frustum planes in world space
	D3DXMATRIX VP = m_View * m_Proj;

	D3DXVECTOR4 col0(VP(0, 0), VP(1, 0), VP(2, 0), VP(3, 0));
	D3DXVECTOR4 col1(VP(0, 1), VP(1, 1), VP(2, 1), VP(3, 1));
	D3DXVECTOR4 col2(VP(0, 2), VP(1, 2), VP(2, 2), VP(3, 2));
	D3DXVECTOR4 col3(VP(0, 3), VP(1, 3), VP(2, 3), VP(3, 3));

	// Planes face inward.
	m_FrustumPlanes[0] = (D3DXPLANE)(col2);        // near
	m_FrustumPlanes[1] = (D3DXPLANE)(col3 - col2); // far
	m_FrustumPlanes[2] = (D3DXPLANE)(col3 + col0); // left
	m_FrustumPlanes[3] = (D3DXPLANE)(col3 - col0); // right
	m_FrustumPlanes[4] = (D3DXPLANE)(col3 - col1); // top
	m_FrustumPlanes[5] = (D3DXPLANE)(col3 + col1); // bottom

	for (int i = 0; i < 6; ++i)
		D3DXPlaneNormalize(&m_FrustumPlanes[i], &m_FrustumPlanes[i]);
}