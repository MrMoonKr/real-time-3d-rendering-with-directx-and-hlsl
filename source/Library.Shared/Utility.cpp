#include "pch.h"
#include "Utility.h"
#include "Shader.h"

using namespace std;

namespace Library
{
    void Utility::GetFileName( const string& inputPath, string& filename )
    {
        string fullPath( inputPath );
        replace( fullPath.begin(), fullPath.end(), '\\', '/' );

        string::size_type lastSlashIndex = fullPath.find_last_of( '/' );

        if ( lastSlashIndex == string::npos )
        {
            filename = fullPath;
        }
        else
        {
            filename = fullPath.substr( lastSlashIndex + 1, fullPath.size() - lastSlashIndex - 1 );
        }
    }

    void Utility::GetDirectory( const string& inputPath, string& directory )
    {
        string fullPath( inputPath );
        replace( fullPath.begin(), fullPath.end(), '\\', '/' );

        string::size_type lastSlashIndex = fullPath.find_last_of( '/' );

        if ( lastSlashIndex == string::npos )
        {
            directory = "";
        }
        else
        {
            directory = fullPath.substr( 0, lastSlashIndex );
        }
    }

    tuple<string, string> Utility::GetFileNameAndDirectory( const string& inputPath )
    {
        string fullPath( inputPath );
        replace( fullPath.begin(), fullPath.end(), '\\', '/' );

        string::size_type lastSlashIndex = fullPath.find_last_of( '/' );

        string directory;
        string filename;

        if ( lastSlashIndex == string::npos )
        {
            directory = "";
            filename = fullPath;
        }
        else
        {
            directory = fullPath.substr( 0, lastSlashIndex );
            filename = fullPath.substr( lastSlashIndex + 1, fullPath.size() - lastSlashIndex - 1 );
        }

        return make_tuple( filename, directory );
    }

    void Utility::LoadBinaryFile( const wstring& filename, vector<char>& data )
    {
        ifstream file( filename.c_str(), ios::binary );
        if ( !file.good() )
        {
            throw exception( "Could not open file." );
        }

        file.seekg( 0, ios::end );
        uint32_t size = ( uint32_t )file.tellg();

        if ( size > 0 )
        {
            data.resize( size );
            file.seekg( 0, ios::beg );
            file.read( &data.front(), size );
        }

        file.close();
    }

    /**
     * @brief 셰이더 소스파일을 컴파일하여 바이너리 데이터를 로드합니다.
     * @param filename 셰이더 소스파일 경로
     * @param entryPoint 셰이더의 진입점. "main", "VSMain", "PSMain 등
     * @param target 셰이더의 타겟 버전. "vs_5_0", "ps_5_0" 등
     * @param data 컴파일된 셰이더 바이너리 데이터
     * @throw exception 컴파일 실패 시 예외 발생
     */
    void Utility::LoadShaderFile( const wstring& filename,  const char* entryPoint, const char* target, vector<char>& data )
    {
        winrt::com_ptr<ID3DBlob> shaderBlob = Shader::CompileShader( filename.c_str(), entryPoint, target );
        if ( shaderBlob == nullptr )
        {
            throw exception( "Could not compile shader." );
        }
        data.resize( shaderBlob->GetBufferSize() );
        memcpy( &data.front(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize() );
        shaderBlob->Release();
        shaderBlob = nullptr;
    }

#pragma warning( push )
#pragma warning( disable : 4996 )
    void Utility::ToWideString( const string& source, wstring& dest )
    {
        dest = wstring_convert<codecvt_utf8<wchar_t>>().from_bytes( source );
    }

    wstring Utility::ToWideString( const string& source )
    {
        return wstring_convert<codecvt_utf8<wchar_t>>().from_bytes( source );
    }

    void Utility::Totring( const wstring& source, string& dest )
    {
        dest = wstring_convert<codecvt_utf8<wchar_t>>().to_bytes( source );
    }

    string Utility::ToString( const wstring& source )
    {
        return wstring_convert<codecvt_utf8<wchar_t>>().to_bytes( source );
    }
#pragma warning( pop )

} // namespace Library