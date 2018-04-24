#pragma once

#include "Projector.h"

namespace Library
{
	class GameTime;

	class PerspectiveProjector : public Projector
	{
		RTTI_DECLARATIONS(PerspectiveProjector, Projector)

	public:
		explicit PerspectiveProjector(Game& game);
		PerspectiveProjector(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);
		PerspectiveProjector(const PerspectiveProjector&) = default;
		PerspectiveProjector& operator=(const PerspectiveProjector&) = default;
		PerspectiveProjector(PerspectiveProjector&&) = default;
		PerspectiveProjector& operator=(PerspectiveProjector&&) = default;
		virtual ~PerspectiveProjector() = default;

		float AspectRatio() const;
		float FieldOfView() const;

		virtual void UpdateProjectionMatrix() override;

		inline static const float DefaultFieldOfView{ DirectX::XM_PIDIV4 };
		inline static const float DefaultAspectRatio{ 4.0f / 3.0f };

	protected:
		float mFieldOfView;
		float mAspectRatio;
	};
}

