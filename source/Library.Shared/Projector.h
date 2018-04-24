#pragma once

#include "GameComponent.h"
#include <DirectXMath.h>

namespace Library
{
	class GameTime;

	class Projector : public GameComponent
	{
		RTTI_DECLARATIONS(Projector, GameComponent)

	public:
		explicit Projector(Game& game, float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance);
		Projector(const Projector&) = default;
		Projector& operator=(const Projector&) = default;
		Projector(Projector&&) = default;
		Projector& operator=(Projector&&) = default;
		virtual ~Projector() = default;

		const DirectX::XMFLOAT3& Position() const;
		const DirectX::XMFLOAT3& Direction() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR PositionVector() const;
		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		float NearPlaneDistance() const;
		float FarPlaneDistance() const;

		DirectX::XMMATRIX ViewMatrix() const;
		DirectX::XMMATRIX ProjectionMatrix() const;
		DirectX::XMMATRIX ViewProjectionMatrix() const;

		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(DirectX::FXMVECTOR position);
		virtual void SetPosition(const DirectX::XMFLOAT3& position);

		virtual void Reset();
		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;
		virtual void ApplyRotation(DirectX::CXMMATRIX transform);
		virtual void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

		inline static const float DefaultNearPlaneDistance{ 0.5f };
		inline static const float DefaultFarPlaneDistance{ 100.0f };

	protected:
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mDirection;
		DirectX::XMFLOAT3 mUp;
		DirectX::XMFLOAT3 mRight;

		DirectX::XMFLOAT4X4 mViewMatrix;
		DirectX::XMFLOAT4X4 mProjectionMatrix;
	};
}