#include "CubeRenderer.h"



// Creates the cube renderer with the device.
// Loads all the required assets here.
CubeRenderer::CubeRenderer(ID3D10Device* basicRenderingDeviceDevice)
{

	HRESULT hr = S_OK;

	hr = D3DX10CreateEffectFromFile( L"SimpleShader.fx", NULL, NULL, "fx_4_0", 0, 0, basicRenderingDeviceDevice, NULL,
                                         NULL, &m_pEffect, NULL, NULL );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be located.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
    }

	m_pTechnique = m_pEffect->GetTechniqueByName( "Render" );
    m_pViewVariable = m_pEffect->GetVariableByName( "View" )->AsMatrix();
    m_pProjectionVariable = m_pEffect->GetVariableByName( "Projection" )->AsMatrix();
	m_pRotationVariable = m_pEffect->GetVariableByName("Rotation")->AsMatrix();
	m_pColorVariable = m_pEffect->GetVariableByName("paintColor")->AsVector();
  
	// Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = sizeof( layout ) / sizeof( layout[0] );

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
    m_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    basicRenderingDeviceDevice->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature,
                                             PassDesc.IAInputSignatureSize, &m_pVertexLayout );

    // Set the input layout
    basicRenderingDeviceDevice->IASetInputLayout( m_pVertexLayout );


	// The input vertices that get statically initialized to render the cube.
	SimpleVertex inputVertices[36];
	// Fill in the required vertices for the cubic surface.
	int k=0;
	// Y+ surface
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, 1.0f, -1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, 1.0f, -1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, 1.0f, 1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, 1.0f, 1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, 1.0f, -1.0f );inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, 1.0f, 1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

	// Y- surface
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, -1.0f, -1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, -1.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, -1.0f, -1.0f );inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, -1.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, -1.0f, 1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, -1.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, -1.0f, -1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, -1.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, -1.0f, 1.0f );inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, -1.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, -1.0f, 1.0f );inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, -1.0f, 0.0f);

	// Z+ surface
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, -1.0f , 1.0f); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, -1.0f , 1.0f); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, 1.0f , 1.0f); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, -1.0f , 1.0f);inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, 1.0f,  1.0f);inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, 1.0f, 1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	

	// Z- surface
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, -1.0f , -1.0f); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, -1.0f , -1.0f); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, 1.0f , -1.0f); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, 1.0f,  -1.0f); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, -1.0f , -1.0f); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, 1.0f, -1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);

	// X+ surface
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, -1.0f,  -1.0f );  inputVertices[k++].Norm = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, 1.0f, -1.0f );  inputVertices[k++].Norm = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3(  1.0f, -1.0f, 1.0f );  inputVertices[k++].Norm = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, 1.0f,  -1.0f );  inputVertices[k++].Norm = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, 1.0f,  1.0f );  inputVertices[k++].Norm = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( 1.0f, -1.0f,  1.0f );  inputVertices[k++].Norm = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);

	// X- surface
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, 1.0f, -1.0f );  inputVertices[k++].Norm = D3DXVECTOR3( -1.0f, 0.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, -1.0f,  -1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( -1.0f, 0.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3(  -1.0f, -1.0f, 1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( -1.0f, 0.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, 1.0f,  1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( -1.0f, 0.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, 1.0f,  -1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( -1.0f, 0.0f, 0.0f);
	inputVertices[k].Pos =  D3DXVECTOR3( -1.0f, -1.0f,  1.0f ); inputVertices[k++].Norm = D3DXVECTOR3( -1.0f, 0.0f, 0.0f);


	// Create the vertex buffer.
	D3D10_BUFFER_DESC bd;
	D3D10_SUBRESOURCE_DATA baseData;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * 36;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
	baseData.pSysMem = &(inputVertices[0]);
    basicRenderingDeviceDevice->CreateBuffer( &bd, &baseData, &m_pVertexBuffer);

	// Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    basicRenderingDeviceDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	 // Set primitive topology
    basicRenderingDeviceDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	
	// Initialize the view matrix
    D3DXVECTOR3 Eye( 0.0f, 0.0f, -8.0f );
    D3DXVECTOR3 At( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 Up( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &m_View, &Eye, &At, &Up );

    // Update Variables that never change
    m_pViewVariable->SetMatrix( ( float* )&m_View );
    

}

/// Remove the allocated resources.
CubeRenderer::~CubeRenderer(void)
{
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pVertexBuffer);
}


// Sets the new aspect ratio on a window resize.
void CubeRenderer::SetAspectRatio(float ratio)
{
	 D3DXMatrixPerspectiveFovLH( &m_Projection, D3DX_PI * 0.25f, ratio, 0.1f, 100.0f );
	 m_pProjectionVariable->SetMatrix( ( float* )&m_Projection );
}



// Renders the complete cube. The world positional matrix is handed over as well.
void CubeRenderer::Render(ID3D10Device* basicRenderingDeviceDevice,const D3DXMATRIX& positionalMatrix,  bool paintRed)
{
	D3DVECTOR color;
	if (paintRed)
	{
		color.x = 0.75f;
		color.y = 0.25f;
		color.z = 0.25f;
	}
	else
	{
		color.x = 0.25f;
		color.y = 0.75f;
		color.z = 0.25f;
	}

	m_pColorVariable->SetFloatVector(&color.x);
	m_pRotationVariable->SetMatrix((float*) &positionalMatrix);
    m_pTechnique->GetPassByIndex( 0 )->Apply( 0 );
	basicRenderingDeviceDevice->Draw(36,0);     
}