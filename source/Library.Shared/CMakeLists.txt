
set( target "Library.Shared" )

set( target_SOURCES
    AnimationClip.cpp AnimationClip.h
    AnimationPlayer.cpp AnimationPlayer.h
    BasicMaterial.cpp BasicMaterial.h
    BlendStates.cpp BlendStates.h
    Bloom.cpp Bloom.h
    Bone.cpp Bone.h
    BoneAnimation.cpp BoneAnimation.h
    Camera.cpp Camera.h
    ColorHelper.cpp ColorHelper.h
    ComputeShader.cpp ComputeShader.h
    ComputeShaderReader.cpp ComputeShaderReader.h
    ContentManager.cpp ContentManager.h ContentManager.inl
    ContentTypeReader.cpp ContentTypeReader.h ContentTypeReader.inl
    ContentTypeReaderManager.cpp ContentTypeReaderManager.h
    DepthMap.cpp DepthMap.h
    DepthMapMaterial.cpp DepthMapMaterial.h
    DepthStencilStates.cpp DepthStencilStates.h
    DirectionalLight.cpp DirectionalLight.h
    DirectXHelper.cpp DirectXHelper.h
    DomainShader.cpp DomainShader.h
    DomainShaderReader.cpp DomainShaderReader.h
    DrawableGameComponent.cpp DrawableGameComponent.h
    FirstPersonCamera.cpp FirstPersonCamera.h
    FpsComponent.cpp FpsComponent.h
    Frustum.cpp Frustum.h
    FullScreenQuad.cpp FullScreenQuad.h
    FullScreenQuadMaterial.cpp FullScreenQuadMaterial.h
    FullScreenRenderTarget.cpp FullScreenRenderTarget.h
    Game.cpp Game.h Game.inl
    GameClock.cpp GameClock.h
    GameComponent.cpp GameComponent.h
    GameException.cpp GameException.h
    GamePadComponent.cpp GamePadComponent.h
    GameTime.cpp GameTime.h
    GaussianBlur.cpp GaussianBlur.h
    GeometryShader.cpp GeometryShader.h
    GeometryShaderReader.cpp GeometryShaderReader.h
    Grid.cpp Grid.h
    HullShader.cpp HullShader.h
    HullShaderReader.cpp HullShaderReader.h
    ImGuiComponent.cpp ImGuiComponent.h
    KeyboardComponent.cpp KeyboardComponent.h
    Keyframe.cpp Keyframe.h
    Light.cpp Light.h Light.inl
    Material.cpp Material.h Material.inl
    MatrixHelper.cpp MatrixHelper.h
    Mesh.cpp Mesh.h
    Model.cpp Model.h
    ModelMaterial.cpp ModelMaterial.h
    ModelReader.cpp ModelReader.h
    MouseComponent.cpp MouseComponent.h
    OrthographicCamera.cpp OrthographicCamera.h
    pch.cpp pch.h
    PerspectiveCamera.cpp PerspectiveCamera.h
    PixelShader.cpp PixelShader.h
    PixelShaderReader.cpp PixelShaderReader.h
    Point.cpp Point.h Point.inl
    PointLight.cpp PointLight.h
    ProxyModel.cpp ProxyModel.h
    RasterizerStates.cpp RasterizerStates.h
    Ray.cpp Ray.h
    Rectangle.cpp Rectangle.h Rectangle.inl
    RenderableFrustum.cpp RenderableFrustum.h
    RenderStateHelper.cpp RenderStateHelper.h
    RenderTarget.cpp RenderTarget.h
    RTTI.cpp RTTI.h
    SamplerStates.cpp SamplerStates.h
    SceneNode.cpp SceneNode.h
    ServiceContainer.cpp ServiceContainer.h
    Shader.cpp Shader.h
    Skybox.cpp Skybox.h
    SkyboxMaterial.cpp SkyboxMaterial.h
    SpotLight.cpp SpotLight.h
    StopWatch.cpp Stopwatch.h StopWatch.inl
    StreamHelper.cpp StreamHelper.h
    StringHelper.cpp StringHelper.h
    Texture.cpp Texture.h Texture.inl
    Texture2D.cpp Texture2D.h
    Texture2DReader.cpp Texture2DReader.h
    TextureCube.cpp TextureCube.h
    TextureCubeReader.cpp TextureCubeReader.h
    TexturedModelMaterial.cpp TexturedModelMaterial.h
    TextureHelper.cpp TextureHelper.h
    Utility.cpp Utility.h
    VectorHelper.cpp VectorHelper.h VectorHelper.inl
    VertexDeclarations.cpp VertexDeclarations.h VertexDeclarations.inl
    VertexShader.cpp VertexShader.h
    VertexShaderReader.cpp VertexShaderReader.h
)

add_library( ${target} STATIC 
    ${target_SOURCES} 
)

target_precompile_headers( ${target} PUBLIC
    pch.h
)

target_link_libraries( ${target} PRIVATE
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


