#include "pch.h"
#include "ProjectedTextureMappingDemo.h"
#include "Game.h"
#include "Texture2D.h"
#include "TexturedModelMaterial.h"
#include "VertexDeclarations.h"
#include "Camera.h"
#include "VectorHelper.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace winrt;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	ProjectedTextureMappingDemo::ProjectedTextureMappingDemo(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
	}

	ProjectedTextureMappingDemo::~ProjectedTextureMappingDemo()
	{
	}

	void ProjectedTextureMappingDemo::Initialize()
	{
		const VertexPositionTextureNormal sourceVertices[]
		{
			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), Vector3Helper::Backward),

			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), Vector3Helper::Backward),
		};

		const span<const VertexPositionTextureNormal> vertices{ sourceVertices };
		mVertexCount = narrow_cast<uint32_t>(vertices.size());
		VertexPositionTextureNormal::CreateVertexBuffer(mGame->Direct3DDevice(), vertices, not_null<ID3D11Buffer**>(mVertexBuffer.put()));

		auto colorMap = mGame->Content().Load<Texture2D>(L"Textures\\Checkerboard.png"s);
		/*mMaterial = make_shared<SpotLightMaterial>(*mGame, colorMap, specularMap);
		mMaterial->Initialize();*/
	}

	void ProjectedTextureMappingDemo::Draw(const GameTime& gameTime)
	{
		gameTime;
	}
}