// -------------------------------------------------------------------------------------
// This is the main file for the circle test application
// -------------------------------------------------------------------------------------

#include "DXUT/DXUT.h"
#include "DXUT/DXUTmisc.h"
#include "resource.h"
#include "CubeRenderer.h"
#include "ContactChecker.h"
#include <math.h>




//--------------------------------------------------------------------------------------
// Basic components 
//--------------------------------------------------------------------------------------
CubeRenderer* g_cubeRenderer = NULL;
// The positional matrix for the cubes.
D3DXMATRIX g_position[2]; 

// The key states we use.
bool g_wasW;
bool g_wasS;
bool g_wasA;
bool g_wasD;

bool g_wasUp;
bool g_wasDown;
bool g_wasLeft;
bool g_wasRight;

// The different angles we contain.
float g_anglesFirstCube[2];
float g_anglesSecondCube[2];


// The contact checker.
ContactChecker g_contact;

// The flag if we are in contact.
bool g_isContact;



//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------

HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBufferSurfaceDesc,
                                      void* pUserContext );
HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBufferSurfaceDesc, void* pUserContext );
void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnD3D10DestroyDevice( void* pUserContext );

void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	
	
    // Set DXUT callbacks
    DXUTSetCallbackD3D10DeviceCreated( OnD3D10CreateDevice );
    DXUTSetCallbackD3D10SwapChainResized( OnD3D10ResizedSwapChain );
    DXUTSetCallbackD3D10DeviceDestroyed( OnD3D10DestroyDevice );
    DXUTSetCallbackD3D10FrameRender( OnD3D10FrameRender );

    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackFrameMove( OnFrameMove );

    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"Contact Test" );
    DXUTCreateDevice( true, 640, 640 );
    DXUTMainLoop(); // Enter into the DXUT render loop

    return DXUTGetExitCode();
}


// Updates the positional and orientational matrices.
void UpdateMatrices()
{
	D3DXMATRIX rotational;
	D3DXMATRIX translational;

	
	D3DXMatrixRotationYawPitchRoll(&rotational, g_anglesFirstCube[0], g_anglesFirstCube[1], 0.0f);
	D3DXMatrixTranslation(&translational, -1.2f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&g_position[0], &rotational, & translational);
	D3DXMatrixRotationYawPitchRoll(&rotational, g_anglesSecondCube[0], g_anglesSecondCube[1], 0.0f);
	D3DXMatrixTranslation(&translational, 1.2f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&g_position[1], &rotational, & translational);
}

//--------------------------------------------------------------------------------------
// Create all the required resources.
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBufferSurfaceDesc,
                                      void* pUserContext )
{
	D3DXVECTOR3 rotationAxisA(1.0, 0.0, 0.0);
	D3DXVECTOR3 rotationAxisB(0.0, 1.0, 0.0);
	D3DXQUATERNION leftA, rightA, leftB, rightB;

	// Generate the quaternions from an axis and angle representation.
	D3DXQuaternionRotationAxis(&leftA, &rotationAxisA, - D3DX_PI * 0.5f);
	D3DXQuaternionRotationAxis(&rightA, &rotationAxisA, D3DX_PI * 0.5f);
	D3DXQuaternionRotationAxis(&leftB, &rotationAxisB, - D3DX_PI * 0.5f);
	D3DXQuaternionRotationAxis(&rightB, &rotationAxisB, D3DX_PI * 0.5f);


	g_cubeRenderer = new CubeRenderer(pd3dDevice);	


	g_anglesFirstCube[0] = g_anglesFirstCube[1] = 0.0f;
	g_anglesSecondCube[0] = g_anglesSecondCube[1] = 0.0f;

	UpdateMatrices();
	
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up stuff here.
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10DestroyDevice( void* pUserContext )
{
	delete g_cubeRenderer;
}

//--------------------------------------------------------------------------------------
// Handle the window size change.
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBufferSurfaceDesc, void* pUserContext )
{
   
	 // Setup the projection parameters again
    float fAspect = static_cast<float>( pBufferSurfaceDesc->Width ) / static_cast<float>( pBufferSurfaceDesc->Height );
	g_cubeRenderer->SetAspectRatio(fAspect);
 
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
     
	//
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
    pd3dDevice->ClearRenderTargetView( pRTV, ClearColor );

    //
    // Clear the depth stencil
    //
    ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
    pd3dDevice->ClearDepthStencilView( pDSV, D3D10_CLEAR_DEPTH, 1.0, 0 );

	g_cubeRenderer->Render(pd3dDevice, g_position[0], g_isContact);
	g_cubeRenderer->Render(pd3dDevice, g_position[1], g_isContact);
	
}






//--------------------------------------------------------------------------------------
// Update the scene here.
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	if (g_wasW)
		g_anglesFirstCube[1] += fElapsedTime;
	if (g_wasS)
		g_anglesFirstCube[1] -= fElapsedTime;
	if (g_wasA)
		g_anglesFirstCube[0] += fElapsedTime;
	if (g_wasD)
		g_anglesFirstCube[0] -= fElapsedTime;


	if (g_wasUp)
		g_anglesSecondCube[1] += fElapsedTime;
	if (g_wasDown)
		g_anglesSecondCube[1] -= fElapsedTime;
	if (g_wasLeft)
		g_anglesSecondCube[0] += fElapsedTime;
	if (g_wasRight)
		g_anglesSecondCube[0] -= fElapsedTime;

	UpdateMatrices();
	g_isContact = g_contact.CheckForContact(g_position[0], g_position[1]);

}





//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	switch(nChar)
	{
	case 'W':
		g_wasW = bKeyDown;
		break;
	case 'S':
		g_wasS = bKeyDown;
		break;
	case 'A':
		g_wasA = bKeyDown;
		break;
	case 'D':
		g_wasD = bKeyDown;
		break;
	case VK_UP:
		g_wasUp = bKeyDown;
		break;
	case VK_DOWN:
		g_wasDown = bKeyDown;
		break;
	case VK_LEFT:
		g_wasLeft = bKeyDown;
		break;
	case VK_RIGHT:
		g_wasRight = bKeyDown;
		break;

	}	
}



