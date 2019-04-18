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
	//1.创建。创建顶点缓存
	m_pd3dDevice->CreateVertexBuffer(24 * sizeof(SKYBOXVERTEX), 0,
		D3DFVF_SKYBOX, D3DPOOL_MANAGED, &m_pVertexBuffer, 0);

	//用一个结构体把顶点数据先准备好
	SKYBOXVERTEX vertices[] =
	{
		//前面的四个顶点
		{ -m_Length / 2, 0.0f,    m_Length / 2, 0.0f, 1.0f, },
		{ -m_Length / 2, m_Length / 2,   m_Length / 2, 0.0f, 0.0f, },
		{ m_Length / 2, 0.0f,    m_Length / 2, 1.0f, 1.0f, },
		{ m_Length / 2, m_Length / 2,   m_Length / 2, 1.0f, 0.0f, },

		//背面的四个顶点
		{ m_Length / 2, 0.0f,   -m_Length / 2, 0.0f, 1.0f, },
		{ m_Length / 2, m_Length / 2,  -m_Length / 2, 0.0f, 0.0f, },
		{ -m_Length / 2, 0.0f,   -m_Length / 2, 1.0f, 1.0f, },
		{ -m_Length / 2, m_Length / 2,  -m_Length / 2, 1.0f, 0.0f, },

		//左面的四个顶点
		{ -m_Length / 2, 0.0f,   -m_Length / 2, 0.0f, 1.0f, },
		{ -m_Length / 2, m_Length / 2,  -m_Length / 2, 0.0f, 0.0f, },
		{ -m_Length / 2, 0.0f,    m_Length / 2, 1.0f, 1.0f, },
		{ -m_Length / 2, m_Length / 2,   m_Length / 2, 1.0f, 0.0f, },

		//右面的四个顶点
		{ m_Length / 2, 0.0f,   m_Length / 2, 0.0f, 1.0f, },
		{ m_Length / 2, m_Length / 2,  m_Length / 2, 0.0f, 0.0f, },
		{ m_Length / 2, 0.0f,  -m_Length / 2, 1.0f, 1.0f, },
		{ m_Length / 2, m_Length / 2, -m_Length / 2, 1.0f, 0.0f, },

		//上面的四个顶点
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

	//准备填充顶点数据
	void *pVertices;

	//Lock
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	//access 把结构体中的数据直接拷到顶点缓冲区中
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
	//1.创建。创建顶点缓存
	m_pd3dDevice->CreateVertexBuffer(4 * sizeof(SKYBOXVERTEX), 0,
		D3DFVF_SKYBOX, D3DPOOL_MANAGED, &m_pFloorVerBuffer, 0);

	//用一个结构体把顶点数据先准备好
	SKYBOXVERTEX vertices[] =
	{
		//底部
		{ -tLength,0.0f,-tLength,0.0f,30.0f },
		{ -tLength,0.0f,tLength,0.0f,0.0f },
		{ tLength,0.0f,-tLength,30.0f,30.0f },
		{ tLength,0.0f,tLength,30.0f,0.0f },
	};
	//准备填充顶点数据
	void *pVertices;

	//Lock
	m_pFloorVerBuffer->Lock(0, 0, (void**)&pVertices, 0);
	//access 把结构体中的数据直接拷到顶点缓冲区中
	memcpy(pVertices, vertices, sizeof(vertices));

	//UnLock
	m_pVertexBuffer->Unlock();

	return true;
}

bool CSkyModel::Load_SkyTextureFromFile(wchar_t *pFrontTextureFile, wchar_t *pBackTextureFile, wchar_t *pLeftTextureFile, \
	wchar_t *pRightTextureFile, wchar_t *pTopTextureFile, wchar_t *pBottomTextureFile)
{
	//从文件加载五张纹理
	D3DXCreateTextureFromFileW(m_pd3dDevice, pFrontTextureFile, &m_pTexture[0]);  //前面
	D3DXCreateTextureFromFileW(m_pd3dDevice, pBackTextureFile, &m_pTexture[1]);  //后面
	D3DXCreateTextureFromFileW(m_pd3dDevice, pLeftTextureFile, &m_pTexture[2]);  //左面
	D3DXCreateTextureFromFileW(m_pd3dDevice, pRightTextureFile, &m_pTexture[3]);  //右面
	D3DXCreateTextureFromFileW(m_pd3dDevice, pTopTextureFile, &m_pTexture[4]);  //上面
	D3DXCreateTextureFromFileW(m_pd3dDevice, pBottomTextureFile, &m_pTexture[5]); //底部
	return TRUE;
}

//--------------------------------------------------------------------------------------
// Name: SkyBoxClass::RenderSkyBox()
// Desc: 绘制出天空盒，可以通过第二个参数选择是否绘制出线框
//--------------------------------------------------------------------------------------
void CSkyModel::Render_SkyBox(D3DXMATRIX *pMatWorld, bool bRenderFrame)
{
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);  //将纹理颜色混合的第一个参数的颜色值用于输出
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);   //纹理颜色混合的第一个参数的值就取纹理颜色值
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	m_pd3dDevice->SetTransform(D3DTS_WORLD, pMatWorld);  //设置世界矩阵
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(SKYBOXVERTEX));    //把包含的几何体信息的顶点缓存和渲染流水线相关联  
	m_pd3dDevice->SetFVF(D3DFVF_SKYBOX);  //设置FVF灵活顶点格式

										  //一个for循环，将6个面绘制出来
	for (int i = 0; i != 6; i++)
	{
		m_pd3dDevice->SetTexture(0, m_pTexture[i]);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}

	//对是否渲染线框的处理代码
	if (bRenderFrame)  //如果要渲染出线框的话
	{
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //把填充模式设为线框填充
																		 //一个for循环，将5个面的线框绘制出来
		for (int i = 0; i != 6; i++)
		{
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);	//绘制顶点 
		}

		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//把填充模式调回实体填充
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

	//对是否渲染线框的处理代码
	if (bRenderFrame)  //如果要渲染出线框的话
	{
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //把填充模式设为线框填充

		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//绘制顶点 

		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//把填充模式调回实体填充
	}
}
