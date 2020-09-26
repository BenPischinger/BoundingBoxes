#pragma once


#include "DXUT/DXUT.h"
#include "DXUT/DXUTmisc.h"


// Renders the textured cube with all the different transformation options.
class CubeRenderer
{
public:
	CubeRenderer(ID3D10Device* basicRenderingDeviceDevice);
	~CubeRenderer(void);

	// Sets the new aspect ratio on a window resize.
	void SetAspectRatio(float ratio);
	
	// Renders the cube with the indicated device.
	void Render(ID3D10Device* basicRenderingDeviceDevice, const D3DXMATRIX& positionalMatrix, bool paintRed);

private:
	// -------------------------------------------------------------
	// All the shader paremteres come here
	//-------------------------------------------------------------
	ID3D10Effect*                       m_pEffect;
	ID3D10Buffer*                       m_pVertexBuffer;
	ID3D10InputLayout*                  m_pVertexLayout;
	ID3D10EffectTechnique*              m_pTechnique;
	ID3D10EffectMatrixVariable*         m_pViewVariable;
	ID3D10EffectMatrixVariable*         m_pProjectionVariable;
	ID3D10EffectMatrixVariable*         m_pRotationVariable;
	ID3D10EffectVectorVariable*			m_pColorVariable;
	D3DXMATRIX                          m_View;
	D3DXMATRIX                          m_Projection;
	D3DXMATRIX							m_Rotation;


	// The vertex 
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Norm;
	};

};

