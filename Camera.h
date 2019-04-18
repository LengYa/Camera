#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include <d3d9.h> 
#include <d3dx9math.h>
#include <d3dx9.h>
#include "Utility.h"

class Camera
	{
	public:
		Camera();
		~Camera();

		//access the values
		const D3DXMATRIX& view() const;
		const D3DXMATRIX& proj() const;
		const D3DXMATRIX& viewproj() const;

		const D3DXVECTOR3& right() const;
		const D3DXVECTOR3& up() const;
		const D3DXVECTOR3& look() const;

		//Access or write the camera's position
		D3DXVECTOR3& pos();
		D3DXMATRIX& SetProj(D3DXMATRIX &p);

		//LokkAtMatrix
		void LookAt(D3DXVECTOR3& pos, D3DXVECTOR3& target, D3DXVECTOR3& up);

		//Perspective projection parameters
		void SetLens(float fov, float aspect, float nearz, float farz);

		//Set the camera speed
		void SetSpeed(float s);

		//update the camera's basis vectors and origin,relative to 
		//the world space,based in user input
		void update(float ftime, D3DXVECTOR3 dir, short state, float angle);
		VOID RotationRightVec(FLOAT fAngle);    // 绕right向量旋转
		VOID RotationUpVec(FLOAT fAngle);       // 绕up向量旋转
		VOID RotationLookVec(FLOAT fAngle);     // 绕look向量旋转 

		//box coordinates should be ralative to world space
		bool isVisible(const AABB& box) const;

	protected:
		// Constructs the view matrix based on the camera's basis
		// vectors and origin, relative to the world space
		void buildView();
		void bulidWorldFrustumPlane();

	private:
		//Save camera related matrices
		D3DXMATRIX		m_View;
		D3DXMATRIX		m_Proj;
		D3DXMATRIX		m_ViewProj;

		//Relative to world space
		D3DXVECTOR3		m_Pos;
		D3DXVECTOR3     m_Right;
		D3DXVECTOR3		m_Up;
		D3DXVECTOR3		m_Look;

		//Camera speed
		float			m_Speed;

		//Frustum planes
		D3DXPLANE		m_FrustumPlanes[6];		// [0] = near
												// [1] = far
												// [2] = left
												// [3] = right
												// [4] = top
												// [5] = bottom
	};

#endif // !CAMERA_H
