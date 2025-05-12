cmake_minimum_required( VERSION 3.30 )


# 의존 패키지 관리자 ( https://vcpkg.io/en/ )

set( VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "" )
set( CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" )


# 프로젝트

project( DX11andHLSL 
    VERSION 1.0.0 
    DESCRIPTION "Real-Time 3D Graphics Programming with DirectX 11 and HLSL" 
    LANGUAGES CXX
)


# 전역 설정

set( CMAKE_CXX_STANDARD 17 )
set( CXX_STANDARD_REQUIRED ON )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

add_definitions( -DUNICODE )
add_definitions( -D_UNICODE )
add_definitions( -DWIN32_LEAN_AND_MEAN )


# 의존 패키지

# find_package( dxsdk-d3dx CONFIG REQUIRED )
# target_link_libraries( main PRIVATE Microsoft::D3DX9 Microsoft::D3DX10 Microsoft::D3DX11 )

# find_package( effects11 CONFIG REQUIRED )
# target_link_libraries( main PRIVATE Microsoft::Effects11 )

find_package( Microsoft.GSL CONFIG REQUIRED )
# target_link_libraries( main PRIVATE Microsoft.GSL::GSL )

find_package( directxtk CONFIG REQUIRED )
# target_link_libraries( main PRIVATE Microsoft::DirectXTK )

find_package( assimp CONFIG REQUIRED )
# target_link_libraries( main PRIVATE assimp::assimp )

find_package( imgui CONFIG REQUIRED )
# target_link_libraries( main PRIVATE imgui::imgui )


# 하위 프로젝트

add_subdirectory( source/Library.Shared )
add_subdirectory( source/Library.Desktop )

add_subdirectory( source/1.1_Win32_Startup )
add_subdirectory( source/1.2_Win32_Framework )
add_subdirectory( source/1.3_Point )
add_subdirectory( source/1.4_Colored_Triangle )
add_subdirectory( source/1.5_Colored_Cube )

add_subdirectory( source/2.1_Model )

# media 폴더를 실행 폴더( Debug, Release )로 복사하는 샘플 코드
# add_custom_command(
#     TARGET ${target} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_CURRENT_SOURCE_DIR}/media $<TARGET_FILE_DIR:${target}>/media
#     COMMENT "Copying 'media' folder to build output directory"
# )
