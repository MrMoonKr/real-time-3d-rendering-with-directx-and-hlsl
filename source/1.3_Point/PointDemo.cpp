#include "pch.h"
#include "PointDemo.h"
#include "Utility.h"
#include "Game.h"
#include "GameException.h"

using namespace std;
using namespace std::string_literals;
using namespace Library;
using namespace DirectX;

//#include <directx-dxc/dxcapi.h>
//#include <wrl/client.h>
//using Microsoft::WRL::ComPtr;
//#include <wrl/client.h>
#include <d3dcompiler.h>
//#include <dxcapi.h>
//#include <directx-dxc/dxcapi.h>

winrt::com_ptr<ID3DBlob> CompileShader(const wchar_t* filename, const char* entryPoint, const char* target)
{
    winrt::com_ptr<ID3DBlob> shaderBlob;
    winrt::com_ptr<ID3DBlob> errorBlob;

    HRESULT hr = D3DCompileFromFile(
        filename, nullptr, nullptr, entryPoint, target,
        D3DCOMPILE_ENABLE_STRICTNESS, 0, shaderBlob.put(), errorBlob.put() );

    if (FAILED(hr)) {
        if (errorBlob) {
            std::cerr << "Shader Compilation Error:\n" 
                      << (char*)errorBlob->GetBufferPointer() << std::endl;
        }
        return nullptr;
    }

    return shaderBlob;
}

winrt::com_ptr<ID3D11VertexShader> CreateVertexShader(ID3D11Device* device, const wchar_t* filename)
{
    //auto shaderBlob = CompileShader(filename, "main", "vs_5_0");
    auto shaderBlob = CompileShader(filename, "VSMain", "vs_5_0");
    if (!shaderBlob) return nullptr;

    winrt::com_ptr<ID3D11VertexShader> vertexShader;
    device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, vertexShader.put());
    return vertexShader;
}

winrt::com_ptr<ID3D11PixelShader> CreatePixelShader(ID3D11Device* device, const wchar_t* filename)
{
    //auto shaderBlob = CompileShader(filename, "main", "ps_5_0");
    auto shaderBlob = CompileShader(filename, "PSMain", "ps_5_0");
    if (!shaderBlob) return nullptr;

    winrt::com_ptr<ID3D11PixelShader> pixelShader;
    device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, pixelShader.put());
    return pixelShader;
}


namespace Rendering
{
    RTTI_DEFINITIONS( PointDemo )

    PointDemo::PointDemo( Game& game ) : DrawableGameComponent( game )
    {
    }

    void PointDemo::Initialize()
    {
        // Load a compiled vertex shader
        // vector<char> compiledVertexShader;
        // Utility::LoadBinaryFile( L"Content\\Shaders\\PointDemoVS.cso"s, compiledVertexShader );
        // ThrowIfFailed( mGame->Direct3DDevice()->CreateVertexShader(
        //                    compiledVertexShader.data(), compiledVertexShader.size(), nullptr, mVertexShader.put() ),
        //                "ID3D11Device::CreatedVertexShader() failed." );

        //winrt::com_ptr<ID3DBlob> shaderBytecode;

        HRESULT hr = E_FAIL ;

        mVertexShader = CreateVertexShader(
            mGame->Direct3DDevice(),
            L"Content\\Shaders\\PointDemoVS.hlsl"
        );

        // Load a compiled pixel shader
        // vector<char> compiledPixelShader;
        // Utility::LoadBinaryFile( L"Content\\Shaders\\PointDemoPS.cso"s, compiledPixelShader );
        // ThrowIfFailed( mGame->Direct3DDevice()->CreatePixelShader(
        //                    compiledPixelShader.data(), compiledPixelShader.size(), nullptr, mPixelShader.put() ),
        //                "ID3D11Device::CreatedPixelShader() failed." );

        mPixelShader = CreatePixelShader(
            mGame->Direct3DDevice(),
            L"Content\\Shaders\\PointDemoPS.hlsl"
        );
    }

    void PointDemo::Draw( const GameTime& )
    {
        ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
        direct3DDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );

        direct3DDeviceContext->VSSetShader( mVertexShader.get(), nullptr, 0 );
        direct3DDeviceContext->PSSetShader( mPixelShader.get(), nullptr, 0 );

        direct3DDeviceContext->Draw( 1, 0 );
    }
} // namespace Rendering
