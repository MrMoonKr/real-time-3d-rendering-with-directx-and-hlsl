
set( target "Library.Desktop" )

set( target_SOURCES
    UtilityWin32.cpp UtilityWin32.h
)

add_library( ${target} STATIC 
    ${target_SOURCES} 
)

target_include_directories( ${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../Library.Shared
)

target_link_libraries( ${target} PRIVATE
    Library.Shared
    dxgi
    d3d11
    d3dcompiler # d3d10compiler d3d11compiler
    dinput8 
    dxguid
    Microsoft.GSL::GSL
    Microsoft::DirectXTK
    assimp::assimp
    imgui::imgui
    # Microsoft::Effects11
)

