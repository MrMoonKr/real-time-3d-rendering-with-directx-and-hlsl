#include "pch.h"
#include "PerspectiveProjector.h"

using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(PerspectiveProjector)

	PerspectiveProjector::PerspectiveProjector(Game& game) :
		Projector(game),
		mFieldOfView(DefaultFieldOfView), mAspectRatio(DefaultAspectRatio)
	{
	}

	PerspectiveProjector::PerspectiveProjector(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		Projector(game, nearPlaneDistance, farPlaneDistance),
		mFieldOfView(fieldOfView), mAspectRatio(aspectRatio)
	{
	}

	float PerspectiveProjector::AspectRatio() const
	{
		return mAspectRatio;
	}

	float PerspectiveProjector::FieldOfView() const
	{
		return mFieldOfView;
	}

	void PerspectiveProjector::UpdateProjectionMatrix()
	{
		XMMATRIX projectionMatrix = XMMatrixPerspectiveFovRH(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
		XMStoreFloat4x4(&mProjectionMatrix, projectionMatrix);
	}
}
