#pragma once
#ifndef SKY_MODEL_H
#define SKY_MODEL_H

#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>

#include "Utility.h"

	//SkyBox's FVF
	struct SKYBOXVERTEX
	{
		float x, y, z;
		float u, v;
	};
#define D3DFVF_SKYBOX	D3DFVF_XYZ|D3DFVF_TEX1

	class CSkyModel
	{
	public:
		CSkyModel(LPDIRECT3DDEVICE9 pDevice);

		virtual ~CSkyModel();

		//Data types & constants
		enum ModelType
		{
			k_box,
			k_hemisphere,
			k_sphere,
			k_dome
		};

		bool	Init_SkyBox(float Length);					//Initialize the sky box
		bool	Load_SkyTextureFromFile(wchar_t *pFrontTextureFile, wchar_t *pBackTextureFile, wchar_t *pLeftTextureFile, \
			wchar_t *pRightTextureFile, wchar_t *pTopTextureFile, wchar_t *pBottomTextureFile);
		void	Render_SkyBox(D3DXMATRIX *pMatWorld, bool bRenderFrame);			//the first parameter is the sky box's world matrix,
																					//the second parameter is doing or not doing render the line (ÊÇ·ñäÖÈ¾³öÏß¿ò)
		bool	Init_Floor(float length);
		void	Render_Floor(bool bRenderFrame);

	private:
		ModelType m_type;
		LPD3DXMESH m_pMesh;

		D3DXHANDLE m_hUVSettings;

		LPDIRECT3DDEVICE9			m_pd3dDevice;			//D3D Device object
		LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer;		//the vertex buffer object
		LPDIRECT3DVERTEXBUFFER9		m_pFloorVerBuffer;		//the floor's vertex buffer
		LPDIRECT3DTEXTURE9			m_pTexture[6];
		float						m_Length;				//the length of skybox
	};
#endif // !SKY_MODEL_H
