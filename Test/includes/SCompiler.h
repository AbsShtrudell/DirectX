#pragma once
#include <windows.h>
#include <d3dcompiler.h>
#include <d3dx11.h>

#pragma comment(lib, "d3dcompiler.lib")

inline HRESULT CompileShaderFromFile( LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;

    hr = D3DCompileFromFile( 
        szFileName, 
        NULL, 
        NULL, 
        szEntryPoint, 
        szShaderModel,
        dwShaderFlags, 
        NULL, 
        ppBlobOut, 
        &pErrorBlob);

    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}