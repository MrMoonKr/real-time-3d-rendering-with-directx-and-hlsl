# Real-Time 3D Rendering with DirectX and HLSL

WIP ( Work in Progress )  

C++ / DirectX 11 직무 교육( OJT, On the job Training )을 위해서 생성 하였습니다.  

## 책 관련 링크

<img src="https://learning.oreilly.com/library/cover/9780133570120/250w/" alt="" height="256px" align="right">

- [Real-Time 3D Rendering with DirectX and HLSL [ 원서 ]](https://www.oreilly.com/library/view/real-time-3d-rendering/9780133570120/)  

- [저자 저장소](https://bitbucket.org/pvarcholik/real-time-3d-rendering-with-directx-and-hlsl/src/) ( 최근 2023-07-11 )  

- 2014년 출시된 책의 소스를 저자가 직접 2023년까지 업데이트 하셨네요  

## 개발 및 테스트 환경

- 시스템 ( Computer System )  

  - AMD Ryzen 9 5900X 12-Core Processor
  - NVIDIA Geforce RTX 3080 10GB
  - Windows 11 64bit Korean

- 컴파일러 및 링커 ( Compiler , Linker )  

  - [Visual Studio Community 2022](https://visualstudio.microsoft.com/ko/free-developer-offers/)  
    - C++를 사용한 데스크톱 개발  
    - C++를 사용한 게임 개발  ( DirectX SDK 포함되어 있음 )

- 에디터 ( Editor, Intergrated Development Environment )  

  - [VS Code](https://visualstudio.microsoft.com/ko/free-developer-offers/)  
    - C/C++ Extension for VS Code
      - c_cpp_properties.json ( compiler path and IntelliSense settings )  
      - tasks.json ( build instructions )  
      - launch.json ( debugger settings )  
      - Chapter01 동작 테스트 후 CMake 도입함.  

- 빌드 매니저 ( Build Manager )
  - [CMake](https://cmake.org/)  
    - [설치](https://cmake.org/download/)  
    - ...
    ```
    $ cmake --version
    ```
  - CMakeLists.txt
  - CMakePresets.json
  - CMakeUserPresets.json

- 패키지 매니저 ( Package Manager )
  - [vcpkg](https://vcpkg.io/en/)  
    - [설치](https://github.com/microsoft/vcpkg)  
    - ...
    ```
    $ vcpkg --version
    $ vcpkg new --application
    ```
  - vcpkg.json
  - vcpkg-configuration.json


## 사용된 패키지 목록

- ms-gsl
  - [vcpkg](https://vcpkg.io/en/package/ms-gsl)
    ```
    $ vcpkg add port ms-gsl
    ```
  - [GSL](https://github.com/Microsoft/GSL)  
  - Microsoft-Guidelines-Support-Library

- directxtk
  - [vcpkg](https://vcpkg.io/en/package/directxtk)
    ```
    $ vcpkg add port directxtk
    ```
  - [DirectXTK](https://github.com/Microsoft/DirectXTK)  
  - DirectX 11 Helper Classes

- assimp
  - [vcpkg](https://vcpkg.io/en/package/assimp)
    ```
    $ vcpkg add port assimp
    ```
  - [assimp](https://github.com/assimp/assimp)  
  - Open-Asset-Importer-Library

- IMGUI
  - [vcpkg](https://vcpkg.io/en/package/imgui)  
  ```
  $ vcpkg add port imgui[win32binding,dx11-binding,freetype]
  ```
  - [imgui](https://github.com/ocornut/imgui)
  - Immediate-Mode-Graphical-User-Interface

- directx-dxc
  - [vcpkg](https://vcpkg.io/en/package/directx-dxc)  
  ```
  $ vcpkg add port directx-dxc
  ```
  - [DirectXShaderCompiler](https://github.com/microsoft/DirectXShaderCompiler)
  - DirectX Shader Compiler which is based on LLVM/Clang

- ...
  - [vcpkg]()  
  ```
  $ vcpkg add port ...
  ```
  - [...]()
  - ...  


## ...

---
---
---







### Overview

This is the source code repository for the book *Real Time 3D Rendering with DirectX and HLSL: A Practical Guide to Graphics Programming* by Paul Varcholik.

[![Real-Time 3D Rendering with DirectX and HLSL](http://www.varcholik.org/RealTime3DRendering/BookCover.jpg)](http://www.informit.com/store/real-time-3d-rendering-with-directx-and-hlsl-a-practical-9780321962720)

There is also a companion site for the book at [http://www.varcholik.org/](http://www.varcholik.org/). There you'll find errata and a forum for discussions and questions about the text.

### Prerequisites
* Windows 10
* Visual Studio 2022 or newer

### Quick Start (latest)

Retrieve this repository:
```
> git clone git clone https://bitbucket.org/pvarcholik/real-time-3d-rendering-with-directx-and-hlsl.git
```

Install vcpkg ([additional detail](https://github.com/microsoft/vcpkg)):
```
> git clone https://github.com/Microsoft/vcpkg.git
> cd vcpkg
> .\bootstrap-vcpkg.bat
> vcpkg integrate install
```

Install dependencies (assuming default triplet of x86-windows):
```
> vcpkg install ms-gsl directxtk assimp imgui
> vcpkg install ms-gsl:x64-windows directxtk:x64-windows assimp:x64-windows imgui:x64-windows
```

Install C++/WinRT Visual Studio Extension ([VSIX](https://marketplace.visualstudio.com/items?itemName=CppWinRTTeam.cppwinrt101804264))

Open the DirectX.sln file (within the build directory) in Visual Studio and enjoy!

### Using Older versions

Retrieve this repository:
```
> git clone https://bitbucket.org/pvarcholik/real-time-3d-rendering-with-directx-and-hlsl.git
> cd real-time-3d-rendering-with-directx-and-hlsl
> git checkout <tagname> (see list of tags below. e.g. git checkout v1.0)
```

### Tags
* v1.0 - 4/4/2014 - Original code published with the book (Visual Studio 2013 projects)
* v2.0 - 7/7/2019 - Significant refactoring (Visual Studio 2019 projects)
    * Visual Studio 2019 projects
    * [vcpkg](https://github.com/microsoft/vcpkg) for dependencies
    * Refactoring for reduced code duplication and reduced platform dependencies
    * Pre-compiled headers
    * Increased warning level (and now treating warnings as errors)
	* More C++11 usage
	* Replaced most raw pointers with smart pointers
	* Additional refactoring for [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) 
	* Bug fixes
    * [ImGui](https://github.com/ocornut/imgui) usage
    * Removed dependency on Effects library
    * Redeveloped material system
    * .fx files have been replaced with separate .hlsl files for associated shader types

### Library Dependencies

* [GSL](https://github.com/Microsoft/GSL) - Guidlines Support Library (Microsoft)
* [DirectXTK](https://github.com/microsoft/DirectXTK) - DirectX Tool Kit
* [Assimp](http://www.assimp.org/) - Open Asset Import Library
* [ImGui](https://github.com/ocornut/imgui) - Dear ImGui

### Chapter-to-Project mappings
With the refactoring of v2.0, I have changed the organization of the projects. With v1.0 I used NVIDIA FX Composer to demonstrate the techniques for the earlier chapters of the book. I had also included Visual Studio projects for these early-chapter topics. With v2.0, I have removed the NVIDIA FX Composer dependency and demonstrate these topics entirely through C++ projects. This mirrors the companion resource of the [DirectX Essentials LiveLessons video series](http://www.safaribooksonline.com/library/view/directx-essentials-livelessons/9780134030036/). Below is a mapping between the current Visual Studio projects and the material in the book.

* Chapter 4
    * 1_1_Win32_Startup
    * 1_1_Win32_Framework
    * 1_3_Point
    * 1_4_Color_Triangle
    * 1_5_Colored_Cube
* Chapter 5
    * 2_1_Model
    * 2_2_Textured_Model
    * 2_3_Texture_Addressing
    * 2_4_Texture_Filtering
* Chapter 6
    * 3_1_Ambient_Lighting
    * 3_2_Diffuse_Lighting
    * 3_3_Specular_Highlights
* Chapter 7
    * 4_1_Point_Light
    * 4_2_Spot_Light
    * 4_3_Multiple_Point_Lights
* Chapter 8
    * 5_1_Skybox
    * 5_2_Environment_Mapping
    * 5_3_Fog
    * 5_4_Transparency
* Chapter 9
    * 6_1_Normal_Mapping
    * 6_2_Displacement_Mapping
* Chapters 10-17
    * The material in these chapters is demonstrated starting with 1_1_Win32_Startup and continuing through 6_2_Displacement_Mapping_
* Chapter 18
    * 7_1_Color_Filtering
    * 7_2_Gaussian_Blurring
    * 7_3_Bloom
    * 7_4_Distortion_Mapping
    * 7_5_Distortion_Masking
* Chapter 19
    * 8_1_Projective_Texture_Mapping
    * 8_2_Shadow_Mapping
* Chapter 20
    * 9_1_Animation
* Chapter 21
    * 10_1_Point_Sprites
    * 10_2_Basic_Tessellation
    * 10_3_Heightmap_Tessellation
    * 10_4_Dynamic_LOD
* Chapter 22
    * 11_1_Compute_Shaders

### Using v1.0 (This material is out-of-date with the head revision)

Chapters 4 through 9 are intended for use with [NVIDIA FX Composer](https://developer.nvidia.com/fx-composer).

Chapters 10 through 22 require Visual Studio 2013 (any sku, including [Express 2013 for Windows Desktop](http://www.visualstudio.com/en-us/products/visual-studio-express-vs.aspx)).

There is no source for Chapters 1 through 3.

### Attention Windows 8.1 Users

The latest revision of NVIDIA FX Composer (which is used in Chapters 4-9) does not run under Windows 8.1. However, [an older version (2.51.0701.1135)](http://www.softpedia.com/progDownload/NVIDIA-FX-Composer-Download-104791.html) does work. I have tested all of the .fxcproj files included with the source code, they all load correctly and their corresponding shaders run without issue.