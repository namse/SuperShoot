
//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 
#include "DXUT.h"
/*
#include "DXUTcamera.h"
#include "DXUTenum.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTres.h"
#include "DXUTsettingsdlg.h"
#include "SDKmesh.h"
#include "SDKmisc.h"
*/
#define DEFMESHFILENAME L"dwarf\\dwarf.x"
#define MAX_NUM_LIGHTS 6
#define ADJACENCY_EPSILON 0.0001f
#define EXTRUDE_EPSILON 0.1f
#define AMBIENT 0.10f


//#define CAMERA_INIT_EYE D3DXVECTOR3( 00.0f, 40.0f, -0.0f )
//#define CAMERA_INIT_LOOKAT D3DXVECTOR3( 20.0f, 0.f, 20.0f )

enum RENDER_TYPE
{
    RENDERTYPE_SCENE,
    RENDERTYPE_ShadowVolume,
    RENDERTYPE_COMPLEXITY
};

D3DXVECTOR4 g_vShadowColor[MAX_NUM_LIGHTS] =
{
    D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 0.2f ),
    D3DXVECTOR4( 1.0f, 1.0f, 0.0f, 0.2f ),
    D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 0.2f ),
    D3DXVECTOR4( 0.0f, 0.0f, 1.0f, 0.2f ),
    D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 0.2f ),
    D3DXVECTOR4( 0.0f, 1.0f, 1.0f, 0.2f )
};


struct LIGHTINITDATA
{
    D3DXVECTOR3 Position;
    D3DXVECTOR4 Color;
public:
LIGHTINITDATA()
{
}
LIGHTINITDATA( D3DXVECTOR3 P, D3DXVECTOR4 C )
{
    Position = P;
    Color = C;
}
};
LIGHTINITDATA g_LightInit[MAX_NUM_LIGHTS] =
{
    LIGHTINITDATA( D3DXVECTOR3( -2.0f, 3.0f, -3.0f ), D3DXVECTOR4( 15.0f, 15.0f, 15.0f, 1.0f ) ),
#if MAX_NUM_LIGHTS > 1
    LIGHTINITDATA( D3DXVECTOR3( 2.0f, 3.0f, -3.0f ), D3DXVECTOR4( 15.0f, 15.0f, 15.0f, 1.0f ) ),
#endif
#if MAX_NUM_LIGHTS > 2
    LIGHTINITDATA( D3DXVECTOR3( -2.0f, 3.0f, 3.0f ), D3DXVECTOR4( 15.0f, 15.0f, 15.0f, 1.0f ) ),
#endif
#if MAX_NUM_LIGHTS > 3
    LIGHTINITDATA( D3DXVECTOR3( 2.0f, 3.0f, 3.0f ), D3DXVECTOR4( 15.0f, 15.0f, 15.0f, 1.0f ) ),
#endif
#if MAX_NUM_LIGHTS > 4
    LIGHTINITDATA( D3DXVECTOR3( -2.0f, 3.0f, 0.0f ), D3DXVECTOR4( 15.0f, 0.0f, 0.0f, 1.0f ) ),
#endif
#if MAX_NUM_LIGHTS > 5
    LIGHTINITDATA( D3DXVECTOR3( 2.0f, 3.0f, 0.0f ), D3DXVECTOR4( 0.0f, 0.0f, 15.0f, 1.0f ) )
#endif
};

struct POSTPROCVERT
{
    float x, y, z;
    float rhw;

    const static D3DVERTEXELEMENT9 Decl[2];
};

const D3DVERTEXELEMENT9 POSTPROCVERT::Decl[2] =
{
    { 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
    D3DDECL_END()
};


struct SHADOWVERT
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;

    const static D3DVERTEXELEMENT9 Decl[3];
};

const D3DVERTEXELEMENT9 SHADOWVERT::Decl[3] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
    D3DDECL_END()
};


struct MESHVERT
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR2 Tex;

    const static D3DVERTEXELEMENT9 Decl[4];
};

const D3DVERTEXELEMENT9 MESHVERT::Decl[4] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
};


struct CEdgeMapping
{
    int m_anOldEdge[2];  // vertex index of the original edge
    int m_aanNewEdge[2][2]; // vertex indexes of the new edge
    // First subscript = index of the new edge
    // Second subscript = index of the vertex for the edge

public:
        CEdgeMapping()
        {
            FillMemory( m_anOldEdge, sizeof( m_anOldEdge ), -1 );
            FillMemory( m_aanNewEdge, sizeof( m_aanNewEdge ), -1 );
        }
};

#pragma warning( disable : 4324 )
struct CLight
{
    D3DXVECTOR3 m_Position;  // Light position
    D3DXVECTOR4 m_Color;     // Light color
    D3DXMATRIXA16 m_mWorld;  // World transform
};


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont*                      g_pFont = NULL;         // Font for drawing text
ID3DXSprite*                    g_pTextSprite = NULL;   // Sprite for batching draw text calls
ID3DXEffect*                    g_pEffect = NULL;       // D3DX effect interface
IDirect3DTexture9*              g_pDefaultTex = NULL;   // Default texture for faces without one
IDirect3DVertexDeclaration9*    g_pMeshDecl = NULL; // Vertex declaration for the meshes
IDirect3DVertexDeclaration9*    g_pShadowDecl = NULL;// Vertex declaration for the shadow volume
IDirect3DVertexDeclaration9*    g_pPProcDecl = NULL;// Vertex declaration for post-process quad rendering
CFirstPersonCamera              g_Camera;               // Viewer's camera
CModelViewerCamera              g_MCamera;              // Camera for mesh control
CModelViewerCamera              g_LCamera;              // Camera for easy light movement control
D3DXHANDLE                      g_hRenderShadow;        // Technique handle for rendering shadow
D3DXHANDLE                      g_hShowShadow;          // Technique handle for showing shadow volume
D3DXHANDLE                      g_hRenderScene;         // Technique handle for rendering scene
D3DXMATRIXA16                   g_mWorldScaling;        // Scaling to apply to mesh
bool                            g_bShowHelp = true;     // If true, it renders the UI control text
bool                            g_bShowShadowVolume = false;  // Whether the shadow volume is visibly shown.
RENDER_TYPE                     g_RenderType = RENDERTYPE_SCENE;  // Type of rendering to perform
int                             g_nNumLights = 1;       // Number of active lights
CDXUTXFileMesh g_Background[2];        // Background meshes
D3DXMATRIXA16 g_mWorldBack[2];        // Additional scaling and translation for background meshes
int                             g_nCurrBackground = 1;  // Current background mesh
CDXUTXFileMesh                  g_LightMesh;            // Mesh to represent the light source
CDXUTXFileMesh                  g_Mesh;                 // The mesh object
ID3DXMesh*                      g_pShadowMesh = NULL;   // The shadow volume mesh
CDXUTDialogResourceManager      g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg                 g_SettingsDlg;          // Device settings dialog
CDXUTDialog                     g_HUD;                  // dialog for standard controls
CDXUTDialog                     g_SampleUI;             // dialog for sample specific controls
D3DXMATRIXA16                   g_mProjection;
CLight g_aLights[MAX_NUM_LIGHTS];  // Light objects
bool                            g_bLeftButtonDown = false;
bool                            g_bRightButtonDown = false;
bool                            g_bMiddleButtonDown = false;

D3DVIEWPORT9					g_viewport;

IDirect3DDevice9*				g_pd3dDevice;
//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4
#define IDC_RENDERTYPE          5
#define IDC_ENABLELIGHT         6
#define IDC_LUMINANCELABEL      7
#define IDC_LUMINANCE           8
#define IDC_BACKGROUND          9
#define IDC_CHANGEMESH          10
#define IDC_MESHFILENAME        11


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed,
                                  void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext );
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK MouseProc( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down,
                         bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void CALLBACK OnLostDevice( void* pUserContext );
void CALLBACK OnDestroyDevice( void* pUserContext );

void InitApp();
HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh );
void RenderText();
