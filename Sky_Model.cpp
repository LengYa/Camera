#define STRICT

#include "Sky_model.h"

//////////////////////////////////////////////////////////////////////////
//Name: CSkyModel Class
//////////////////////////////////////////////////////////////////////////
CSkyModel::CSkyModel(LPDIRECT3DDEVICE9 pDevice)
{
	m_pd3dDevice = pDevice;
	m_pVertexBuffer = NULL;
	for (size_t i = 0; i != 6; i++)
	{
		m_pTexture[i] = NULL;
	}
	m_Length = 0.0f;
}

CSkyModel::~CSkyModel()
{
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pFloorVerBuffer);

	for (size_t i = 0; i != 6; ++i)
	{
		SAFE_RELEASE(m_pTexture[i]);
	}
}

bool CSkyModel::Init_SkyBox(float Length)
{
	m_Length = Length;
	//1.�������������㻺��
	m_pd3dDevice->CreateVertexBuffer(24 * sizeof(SKYBOXVERTEX), 0,
		D3DFVF_SKYBOX, D3DPOOL_MANAGED, &m_pVertexBuffer, 0);

	//��һ���ṹ��Ѷ���������׼����
	SKYBOXVERTEX vertices[] =
	{
		//ǰ����ĸ�����
		{ -m_Length / 2, 0.0f,    m_Length / 2, 0.0f, 1.0f, },
		{ -m_Length / 2, m_Length / 2,   m_Length / 2, 0.0f, 0.0f, },
		{ m_Length / 2, 0.0f,    m_Length / 2, 1.0f, 1.0f, },
		{ m_Length / 2, m_Length / 2,   m_Length / 2, 1.0f, 0.0f, },

		//������ĸ�����
		{ m_Length / 2, 0.0f,   -m_Length / 2, 0.0f, 1.0f, },
		{ m_Length / 2, m_Length / 2,  -m_Length / 2, 0.0f, 0.0f, },
		{ -m_Length / 2, 0.0f,   -m_Length / 2, 1.0f, 1.0f, },
		{ -m_Length / 2, m_Length / 2,  -m_Length / 2, 1.0f, 0.0f, },

		//������ĸ�����
		{ -m_Length / 2, 0.0f,   -m_Length / 2, 0.0f, 1.0f, },
		{ -m_Length / 2, m_Length / 2,  -m_Length / 2, 0.0f, 0.0f, },
		{ -m_Length / 2, 0.0f,    m_Length / 2, 1.0f, 1.0f, },
		{ -m_Length / 2, m_Length / 2,   m_Length / 2, 1.0f, 0.0f, },

		//������ĸ�����
		{ m_Length / 2, 0.0f,   m_Length / 2, 0.0f, 1.0f, },
		{ m_Length / 2, m_Length / 2,  m_Length / 2, 0.0f, 0.0f, },
		{ m_Length / 2, 0.0f,  -m_Length / 2, 1.0f, 1.0f, },
		{ m_Length / 2, m_Length / 2, -m_Length / 2, 1.0f, 0.0f, },

		//������ĸ�����
		{ m_Length / 2, m_Length / 2, -m_Length / 2, 1.0f, 0.0f, },
		{ m_Length / 2, m_Length / 2,  m_Length / 2, 1.0f, 1.0f, },
		{ -m_Length / 2, m_Length / 2, -m_Length / 2, 0.0f, 0.0f, },
		{ -m_Length / 2, m_Length / 2,  m_Length / 2, 0.0f, 1.0f, },

		//bottom
		{-m_Length / 2,0,m_Length / 2,0,0},
		{-m_Length / 2,0,-m_Length / 2,0,1},
		{m_Length / 2,0,m_Length / 2,1,0},
		{m_Length / 2,0,-m_Length / 2,1,1},
	};

	//׼����䶥������
	void *pVertices;

	//Lock
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	//access �ѽṹ���е�����ֱ�ӿ������㻺������
	memcpy(pVertices, vertices, sizeof(vertices));

	//UnLock
	m_pVertexBuffer->Unlock();

	//Floor
	float _length = 5000.0f;
	this->Init_Floor(_length);

	return true;
}

bool CSkyModel::Init_Floor(float tLength)
{
	//1.�������������㻺��
	m_pd3dDevice->CreateVertexBuffer(4 * sizeof(SKYBOXVERTEX), 0,
		D3DFVF_SKYBOX, D3DPOOL_MANAGED, &m_pFloorVerBuffer, 0);

	//��һ���ṹ��Ѷ���������׼����
	SKYBOXVERTEX vertices[] =
	{
		//�ײ�
		{ -tLength,0.0f,-tLength,0.0f,30.0f },
		{ -tLength,0.0f,tLength,0.0f,0.0f },
		{ tLength,0.0f,-tLength,30.0f,30.0f },
		{ tLength,0.0f,tLength,30.0f,0.0f },
	};
	//׼����䶥������
	void *pVertices;

	//Lock
	m_pFloorVerBuffer->Lock(0, 0, (void**)&pVertices, 0);
	//access �ѽṹ���е�����ֱ�ӿ������㻺������
	memcpy(pVertices, vertices, sizeof(vertices));

	//UnLock
	m_pVertexBuffer->Unlock();

	return true;
}

bool CSkyModel::Load_SkyTextureFromFile(wchar_t *pFrontTextureFile, wchar_t *pBackTextureFile, wchar_t *pLeftTextureFile, \
	wchar_t *pRightTextureFile, wchar_t *pTopTextureFile, wchar_t *pBottomTextureFile)
{
	//���ļ�������������
	D3DXCreateTextureFromFileW(m_pd3dDevice, pFrontTextureFile, &m_pTexture[0]);  //ǰ��
	D3DXCreateTextureFromFileW(m_pd3dDevice, pBackTextureFile, &m_pTexture[1]);  //����
	D3DXCreateTextureFromFileW(m_pd3dDevice, pLeftTextureFile, &m_pTexture[2]);  //����
	D3DXCreateTextureFromFileW(m_pd3dDevice, pRightTextureFile, &m_pTexture[3]);  //����
	D3DXCreateTextureFromFileW(m_pd3dDevice, pTopTextureFile, &m_pTexture[4]);  //����
	D3DXCreateTextureFromFileW(m_pd3dDevice, pBottomTextureFile, &m_pTexture[5]); //�ײ�
	return TRUE;
}

//--------------------------------------------------------------------------------------
// Name: SkyBoxClass::RenderSkyBox()
// Desc: ���Ƴ���պУ�����ͨ���ڶ�������ѡ���Ƿ���Ƴ��߿�
//--------------------------------------------------------------------------------------
void CSkyModel::Render_SkyBox(D3DXMATRIX *pMatWorld, bool bRenderFrame)
{
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);  //��������ɫ��ϵĵ�һ����������ɫֵ�������
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);   //������ɫ��ϵĵ�һ��������ֵ��ȡ������ɫֵ
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	m_pd3dDevice->SetTransform(D3DTS_WORLD, pMatWorld);  //�����������
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SKYBOXVERTEX));    //�Ѱ����ļ�������Ϣ�Ķ��㻺�����Ⱦ��ˮ�������  
	m_pd3dDevice->SetFVF(D3DFVF_SKYBOX);  //����FVF�����ʽ

										  //һ��forѭ������6������Ƴ���
	for (int i = 0; i != 6; i++)
	{
		m_pd3dDevice->SetTexture(0, m_pTexture[i]);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}

	//���Ƿ���Ⱦ�߿�Ĵ������
	if (bRenderFrame)  //���Ҫ��Ⱦ���߿�Ļ�
	{
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //�����ģʽ��Ϊ�߿����
																		 //һ��forѭ������5������߿���Ƴ���
		for (int i = 0; i != 6; i++)
		{
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);	//���ƶ��� 
		}

		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//�����ģʽ����ʵ�����
	}
}


void CSkyModel::Render_Floor(bool bRenderFrame)
{
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetStreamSource(0, m_pFloorVerBuffer, 0, sizeof(SKYBOXVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_SKYBOX);
	m_pd3dDevice->SetTexture(0, m_pTexture[5]);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���Ƿ���Ⱦ�߿�Ĵ������
	if (bRenderFrame)  //���Ҫ��Ⱦ���߿�Ļ�
	{
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //�����ģʽ��Ϊ�߿����

		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//���ƶ��� 

		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//�����ģʽ����ʵ�����
	}
}
