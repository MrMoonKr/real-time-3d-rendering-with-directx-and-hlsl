# CMakeLists.txt for Ch01_01_Win32_Startup

set( target Ch01_03_Point )

add_executable( ${target} WIN32
    Program.cpp
    PointDemo.cpp PointDemo.h
    RenderingGame.cpp RenderingGame.h
)

target_include_directories( ${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../Library.Shared
    ${CMAKE_CURRENT_SOURCE_DIR}/../Library.Desktop
)

target_link_libraries( ${target} PRIVATE 
    Library.Shared
    Library.Desktop
    dxgi
    d3d11
    d3dcompiler # d3d10compiler d3d11compiler
    #dxcompiler
    dinput8 
    dxguid
    Microsoft.GSL::GSL
    Microsoft::DirectXTK
    assimp::assimp
    imgui::imgui
    # Microsoft::Effects11
)

# data 폴더를 실행 폴더( Debug, Release )로 복사 샘플 코드
add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../Library.Desktop/Content $<TARGET_FILE_DIR:${target}>/Content
        ${CMAKE_CURRENT_SOURCE_DIR}/Content $<TARGET_FILE_DIR:${target}>/Content
    COMMENT "Copying 'Content' folder to build output directory"
)

