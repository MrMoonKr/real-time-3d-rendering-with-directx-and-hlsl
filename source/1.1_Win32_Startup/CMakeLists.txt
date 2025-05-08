# CMakeLists.txt for Ch01_01_Win32_Startup

set( target Ch01_01_Win32_Startup )

add_executable( ${target} WIN32
    Program.cpp
    GameException.cpp GameException.h
)

target_precompile_headers( ${target} PRIVATE
    pch.h
)

# include_directories( ${target} PRIVATE 
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
# )

# target_include_directories( ${target} PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/../Common
# )

target_link_libraries( ${target} PRIVATE 
    dxgi
    d3d11
    # d3dcompiler # d3d10compiler d3d11compiler
    # dinput8 
    # dxguid
    Microsoft.GSL::GSL
    # Microsoft::DirectXTK
    # assimp::assimp
    # imgui::imgui
    # Microsoft::Effects11
)

# data 폴더를 실행 폴더( Debug, Release )로 복사 샘플 코드
# add_custom_command(
#     TARGET ${target} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:${target}>/data
#     COMMENT "Copying 'data' folder to build output directory"
# )

